# Question: https://projecteuler.net/problem=704

"""
  . The first few values of F(n) suggest that F(n) is of this sequence http://oeis.org/A119387.
  . I.e., F(n) is the number of unchanged 0's and 1's when changing from n to n+1.
    Or, it's the popcount of (n xor n+1).
  . We can group the numbers by the number of digits of each number in binary.
    I.e.,
        Cluster | n          | bin(n)    | bin(n+1)  | popcount(n xor n+1) | Notes
        --------|------------|-----------|-----------|---------------------|---------------------
              0 | 0          | 0         | 1         | 0                   |
        --------|------------|-----------|-----------|---------------------|---------------------
              1 | 1          | 01        | 10        | 0                   | This is the first node
                | 2          | 10        | 11        | 1                   | {2} is adding 1 before each member of cluster 0
        --------|------------|-----------|-----------|---------------------|---------------------
              2 | 3          | 011       | 100       | 0                   | This is the first node
                | 4          | 100       | 101       | 2                   | {4} is adding 10 before each member of cluster 0
                | 5          | 101       | 110       | 1                   | {5, 6} is adding 1 before each member of cluster 1
                | 6          | 110       | 111       | 2                   |
  . We can see that,
      . Cluster k composes of numbers with k+1 digits in binary.
      . We can build cluster k by, adding the first node, which already has k+1 digits
                                   adding k-1 digits before each member of cluster 0
                                   adding k-2 digits before each member of cluster 1
                                   ...
                                   adding 1 digit before each member of cluster k-1
  . From that, we can build data structure to sum F(n).
"""

from math import log, ceil

N = 10**16

class Cluster:
    def __init__(self, cluster_sum, cluster_size, n_digits):
        self.sum = cluster_sum
        self.size = cluster_size
        self.n_digits = n_digits
        self.sub_clusters = []
    def add_sub_cluster(self, sub_cluster):
        self.sub_clusters.append(sub_cluster)
    def get_partial_cluster_sum(self, path, n_elements, n_considering_digits):
        if self.size <= n_elements:
            return self.sum + (n_considering_digits - self.n_digits) * self.size, n_elements - self.size

        n_remaining = n_elements

        ans = 0

        for sub_cluster_idx in range(len(self.sub_clusters)):
            if n_remaining == 0:
                break
            sub_cluster = self.sub_clusters[sub_cluster_idx]
            sub_sum, new_n_remaining = sub_cluster.get_partial_cluster_sum(path + [sub_cluster.size], n_remaining, n_considering_digits)
            ans += sub_sum
            n_remaining = new_n_remaining
        return ans, n_remaining
            

def solve(n):
    ans = 0

    n_clusters = int(ceil(log(n)/log(2)))
    
    cluster_size = [2**j for j in range(n_clusters)]
    cluster_sum = [0] * n_clusters
    
    clusters = []
    cluster_0 = Cluster(cluster_sum = 0, cluster_size = 1, n_digits = 1)
    cluster_0_first_node = Cluster(cluster_sum = 0, cluster_size = 1, n_digits = 1)
    cluster_0.add_sub_cluster(cluster_0_first_node)
    clusters.append(cluster_0)


    for k in range(1, n_clusters):
        for sub_cluster_idx in range(k):
            cluster_sum[k] += cluster_sum[sub_cluster_idx] + (k-sub_cluster_idx)*cluster_size[sub_cluster_idx]
        cluster_k = Cluster(cluster_sum[k], cluster_size[k], k+1)
        cluster_k_first_node = Cluster(cluster_sum = 0, cluster_size = 1, n_digits = k+1)
        cluster_k.add_sub_cluster(cluster_k_first_node)
        for sub_cluster_idx in range(k):
            cluster_k.add_sub_cluster(clusters[sub_cluster_idx])
        clusters.append(cluster_k)

    ans += sum(cluster_sum[:-1])

    n_remaining = n - sum(cluster_size[:-1])

    sub_sum, _ = clusters[-1].get_partial_cluster_sum([64], n_remaining+1, len(cluster_sum))
    ans += sub_sum

    return ans



if __name__ == "__main__":
    ans = solve(N)
    print(ans)
