## Construct a high-dimensional vector index with IVF

​		The code implements the construction and query of high-dimensional vector indexes using K-means and IVF algorithms, and uses the method of randomly generating test datasets for testing.

### Parameters

```c++
int num_vectors = 1000 //number of vectors
int dim = 128 // vector dimention
int k = 30; //k-nearest neighbours
IVFIndex index(256, 20);  // 256 clusters, 20 vectors per cluster
```

### Run the code

