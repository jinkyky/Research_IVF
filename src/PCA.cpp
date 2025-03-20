// pca.cpp
#include "PCA.h"
#include <Eigen/Dense>
#include <numeric>  // 新增头文件用于特征值排序

PCA::PCA(int n_components) : n_components(n_components) {}

void PCA::fit(const DataSet& data) {
    if (data.empty() || data[0].empty()) {
        throw std::invalid_argument("Input data cannot be empty");
    }
    const int rows = data.size();
    const int cols = data[0].size();
    VecSet<float> float_data(rows, std::vector<float>(cols));

    for (int i = 0; i < rows; ++i) {
        std::transform(data[i].begin(), data[i].end(), float_data[i].begin(),
                    [](ElemType v) { return static_cast<float>(v); });
    }

    
    // 转换为Eigen矩阵
    Eigen::MatrixXf mat(rows, cols);
    for (int i = 0; i < rows; ++i)
        mat.row(i) = Eigen::VectorXf::Map(float_data[i].data(), cols);
    
    // 计算均值
    mean = std::vector<float>(cols, 0);
    for (const auto& vec : float_data)
        for (int j = 0; j < cols; ++j) mean[j] += vec[j];
    for (auto& m : mean) m /= rows;
    
    // 中心化（已修复行向量问题）
    mat.rowwise() -= Eigen::RowVectorXf::Map(mean.data(), cols);
    
    // 计算协方差矩阵
    Eigen::MatrixXf cov = (mat.adjoint() * mat) / (rows - 1);
    
    // 特征分解
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf> solver(cov);
    Eigen::VectorXf eigenvalues = solver.eigenvalues();
    Eigen::MatrixXf eigen_vectors = solver.eigenvectors();
    
    // 特征值排序（新增稳定性改进）
    std::vector<int> indices(cols);
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(),
        [&](int a, int b) { return eigenvalues[a] > eigenvalues[b]; });
    
    // 取前n_components个特征向量
    components.resize(n_components);
    for (int i = 0; i < n_components; ++i) {
        components[i] = std::vector<float>(cols);
        Eigen::VectorXf v = eigen_vectors.col(indices[i]);
        std::copy(v.data(), v.data() + cols, components[i].begin());
    }
}

DataSet PCA::transform(const DataSet& data) const {
    DataSet result;
    for (const auto& vec : data)
        result.push_back(transform(vec));
    return result;
}

Data_Vector PCA::transform(const Data_Vector& vec) const {
    Data_Vector proj(n_components, 0);
    std::vector<float> centered(vec.size());
    for (size_t i = 0; i < vec.size(); ++i)
        centered[i] = (float)vec[i] - mean[i];

    float sum = 0;
    for (int i = 0; i < n_components; ++i) {
        for (size_t j = 0; j < centered.size(); ++j)
            sum += centered[j] * components[i][j];
        proj[i] = static_cast<ElemType>(sum); //pca算法的近似解，因为向量所在域可能为uint8_t而不一定是float
    }
    return proj;
}