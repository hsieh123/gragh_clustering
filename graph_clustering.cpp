//
// Created by Wei00161@umn.edu on 2020/8/19.
//

#include "graph_clustering.h"
// at beginning, graph should run this function to know its initial modularity
void graph_clustering::calculate_modularity(vector<cluster> &) {

}

/*when doing the hierarchical clustering, it does clustering based on the delta_modularity in a greedy way
it only merge two clusters who can increase most modularity
according the definition of modularity, to calculate the delta_modularity, you don't have to go over all node pairs
you only have to recalculate the nodes in the clusters that you try to merge
We have to do this step to decrease our computing complexity*/

void graph_clustering::calculate_delta_modularity(vector<cluster> & clusters) {
    cout << "k0: " << k(clusters[0].chunks_.begin()->second);
    cout << "  << should be 3"<<endl;
    cout << "A04: " << A(clusters[0].chunks_.begin()->second,clusters[4].chunks_.begin()->second);
    cout << "  << should be 2" <<endl;
    cout << "A74: " << A(clusters[0].chunks_.begin()->second,clusters[7].chunks_.begin()->second);
    cout << "  << should be 0"<<endl;
    cout << "A12(cluster,node): " << A(clusters[1],clusters[2].chunks_.begin()->second);
    cout << "  << should be 1"<<endl;
    cout << "A15(clusters): "<<A(clusters[1],clusters[5]);
    cout << "  << should be 2" <<endl;
    cout << "A07(clusters): "<<A(clusters[0],clusters[7]);
    cout << "  << should be 0" <<endl;
}





/*first complete original algorithm, then add some "if" statement for following corner cases
 * 1) cluster size should lower than primer capacity
 * 2) if go over all cluster pairs and the modularity cannot increase anymore, stop
*/
/*
after chunk_based clustering, please write the result to file so that we can reuse the clustering result.
 The input should be all data chunks in our data set (all expansion)
 The output should be primers (just clusters) and chunks in the primer
*/
void graph_clustering::clustering_chunk_based(vector<cluster> & clusters) {
    calculate_delta_modularity(clusters);
}


/*
please first accomplish chunk_based.
based on the code of chunk_based clustering, this file_based should add some more constraints
please talk with me before you start.
*/

/*
after file_based clustering, don't have to write to file. this function will be integrated into zhichao's code.
 remain the result in data structure: vector<cluster> clusters
*/
void graph_clustering::clustering_file_based(vector<cluster> &) {

}


int32_t graph_clustering::k(node& n) {
    int32_t weight_sum=0;
    for (std::unordered_map<string,int>::iterator it= n.adjacent_nodes_.begin(); it!=n.adjacent_nodes_.end(); it++) {
        weight_sum += it->second;
    }
    return weight_sum;
}

int32_t graph_clustering::A(node& n1, node& n2) {
    unordered_map<string,int>::iterator it = n1.adjacent_nodes_.find(n2.chunk_ID_);
    return (it != n1.adjacent_nodes_.end())? it->second : 0;
}

int32_t graph_clustering::A(cluster& c, node& n) {
    int32_t weight_sum=0;
    for (std::unordered_map<string,node&>::iterator it = c.chunks_.begin(); it != c.chunks_.end(); it++)
        weight_sum += A(it->second, n);
    return weight_sum;
}

// Computational intensive since we need to match every chunk in every cluster. Better way?
int32_t graph_clustering::A(cluster& c1, cluster& c2) {
    int32_t weight_sum=0;
    for (std::unordered_map<string,node&>::iterator it = c2.chunks_.begin(); it != c2.chunks_.end(); it++)
        weight_sum += A(c1,it->second);
    return weight_sum;
}