#ifndef K_MEANS_H
#define K_MEANS_H
#include <vector>
#include "KNNPack.h"
#include <stdexcept>
class KMeans {
public:
    KMeans(int k_, int max_iter_ = 20, bool verbose_ = false);
    void fit(const DataSet& data);
    const VecSet<float>& get_centroids() const { return centroids; }
    const std::vector<int>& get_labels() const { return labels; }
    template <typename T1, typename T2>
    static double distance(const std::vector<T1> &a, const std::vector<T2> &b) {
        if (a.empty() || b.empty() || a.size() != b.size()) {
            throw std::invalid_argument("Vectors must be non-empty and same dimension");
        }
        double dist = 0;
        #pragma omp simd reduction(+:dist)
        for(size_t i = 0; i < a.size(); i++)
        {   
            dist += (static_cast<double>(a[i]) - static_cast<double>(b[i])) * (static_cast<double>(a[i]) - static_cast<double>(b[i]));
        }
        return dist; // sqrt is worse
    } 

private:
    int k;
    int max_iter;
    bool verbose;
    VecSet<float> centroids; //聚心为浮点数！
    std::vector<int> labels;
    
    void init_centroids(const DataSet& data);
    bool update_centroids(const DataSet& data);
};
#endif