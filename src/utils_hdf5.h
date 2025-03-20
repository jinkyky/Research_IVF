#ifndef UTILSHDF5_H
#define UTILSHDF5_H
#include <string>
#include <vector>
#include <H5Cpp.h>
#include <fstream>
#include <iostream>
#include "KNNPack.h"

template <typename T>
VecSet<T> load_data_hdf5( //模板函数必须放在头文件里实现
    const std::string &filename,
    const std::string &dataset_name)
{
    try
    {
        // open hdf5
        H5::H5File in(filename, H5F_ACC_RDONLY);
        // open group
        H5::DataSet dataset = in.openDataSet(dataset_name);
        H5::DataSpace dataspace = dataset.getSpace();

        // get information of dimension
        hsize_t dims[2];
        dataspace.getSimpleExtentDims(dims, nullptr);
        size_t num_vectors = dims[0];
        size_t dim = dims[1];

        std::vector<T> buffer(num_vectors * dim);
        dataset.read(buffer.data(), H5::PredType::NATIVE_FLOAT);
        // read data into buffer
        if(dataset_name == "/neighbors") {
            dataset.read(buffer.data(), H5::PredType::NATIVE_INT);
        } else {
            dataset.read(buffer.data(), H5::PredType::NATIVE_FLOAT); //dataset_name == train || test时格式位NATIVE_FLOAT, neighbor时格式为NATIVE_INT
        }
        //to format of DataSet
        VecSet<T> data;
        data.reserve(num_vectors);
        for (size_t i = 0; i < num_vectors; ++i)
        {
            std::vector<T> vec(
                buffer.begin() + i * dim,
                buffer.begin() + (i + 1) * dim);
            data.emplace_back(std::move(vec));
        }

        std::cout << "Loaded SIFT dataset '" << dataset_name
                  << "' (" << num_vectors << "*" << dim << ")"
                  << std::endl;
        return data;
    } catch (const H5::Exception& e){
        std::cerr << "HDF5 Error: " << e.getCDetailMsg() << std::endl;
        throw std::runtime_error("Failed to load HDF5 data");
    }
}
#endif