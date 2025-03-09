#include "IVF.h"
#include <algorithm>
#include <cmath>  

struct Candidate {
    int id;
    double dist;  
    Candidate(int i, double d) : id(i), dist(d) {}
    bool operator<(const Candidate& other) const { return dist < other.dist; }
};

IVFIndex::IVFIndex(int nlist_, int nprobe_, int max_iter_)
    : nlist(nlist_), nprobe(nprobe_), 
      kmeans(nlist_, max_iter_, false), data() {}

void IVFIndex::build(const DataSet& data_) {
    data = data_;
    kmeans.fit(data_);  
    centroids = kmeans.get_centroids();
    build_inverted_lists(data_);
}

void IVFIndex::build_inverted_lists(const DataSet& data) {
    inverted_lists_.resize(nlist);
    const auto& labels = kmeans.get_labels();
    
    for (size_t i = 0; i < data.size(); ++i) {
        int cluster_id = labels[i];
        if (cluster_id >= 0 && cluster_id < nlist) {  
            inverted_lists_[cluster_id].push_back(i);
        }
    }
}

std::vector<int> IVFIndex::find_nearest_clusters(
    const Vector& query, int nprobe
) const {
    std::priority_queue<Candidate> pq;
    
    for (int c = 0; c < nlist; ++c) {
        double dist = KMeans::distance(query, centroids[c]);
        pq.emplace(c, dist);
        if (pq.size() > nprobe) pq.pop();
    }
    
    std::vector<int> result;
    while (!pq.empty()) {
        result.push_back(pq.top().id);
        pq.pop();
    }
    std::reverse(result.begin(), result.end());  
    return result;
}

std::vector<int> IVFIndex::search(const Vector& query, int k) const {
    auto cluster_ids = find_nearest_clusters(query, nprobe);
    std::priority_queue<Candidate> pq;
    
    for (int c : cluster_ids) {
        for (int vec_id : inverted_lists_[c]) {
            double dist = KMeans::distance(query, data[vec_id]);
            pq.emplace(vec_id, dist);
            if (pq.size() > k) pq.pop();
        }
    }
    
    std::vector<int> result;
    while (!pq.empty()) {
        result.push_back(pq.top().id);
        pq.pop();
    }
    std::reverse(result.begin(), result.end());  
    return result;
}