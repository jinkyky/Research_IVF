#ifndef KNNPACK_H
#define KNNPACK_H
//#define TEST_1B
#include <cstdint>
#include <vector>

#ifdef TEST_1B
typedef uint8_t ElemType; //测试集向量分量的类型,bvec是8位无符号整数
#else
typedef float ElemType;
#endif
template <typename T>
using VecSet = std::vector<std::vector<T>>;

using Data_Vector = std::vector<ElemType>;
using Index_Vector = std::vector<int>;
using DataSet = VecSet<ElemType>;
using IndexSet = VecSet<int>;

#endif