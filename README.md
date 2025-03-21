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



### Paper Reading

| Title                                                        | Content                                                      | Creative points                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| [Elastic deep autoencoder for text embedding **clustering** by an improved graph regularization](https://www.sciencedirect.com/science/article/pii/S0957417423022820) | 1. 提出了一种名为EDA-TEC的文本嵌入聚类方法，旨在学习输入数据的压缩表示，以捕获最重要的特征，同时丢弃噪音和不相关的细节。<br/>2. 引入加权余弦相似度（Weighted Cosine Similarity）替代传统余弦相似度，通过加权汉明距离（WHD）量化文本向量间的共同特征。<br/>3. 将自编码器的表示学习与聚类任务联合优化，通过KL散度损失同步更新聚类中心和嵌入表示，实现聚类导向的特征降维。 | 1. **端到端学习**：避免传统“先降维后聚类”的误差累积，特征表示直接适配聚类目标<br/>2. **可解释性**：稀疏正则化简化潜在表示，突出关键特征，提升聚类结果的可解释性 |
| [Various **dimension reduction** techniques for **high dimensional** data analysis: a review](https://link.springer.com/article/10.1007/s10462-020-09928-0) | 回顾各种降维方式，如PCA、线性判别分析（LDA）、奇异值分解（SVD）、局部保持投影（LPP）、t-分布随机邻域嵌入（t-SNE）等 |                                                              |
| [A novel approach for dimension reduction using word embedding: An enhanced text classification approach](https://www.sciencedirect.com/science/article/pii/S2667096822000052) | 结合 **tf-idf** 和 **GloVe 词嵌入** 的维度缩减方法（rRF），通过聚类语义相似的词并保留每类中 tf-idf 最高的词，减少冗余特征，提升分类性能 | 冗余特征移除（rRF）                                          |
| [Understanding how dimension reduction tools work: an empirical approach to deciphering t-SNE, UMAP, TriMAP, and PaCMAP for data visualization](http://www.jmlr.org/papers/v22/20-1061.html) | 1. 总结之前降维方式，如t-SNE、UMAP等，分析不足<br/>2. 提出新算法PaCMAP | 通过动态调整图中组件和权重，同时保留局部与全局结构           |

### Results

#### With PCA

| Max Threads | PCA Time(s) | Build Time(s) | Average Query Time(s) | Average Recall | Query Times(random) |
| ----------- | ----------- | ------------- | --------------------- | -------------- | ------------------- |
| 32          | 8.97984     | 40.9737       | 0.0287924             | 0.793          | 100                 |
| 16          | 8.95436     | 53.4252       | 0.0277089             | 0.7838         | 100                 |
| 12          | 9.0202      | 66.7962       | 0.0213911             | 0.7957         | 100                 |
| 8           | 8.94929     | 97.7177       | 0.0209131             | 0.7992         | 100                 |
| 4           | 8.80589     | 185.53        | 0.021448              | 0.7927         | 100                 |



#### Without PCA

| Max Threads | PCA Time(s) | Build Time(s) | Average Query Time(s) | Average Recall | Query Times(random) |
| ----------- | ----------- | ------------- | --------------------- | -------------- | ------------------- |
| 32          | 0           | 50.2272       | 0.0307196             | 0.9845         | 100                 |
| 16          | 0           | 82.7349       | 0.0309912             | 0.9778         | 100                 |
| 12          | 0           | 103.519       | 0.030559              | 0.9843         | 100                 |
| 8           | 0           | 149.937       | 0.0306606             | 0.9896         | 100                 |
| 4           | 0           | 279.007       | 0.0310299             | 0.9829         | 100                 |

### Update

###### 2025/3/13

1. add PCA to realize dimensionality reduction
2. use SIFT dataset to take place of random one

