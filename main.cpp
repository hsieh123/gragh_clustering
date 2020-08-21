#include <iostream>
#include "graph_clustering.h"
using namespace std;
//  assume we have graph blow
//
//   chunk 0 —— chunk 1 —— chunk 2 —— chunk 3
//    ||          ||        ||        ||
//   chunk 4 —— chunk 5    chunk 6 —— chunk 7
//
//  assume the number "——" or "|" is the weight of edge
//  thus, chunk pair {1,5} {2,6} {3,7} {4,8} have edge with weight 2
//

/*
 * input: a weight indirect graph
 * output: clusters of the vertices (chunks)
 * edge weight: the number of common files between two chunks
 * main idea: chunks who are always contained by same files have a higher correlation
 *            for those highly correlated chunks, they will be in same cluster and store in same primer (one primer one cluster).
 *            In that case, to restore any file, its required chunks will highly possible in same primer.
 */

/*
we have two kinds of clustering in this program. each of them corresponding to a specific assignment algorithm
 1) clustering_chunk_based   (used in file based assignment)
 2) clustering_file_based    (used in chunk based assigment)
 both clustering should obey the primer capacity constraint (i.e., no cluster's size would exceed primer capacity)
 while clustering_file_based should consider more constraints, details in definition of the function
*/




int main() {
    // test for clustering_chunk_based
    //inital data set for clustering_chunk_based
    graph_clustering graphClustering;

        node node0,node1,node2,node3,node4,node5,node6,node7;
        node0.chunk_ID_="chunk0";    node1.chunk_ID_="chunk1";
        node2.chunk_ID_="chunk2";    node3.chunk_ID_="chunk3";
        node4.chunk_ID_="chunk4";    node5.chunk_ID_="chunk5";
        node6.chunk_ID_="chunk6";    node7.chunk_ID_="chunk7";

        node0.adjacent_nodes_.emplace("chunk1",1);
        node0.adjacent_nodes_.emplace("chunk4",2);

        node1.adjacent_nodes_.emplace("chunk0",1);
        node1.adjacent_nodes_.emplace("chunk2",1);
        node1.adjacent_nodes_.emplace("chunk5",2);

        node2.adjacent_nodes_.emplace("chunk1",1);
        node2.adjacent_nodes_.emplace("chunk3",1);
        node2.adjacent_nodes_.emplace("chunk6",2);

        node3.adjacent_nodes_.emplace("chunk2",1);
        node3.adjacent_nodes_.emplace("chunk7",2);

        node4.adjacent_nodes_.emplace("chunk0",2);
        node4.adjacent_nodes_.emplace("chunk5",1);

        node5.adjacent_nodes_.emplace("chunk1",2);
        node5.adjacent_nodes_.emplace("chunk4",1);

        node6.adjacent_nodes_.emplace("chunk2",2);
        node6.adjacent_nodes_.emplace("chunk7",1);

        node7.adjacent_nodes_.emplace("chunk3",2);
        node7.adjacent_nodes_.emplace("chunk6",1);

        graphClustering.graph_matrix.emplace(node0.chunk_ID_,node0);
        graphClustering.graph_matrix.emplace(node1.chunk_ID_,node1);
        graphClustering.graph_matrix.emplace(node2.chunk_ID_,node2);
        graphClustering.graph_matrix.emplace(node3.chunk_ID_,node3);
        graphClustering.graph_matrix.emplace(node4.chunk_ID_,node4);
        graphClustering.graph_matrix.emplace(node5.chunk_ID_,node5);
        graphClustering.graph_matrix.emplace(node6.chunk_ID_,node6);
        graphClustering.graph_matrix.emplace(node7.chunk_ID_,node7);



    unordered_map<int,cluster> clusters;

        cluster clst0,clst1,clst2,clst3,clst4,clst5,clst6,clst7;
        clst0.cluster_ID_=0;
        clst1.cluster_ID_=1;
        clst2.cluster_ID_=2;
        clst3.cluster_ID_=3;
        clst4.cluster_ID_=4;
        clst5.cluster_ID_=5;
        clst6.cluster_ID_=6;
        clst7.cluster_ID_=7;
        // at beginning, each chunk is a cluster
        clst0.chunks_.emplace("chunk0",node0);
        clst1.chunks_.emplace("chunk1",node1);
        clst2.chunks_.emplace("chunk2",node2);
        clst3.chunks_.emplace("chunk3",node3);
        clst4.chunks_.emplace("chunk4",node4);
        clst5.chunks_.emplace("chunk5",node5);
        clst6.chunks_.emplace("chunk6",node6);
        clst7.chunks_.emplace("chunk7",node7);
        clusters.emplace(clst0.cluster_ID_,clst0);
        clusters.emplace(clst1.cluster_ID_,clst1);
        clusters.emplace(clst2.cluster_ID_,clst2);
        clusters.emplace(clst3.cluster_ID_,clst3);
        clusters.emplace(clst4.cluster_ID_,clst4);
        clusters.emplace(clst5.cluster_ID_,clst5);
        clusters.emplace(clst6.cluster_ID_,clst6);
        clusters.emplace(clst7.cluster_ID_,clst7);


    graphClustering.clustering_chunk_based(clusters);
    //graphClustering.clustering_file_based(clusters);

    return 0;
}