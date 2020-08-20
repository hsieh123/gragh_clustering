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

using namespace std;

// node == chunk
class node{
public:
    string chunk_ID_;
    list<pair<string,int>> adjacent_nodes_;
};

// at beginning, one cluster is one node (chunk). after assignment, one cluster is a set of chunks, those chunks will be stored in one primer
class cluster{
public:
    int cluster_ID_;
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
    unordered_map<string,node&> graph_matrix;
    void calculate_modularity(vector<cluster>&);
    void calculate_delta_modularity(vector<cluster>&);
    void clustering_file_based(vector<cluster>&);
    void clustering_chunk_based(vector<cluster>&);
};


#endif //GRAGH_CLUSTERING_GRAPH_CLUSTERING_H
