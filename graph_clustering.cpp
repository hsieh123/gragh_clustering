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

void graph_clustering::calculate_delta_modularity(vector<cluster> &) {

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
void graph_clustering::clustering_chunk_based(vector<cluster> &) {

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