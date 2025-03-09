cmake_minimum_required(VERSION 3.10)
project(IVF_Demo)

# 编译选项
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -mavx2 -fopenmp")

# 包含头文件目录
include_directories(include)

# 添加源文件
file(GLOB_RECURSE SOURCES "src/*.cpp" "main.cpp")

# 生成可执行文件
add_executable(ivf_demo ${SOURCES})

# 链接OpenMP（如果需要）
find_package(OpenMP REQUIRED)
target_link_libraries(ivf_demo OpenMP::OpenMP_CXX)