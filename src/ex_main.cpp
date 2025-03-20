#include <chrono>
#include <iostream>
#include <algorithm>
#include <omp.h>
#include <random>
#include "IVF.h"
#include "utils.h"
#include "utils_hdf5.h"
#include "KNNPack.h"
#include "PCA.h"

using namespace std;
int main(int argc, char* argv[])
{
    try
    {
        /*random
        // 生成测试数据集（128维）
        generate_test_data("test_data.bin", 10000, 128);

        // 加载原始数据
        DataSet base = load_data("test_data.bin");
        DataSet queries = load_data("test_data.bin");
        */
        // 查询前k_recall=100 近的向量，若使用1 Billion大小数据集，则查询前1000个
        // SIFT
        #ifdef TEST_1B
        IndexSet neighbors = load_data_xvec<int>("data/gnd/idx_1000M.ivecs", 1e4);
        DataSet queries = load_data_xvec<ElemType>("data/bigann_query.bvecs",1e4);
        DataSet base = load_data_xvec<ElemType>("data/bigann_base.bvecs", 1e9);
        #else //默认使用1M数据
        DataSet base = load_data_hdf5<ElemType>("data/sift-128-euclidean.hdf5", "/train");
        DataSet queries = load_data_hdf5<ElemType>("data/sift-128-euclidean.hdf5", "/test");
        IndexSet neighbors = load_data_hdf5<int>("data/sift-128-euclidean.hdf5", "/neighbors");
        #endif

        // 默认查询前k_recall=100 近的向量，若使用1B数据集，则查询前1000个
        int k_recall = 100;
        #ifdef TEST_1B
        k_recall = 1000;  
        #endif

        bool use_pca = false;
        int query_cnt = 1;
        int limit_thread = 32; 
        // 解析命令行参数
        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];

            // 处理 --use-pca
            if (arg == "--use-pca") {
                use_pca = true;
            }
            // 处理 --limit-thread=x
            else if (arg.find("--limit-thread=", 0) == 0) {  // 检查是否以 "--limit-thread=" 开头
                std::string value = arg.substr(15); // 截取等号后的部分
                limit_thread = std::atoi(value.c_str());
                std::cout << "limit thread is " << limit_thread << std::endl;
            }
            // 处理-iter=y
            else if (arg.find("--iter=", 0) == 0) {  // 检查是否以 "--iter=" 开头
                std::string value = arg.substr(7); // 截取等号后的部分
                query_cnt = std::atoi(value.c_str());
                std::cout << "query_cnt is " << query_cnt << std::endl;
            }
            // 无效参数
            else {
                std::cerr << "Usage: ./ex_main [--use-pca] [--limit-thread=x] [--iter=y]\n"
                        << "'iter' is query_cnt\n";
                return 1;
            }
        }
        //set threads
        omp_set_num_threads(limit_thread);
        const int origin_dim = 128;
        const int pca_dim = 64;
        const int base_size = base.size();
        const int query_size = queries.size();
        std::cout << "base size " << base_size << " query size " << query_size << std::endl; 

        // ==== PCA降维处理 ====
        std::chrono::duration<int64_t, std::nano> PCA_time;
        DataSet base_lowdim, queries_lowdim;
        if (use_pca) {
            auto start = std::chrono::high_resolution_clock::now();
            PCA pca(pca_dim);

            // 仅在训练集(base)上拟合PCA模型
            std::cout << "Fitting PCA...\n";
            pca.fit(base);

            // 转换所有数据到低维空间
            std::cout << "Transforming data...\n";
            base_lowdim = pca.transform(base);
            queries_lowdim = pca.transform(queries);
            PCA_time = std::chrono::high_resolution_clock::now() - start;
        }

        // 使用降维后的数据构建IVF索引
        auto start = std::chrono::high_resolution_clock::now();
        IVFIndex index(256, 20); // 256个聚类，探测20个最近簇
        if (use_pca) {
            index.build(base_lowdim);
        } else {
            index.build(base);
        }
        auto build_time = std::chrono::high_resolution_clock::now() - start;

        // 执行查询（使用降维后或不降维的查询向量）, test_query < 10000
        std::chrono::nanoseconds total_query_time = std::chrono::nanoseconds{0};
        int total_recall_count = 0;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, query_size - 1);
        for(int i = 0; i < query_cnt; ++i) { //多次查询，每次查询采用均匀分布的下标
            int test_query = dis(gen);
            start = std::chrono::high_resolution_clock::now();
            auto results = index.search(use_pca ? queries_lowdim[test_query] : queries[test_query], k_recall); //根据是否使用pca降维决定使用什么查询向量
            if (results.empty())
            {
                throw std::runtime_error("Search returned empty results");
            }
            auto single_query_time = std::chrono::high_resolution_clock::now() - start;
            total_query_time += single_query_time;
            
            // 计算recall
            for (int i = 0; i < k_recall; i++) {
                //如果result[i]在实际最近k_recall个邻居之中
                if (std::find(neighbors[test_query].begin(), neighbors[test_query].begin() + k_recall, results[i]) != neighbors[test_query].begin() + k_recall) {
                    ++total_recall_count;
                }
            }
            // 输出搜索结果
            std::cout << "\nTop " << k_recall << " neighbors of  queries[" << test_query << "]: ";
            for (int id : results)
                std::cout << id << " ";
            std::cout << std::endl;
        }
        std::cout << "total recall is " << total_recall_count << std::endl;
        float recall = float(total_recall_count) / float(k_recall * query_cnt); //平均召回率，仅考虑前k_recall个邻居
        auto query_time = total_query_time / query_cnt;

        // 输出性能结果
        std::cout << "query_cnt: " << query_cnt << std::endl;
        std::cout << "\n=== Performance With/Without PCA===" << std::endl;
        std::cout << "using threads: " << limit_thread << std::endl;
        std::cout << "Original dimension: " << origin_dim
                  << " -> Reduced dimension: " << (use_pca ? pca_dim : origin_dim) << std::endl;
        std::cout << "PCA time: "
                  << (use_pca ? std::chrono::duration<double>(PCA_time).count() : 0)
                  << "s" << std::endl;
        std::cout << "Build time: "
                  << std::chrono::duration<double>(build_time).count()
                  << "s" << std::endl;
        std::cout << "Average Query time: "
                  << std::chrono::duration<double>(query_time).count()
                  << "s" << std::endl;

        // 输出recall
        std::cout << "=== Recall ===" << std::endl;
        std::cout << "Top " << k_recall << " neighbors of average recall: " << recall << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}