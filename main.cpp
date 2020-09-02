#include <iostream>
#include "graph_clustering.h"
#include "global.h"
#include "data_structure.h"
#include "hashfile_to_map.h"

using namespace std;
//  assume we have graph blow
//
//   chunk 0 —— chunk 1  ——  chunk 2 —— chunk 3 
//    ||          ||          ||         ||      
//   chunk 4 —— chunk 5      chunk 6 —— chunk 7 
//    |                                   |
//   chunk 8 —— chunk 9      chunk 12 —— chunk 13 
//    ||         ||           ||         || 
//   chunk 10 —— chunk 11 —— chunk 14 —— chunk 15 
//
// assume the number "——" or "|" is the weight of edge
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
    unordered_map<int,cluster> clusters;

    if(false){
        node node0,node1,node2,node3,node4,node5,node6,node7;
        node node8,node9,node10,node11,node12,node13,node14,node15;
        node0.chunk_ID_="chunk0";    node1.chunk_ID_="chunk1";
        node2.chunk_ID_="chunk2";    node3.chunk_ID_="chunk3";
        node4.chunk_ID_="chunk4";    node5.chunk_ID_="chunk5";
        node6.chunk_ID_="chunk6";    node7.chunk_ID_="chunk7";
        node8.chunk_ID_="chunk8";    node9.chunk_ID_="chunk9";
        node10.chunk_ID_="chunk10";    node11.chunk_ID_="chunk11";
        node12.chunk_ID_="chunk12";    node13.chunk_ID_="chunk13";
        node14.chunk_ID_="chunk14";    node15.chunk_ID_="chunk15";

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
        node4.adjacent_nodes_.emplace("chunk8",1);

        node5.adjacent_nodes_.emplace("chunk1",2);
        node5.adjacent_nodes_.emplace("chunk4",1);

        node6.adjacent_nodes_.emplace("chunk2",2);
        node6.adjacent_nodes_.emplace("chunk7",1);

        node7.adjacent_nodes_.emplace("chunk3",2);
        node7.adjacent_nodes_.emplace("chunk6",1);
        node7.adjacent_nodes_.emplace("chunk13",1);

        node8.adjacent_nodes_.emplace("chunk4",1);
        node8.adjacent_nodes_.emplace("chunk9",1);
        node8.adjacent_nodes_.emplace("chunk10",2);

        node9.adjacent_nodes_.emplace("chunk8",1);
        node9.adjacent_nodes_.emplace("chunk11",2);

        node10.adjacent_nodes_.emplace("chunk8",2);
        node10.adjacent_nodes_.emplace("chunk11",1);

        node11.adjacent_nodes_.emplace("chunk9",2);
        node11.adjacent_nodes_.emplace("chunk10",1);

        node12.adjacent_nodes_.emplace("chunk13",1);
        node12.adjacent_nodes_.emplace("chunk14",2);

        node13.adjacent_nodes_.emplace("chunk7",1);
        node13.adjacent_nodes_.emplace("chunk12",1);
        node13.adjacent_nodes_.emplace("chunk15",2);

        node14.adjacent_nodes_.emplace("chunk11",1);
        node14.adjacent_nodes_.emplace("chunk12",2);
        node14.adjacent_nodes_.emplace("chunk15",1);

        node15.adjacent_nodes_.emplace("chunk14",1);
        node15.adjacent_nodes_.emplace("chunk13",2);

        
        graphClustering.graph_matrix.emplace(node0.chunk_ID_,node0);
        graphClustering.graph_matrix.emplace(node1.chunk_ID_,node1);
        graphClustering.graph_matrix.emplace(node2.chunk_ID_,node2);
        graphClustering.graph_matrix.emplace(node3.chunk_ID_,node3);
        graphClustering.graph_matrix.emplace(node4.chunk_ID_,node4);
        graphClustering.graph_matrix.emplace(node5.chunk_ID_,node5);
        graphClustering.graph_matrix.emplace(node6.chunk_ID_,node6);
        graphClustering.graph_matrix.emplace(node7.chunk_ID_,node7);
        graphClustering.graph_matrix.emplace(node8.chunk_ID_,node8);
        graphClustering.graph_matrix.emplace(node9.chunk_ID_,node9);
        graphClustering.graph_matrix.emplace(node10.chunk_ID_,node10);
        graphClustering.graph_matrix.emplace(node11.chunk_ID_,node11);
        graphClustering.graph_matrix.emplace(node12.chunk_ID_,node12);
        graphClustering.graph_matrix.emplace(node13.chunk_ID_,node13);
        graphClustering.graph_matrix.emplace(node14.chunk_ID_,node14);
        graphClustering.graph_matrix.emplace(node15.chunk_ID_,node15);

        cluster clst0,clst1,clst2,clst3,clst4,clst5,clst6,clst7;
        cluster clst8,clst9,clst10,clst11,clst12,clst13,clst14,clst15;
        clst0.cluster_ID_=0;
        clst1.cluster_ID_=1;
        clst2.cluster_ID_=2;
        clst3.cluster_ID_=3;
        clst4.cluster_ID_=4;
        clst5.cluster_ID_=5;
        clst6.cluster_ID_=6;
        clst7.cluster_ID_=7;
        clst8.cluster_ID_=8;
        clst9.cluster_ID_=9;
        clst10.cluster_ID_=10;
        clst11.cluster_ID_=11;
        clst12.cluster_ID_=12;
        clst13.cluster_ID_=13;
        clst14.cluster_ID_=14;
        clst15.cluster_ID_=15;
        // at beginning, each chunk is a cluster
        clst0.chunks_.emplace("chunk0",node0);
        clst1.chunks_.emplace("chunk1",node1);
        clst2.chunks_.emplace("chunk2",node2);
        clst3.chunks_.emplace("chunk3",node3);
        clst4.chunks_.emplace("chunk4",node4);
        clst5.chunks_.emplace("chunk5",node5);
        clst6.chunks_.emplace("chunk6",node6);
        clst7.chunks_.emplace("chunk7",node7);
        clst8.chunks_.emplace("chunk8",node8);
        clst9.chunks_.emplace("chunk9",node9);
        clst10.chunks_.emplace("chunk10",node10);
        clst11.chunks_.emplace("chunk11",node11);
        clst12.chunks_.emplace("chunk12",node12);
        clst13.chunks_.emplace("chunk13",node13);
        clst14.chunks_.emplace("chunk14",node14);
        clst15.chunks_.emplace("chunk15",node15);
        clusters.emplace(clst0.cluster_ID_,clst0);
        clusters.emplace(clst1.cluster_ID_,clst1);
        clusters.emplace(clst2.cluster_ID_,clst2);
        clusters.emplace(clst3.cluster_ID_,clst3);
        clusters.emplace(clst4.cluster_ID_,clst4);
        clusters.emplace(clst5.cluster_ID_,clst5);
        clusters.emplace(clst6.cluster_ID_,clst6);
        clusters.emplace(clst7.cluster_ID_,clst7);
        clusters.emplace(clst8.cluster_ID_,clst8);
        clusters.emplace(clst9.cluster_ID_,clst9);
        clusters.emplace(clst10.cluster_ID_,clst10);
        clusters.emplace(clst11.cluster_ID_,clst11);
        clusters.emplace(clst12.cluster_ID_,clst12);
        clusters.emplace(clst13.cluster_ID_,clst13);
        clusters.emplace(clst14.cluster_ID_,clst14);
        clusters.emplace(clst15.cluster_ID_,clst15);
    }


    load_fileref(g_file_filename_);
    //load_chunkref(g_chunk_filename_);
    //load_finish();

    int cluster_cnt = 0, file_cnt=0, chunk_cnt=0;
    node *n, *neighbor_n;

    for(pair<string, FileRef*> fp: all_fileref){
        // for every pair of chunks, create nodes and increase their weight
        for (std::unordered_map<string, ChunkRef*>::iterator i = fp.second->internal_chunk_map.begin(); i != fp.second->internal_chunk_map.end(); i++){
            // if chunk i not existed in graph_matrix, create a new node for this chunk
            unordered_map<string,node&>::iterator nit = graphClustering.graph_matrix.find(i->first);
            if (nit==graphClustering.graph_matrix.end()) {
                // first time see the chunk, create a new node for it
                n = new node(i->first);
                n->k = 0;
            } else {
                // saw this chunk, get its node
                n = &nit->second;
            }
            // pair chunk i with rest of the chunks, chunk j
            // add them into node->adjacent_nodes
            for (std::unordered_map<string, ChunkRef*>::iterator j = std::next(i); j != fp.second->internal_chunk_map.end(); j++){
                // if j is already in n->adjacent_nodes, then its weight++, else insert j into n->adjacent_nodes with weight=1
                unordered_map<string,int>::iterator neighbor_it = n->adjacent_nodes_.find(j->first);
                if(neighbor_it == n->adjacent_nodes_.end()) {
                    // j is not in i's neighbor list, add it!
                    n->adjacent_nodes_.emplace(j->first, 1);
                } else {
                    // found j in i's adjacent_nodes, weight++
                    neighbor_it->second++;
                }
                // add the neighbor's weight for this (n,j) pair
                // find neighbor node in graph_matrix, if not found, create one
                // if found, add neighbor node and weight
                unordered_map<string,node&>::iterator graph_it = graphClustering.graph_matrix.find(j->first);
                if(graph_it == graphClustering.graph_matrix.end()) {
                    neighbor_n = new node(j->first);
                    neighbor_n->k = 0;
                    neighbor_n->adjacent_nodes_.emplace(i->first,1);
                    graphClustering.graph_matrix.emplace(neighbor_n->chunk_ID_,*neighbor_n);
                }else{
                    // check if the j node in graph_matrix already has i in its adjacent_nodes
                    unordered_map<string,int>::iterator i_it = graph_it->second.adjacent_nodes_.find(i->first);
                    if(i_it == graph_it->second.adjacent_nodes_.end()){
                        graph_it->second.adjacent_nodes_.emplace(i->first,1);
                    }else{
                        i_it->second++;
                    }
                }
            }
            // make sure the above changes reflect to graph_matrix
            if (nit==graphClustering.graph_matrix.end()) {
                graphClustering.graph_matrix.emplace(n->chunk_ID_,*n);
            } //else {
                //nit->second = *n; // no need to re-assign
                //nit->second.k = 1; //set a tag
            //}
            chunk_cnt++;
        }
        file_cnt++;
        // Finished with this file, delete the FileRef
        delete fp.second;
    }
    cout<<"Processed "<<file_cnt<<" files into graph cluster"<<endl;

    // check graph_matrix is modified by scanning k
    // CONFIRMED the modification to graph_matrix are effective
    cluster *cl;

    for(auto nit : graphClustering.graph_matrix) {
        cl = new cluster(cluster_cnt);
        cl->chunks_.emplace(nit.first,nit.second);
        clusters.emplace(cl->cluster_ID_,*cl);
        cluster_cnt++;
    }
    cout<<"Created "<<cluster_cnt<<" clusters based on "<<file_cnt<<" files and "<<chunk_cnt<<" chunks"<<endl;
    // }

    // create clusters from graphClustering.graph_matrix

    
    // for(pair<string, ChunkRef*> cp: all_chunkref){
    //     node n(cp.first);
    //     for(pair<string, ChunkRef*> cp_neighbor: all_chunkref) {
    //         if (cp_neighbor.first == cp.first) 
    //             continue;
    //         int weight = 0;
    //         // check if files appears in one node's file list also appear in the second node's file list
    //         // if yes, add to the weight
    //         for(auto cp_f_it = cp.second->file_ref_map.begin(); cp_f_it != cp.second->file_ref_map.end(); cp_f_it++) {
    //             weight += cp_neighbor.second->file_ref_map.count(cp_f_it->first);
    //         }
    //         // if two nodes belongs to the same files, add them in the graph
    //         if(weight != 0) {
    //             n.adjacent_nodes_.emplace(cp_neighbor.first,weight);
    //         }
    //     }
    //     graphClustering.graph_matrix.emplace(n.chunk_ID_,n);
    //     cluster cl(cluster_cnt);
    //     cl.chunks_.emplace(n.chunk_ID_,n);
    //     clusters.emplace(cl.cluster_ID_,cl);
    //     cluster_cnt++;
    // }

    graphClustering.init(clusters); // calculate k for each node
    cout <<"Graph clustering initiated"<<endl;
    graphClustering.clustering_chunk_based(clusters);
    cout <<"Graph clustering partitioned"<<endl;
    graphClustering.save_clusters(clusters);
    cout <<"Graph clustering saved to "<<graphClustering.global_filename_<<endl;
    
    // clear clusters to test load_clusters from file global_filename_
    clusters.clear();
    
    graphClustering.load_clusters(clusters);
    //graphClustering.clustering_file_based(clusters);
    D(cout<<"Loaded cluster:"<<endl);
    if(DEBUG) graphClustering.print_clusters(clusters);
    //clusters are ready from this point
    return 0;
}