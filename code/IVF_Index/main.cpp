#include "IVF.h"
#include "utils.hpp"
#include <chrono>
#include <iostream>

int main() {
    try {
        // test data set
        generate_test_data("test_data.bin", 10000, 128);

        // load
        Dataset base = load_data("test_data.bin");  
        Dataset queries = load_data("test_data.bin"); 

        auto start = std::chrono::high_resolution_clock::now();

        IVFIndex index(256, 20);  // 256 clusters, 20 vectors per cluster

        index.build(base);
        auto build_time = std::chrono::high_resolution_clock::now() - start;

        // query
        const int test_query = 0;
        start = std::chrono::high_resolution_clock::now();

        int k = 30; /*k nearest neighbors ,value of k can be changed*/

        auto results = index.search(queries[test_query], k);
        auto query_time = std::chrono::high_resolution_clock::now() - start;

        // result
        std::cout << "\n=== Performance ===" << std::endl;
        std::cout << "Build time: " 
                  << std::chrono::duration<double>(build_time).count() 
                  << "s" << std::endl;
        std::cout << "Query time: " 
                  << std::chrono::duration<double>(query_time).count() 
                  << "s" << std::endl;

        std::cout << "\nTop " << k << " neighbors: ";
        for (int id : results) std::cout << id << " ";
        std::cout << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}