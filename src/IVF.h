#pragma once
#include "k_means.h"
#include <queue>
#include "KNNPack.h" 
#include <vector>

class IVFIndex {
public:
    IVFIndex(int nlist_, int nprobe_, int max_iter_ = 100);
    void build(const DataSet& data);
    std::vector<int> search(const Data_Vector& query, int k) const;

private:
    int nlist; //n个区块，泰森多边形
    int nprobe; //查找邻居时选择相邻的区块个数
    KMeans kmeans;
    DataSet data;
    VecSet<float> centroids;
    std::vector<std::vector<int>> inverted_lists_;
    
    void build_inverted_lists(const DataSet& data);
    std::vector<int> find_nearest_clusters(const Data_Vector& query, int nprobe) const;
};