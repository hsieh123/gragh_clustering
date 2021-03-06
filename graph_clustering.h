//
// Created by Wei00161@umn.edu on 2020/8/19.
//

#ifndef GRAGH_CLUSTERING_GRAPH_CLUSTERING_H
#define GRAGH_CLUSTERING_GRAPH_CLUSTERING_H

#include <list>
#include <vector>
#include <string>
#include <unordered_map>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include "global.h"

using namespace std;

#ifndef DEBUG 
#define DEBUG 0 // set debug mode
#endif
#ifdef DEBUG 
#define D(x) (x)
#else 
#define D(x) do{}while(0)
#endif

// node == chunk
class node{
public:
    node(string id) {chunk_ID_ = id;}
    node(){}
    string chunk_ID_;
    int32_t k;
    unordered_map<string,int> adjacent_nodes_;
};

// at beginning, one cluster is one node (chunk). after assignment, one cluster is a set of chunks, those chunks will be stored in one primer
class cluster{
public:
    cluster(int id) {cluster_ID_ = id;}
    cluster(){}
    int cluster_ID_;
    int32_t k; //constructed during merge
    unordered_map<string,node&> chunks_;
};

class graph_clustering {
public:
/*
    it's the Adjacency List form of the graph

    node 1:
        node 2 (weight)
        node 3 (weight)
        node 4 (weight)
    node 2:
        node 1 (weight)
        node 3 (weight)
        node 4 (weight)
    ...:
        ...
        ...
*/

// in this matrix every edge will be stored twice, when go over all edges please keep it in mind!!
// e.g, the modularity requires you to calculate the summation of all edges' weight, if you add all weight in the matrix please divide 2
// i.e. see graph_clustering::init
    unordered_map<string,node&> graph_matrix;
    float global_modularity_;
    float global_total_sum_ = 0;
    float max_delta_modularity_;
    string global_filename_ = "clusters.dat";
    
    void init(unordered_map<int, cluster>&);
    std::tuple<int,int> calculate_modularity(unordered_map<int, cluster>&);
    void merge_clusters(unordered_map<int, cluster>&, std::tuple<int,int>);
    void clustering_file_based(unordered_map<int, cluster>&, int);
    void clustering_chunk_based(unordered_map<int, cluster>&);
    void print_clusters(unordered_map<int, cluster>&);
    void save_clusters(unordered_map<int, cluster>&);
    void load_clusters(unordered_map<int, cluster>&);

private:
    //set<pair<node&,node&>>epsilon; //records nodes in the same cluster
    int32_t k(node&);
    int32_t k(cluster&);
    int32_t A(node&, node&);
    int32_t A(cluster&, node&);
    int32_t A(cluster&, cluster&);
};


#endif //GRAGH_CLUSTERING_GRAPH_CLUSTERING_H
