## Construct a high-dimensional vector index with IVF

​		The code implements the construction and query of high-dimensional vector indexes using K-means and IVF algorithms, and uses the method of randomly generating test datasets for testing.

### Parameters

you can change the parameters in the code to test more samples
```c++
int num_vectors = 1000 //number of vectors
int dim = 128 // vector dimention
int k = 30; //k-nearest neighbours
IVFIndex index(256, 20);  // 256 clusters, 20 vectors per cluster
```

### Run the code

1. download code from github [code](https://github.com/jinkyky/Research_IVF.git)

2. open terminal , enter : 

   ```js
   cd code/IVF_Index
   ```

3. compile

   ```js
   g++ -O3 -mavx2 -fopenmp -Iinclude src/k_means.cpp src/IVF.cpp src/utils.cpp main.cpp -o ivf_demo
   ```

   or use cmake:

   ```js
   cmake -G "MinGW Makefiles" .
   mingw32-make.exe all 
   ```

   or mount all files in one directory , then:

   ```js
   g++ IVF.cpp IVF.h k_means.cpp k_means.h main.cpp utils.cpp utils.hpp -o ivf_demo
   ```

4. finally the executable file ivf_demo.exe is generated,run it

### Result examples

examples are given in the photo folder
