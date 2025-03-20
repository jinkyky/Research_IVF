#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <vector>
#include "KNNPack.h"
#include <fstream>
#include <iostream>

DataSet load_data(const std::string& filename);

template <typename T>
VecSet<T> load_data_xvec(const std::string& filename, int max_read) {
    // 根据向量类型T读取.fvec或.bvec或.ivec类型文件, 假定每个向量的维数是相同的
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        exit(1);
    }

    uint32_t dim;
    VecSet<T> data;
    int read_num = 0;
    // int vector_size = 4 + dim * sizeof(T); 包括一个维度域（int类型）和一个数据域（T类型，小端规则）
    std::cout << "reading " << "vectors ..." << std::endl; 
    while(read_num < max_read) {
        // 读取向量维度，4个byte
        if(!in.read(reinterpret_cast<char*>(&dim), sizeof(dim))) break;

        std::vector<T> vec(dim);
        if(!in.read(reinterpret_cast<char*>(vec.data()), dim * sizeof(T))) break;

        data.push_back(std::move(vec));
        ++read_num;
    }

    std::cout << "Loaded " << data.size() 
              << " vectors, dimension = " << dim 
              << std::endl;
    return data;
}

void print_hex(uint8_t value);
void print_hex(const std::string& filename, int lines = 10, int bytes_per_line = 16);
std::vector<float> convertVector(const Data_Vector& input);

void generate_test_data(
    const std::string& filename,
    int num_vectors = 10000,
    int dim = 128
);
#endif