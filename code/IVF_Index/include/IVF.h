#pragma once
#include "k_means.h"
#include <queue>
#include <vector>

class IVFIndex {
public:
    IVFIndex(int nlist_, int nprobe_, int max_iter_ = 100);
    void build(const DataSet& data);
    std::vector<int> search(const Vector& query, int k) const;

private:
    int nlist;
    int nprobe;
    KMeans kmeans;
    DataSet data;
    DataSet centroids;
    std::vector<std::vector<int>> inverted_lists_;
    
    void build_inverted_lists(const DataSet& data);
    std::vector<int> find_nearest_clusters(const Vector& query, int nprobe) const;
};