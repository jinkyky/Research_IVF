#include "utils.h"
#include <fstream>
#include <iostream>
#include <random>
#include <cstdint>
#include <iomanip>

DataSet load_data(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        exit(1);
    }

    uint32_t num_vectors, dim;
    in.read(reinterpret_cast<char*>(&num_vectors), sizeof(num_vectors));
    in.read(reinterpret_cast<char*>(&dim), sizeof(dim));

    DataSet data;
    data.reserve(num_vectors);
    for (uint32_t i = 0; i < num_vectors; ++i) {
        Data_Vector vec(dim);
        in.read(reinterpret_cast<char*>(vec.data()), dim * sizeof(float));
        data.emplace_back(std::move(vec));
    }

    std::cout << "Loaded " << data.size() 
              << " vectors, dimension = " << dim 
              << std::endl;
    return data;
}

void generate_test_data(
    const std::string& filename, 
    int num_vectors, 
    int dim
) {
    std::ofstream out(filename, std::ios::binary);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

    uint32_t num = num_vectors;
    uint32_t dim_val = dim;
    out.write(reinterpret_cast<const char*>(&num), sizeof(num));
    out.write(reinterpret_cast<const char*>(&dim_val), sizeof(dim_val));

    for (int i = 0; i < num_vectors; ++i) {
        Data_Vector vec(dim);
        for (int j = 0; j < dim; ++j) {
            vec[j] = dis(gen);
        }
        out.write(
            reinterpret_cast<const char*>(vec.data()), 
            dim * sizeof(float)
        );
    }

    std::cout << "Generated test data: " << filename 
              << " (" << num_vectors << "x" << dim << ")"
              << std::endl;
}

void print_hex(uint8_t value) {
    std::cout << std::hex << std::uppercase  // 切换到十六进制并使用大写字母
              << std::setw(2) << std::setfill('0')  // 确保始终输出两位数
              << static_cast<int>(value)  // 防止 uint8_t 误解释为 char
              << std::dec;  // 切回十进制模式
}
void print_hex(const std::string& filename, int lines, int bytes_per_line) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Cannot open file: " << filename << std::endl;
        return;
    }

    std::vector<uint8_t> buffer(bytes_per_line);
    for (int line = 0; line < lines; ++line) {
        file.read(reinterpret_cast<char*>(buffer.data()), bytes_per_line);
        std::streamsize bytes_read = file.gcount();
        if (bytes_read == 0) break;  // 文件读取结束

        // 输出偏移地址
        std::cout << std::setw(8) << std::setfill('0') << std::hex << line * bytes_per_line << ": ";
        
        // 输出十六进制数据
        for (std::streamsize i = 0; i < bytes_read; ++i) {
            std::cout << std::setw(2) << std::setfill('0') << std::hex 
                      << static_cast<int>(buffer[i]) << " ";
        }

        // 对齐不足16字节的行
        for (std::streamsize i = bytes_read; i < bytes_per_line; ++i) {
            std::cout << "   ";
        }

        // 输出 ASCII 视图
        std::cout << " |";
        for (std::streamsize i = 0; i < bytes_read; ++i) {
            char c = buffer[i];
            std::cout << (isprint(c) ? c : '.');
        }
        std::cout << "|" << std::endl;
    }
}
std::vector<float> convertVector(const Data_Vector& input) {
    // 转化data向量为float类型
    std::vector<float> output;
    output.reserve(input.size());  // 预分配空间
    for (auto val : input) {
        output.push_back(static_cast<float>(val));
    }
    return output;
}