// pca.h
#ifndef PCA_H
#define PCA_H
#include <vector>
#include "KNNPack.h"

class PCA {
public:
    PCA(int n_components);
    void fit(const DataSet& data);
    DataSet transform(const DataSet& data) const;
    Data_Vector transform(const Data_Vector& vec) const;
    int get_dim() const { return n_components; }
    
private:
    int n_components;
    std::vector<float> mean;
    VecSet<float> components; //特征向量
};
#endif