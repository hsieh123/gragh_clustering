//
// Created by Wei00161@umn.edu on 2020/8/19.
//

#include "graph_clustering.h"


// Setup k for each node
void graph_clustering::init(unordered_map<int,cluster> & clusters){
    for (std::unordered_map<int,cluster>::iterator it = clusters.begin(); it != clusters.end(); it++){
        for(std::unordered_map<string,node&>::iterator cit = it->second.chunks_.begin(); cit != it->second.chunks_.end(); cit++){
            D(cout << "Init <c,n>=<"<<it->first<<","<<cit->first<<">  ");
            cit->second.k = k(cit->second);
            D(cout<<"k="<<cit->second.k<<endl);
            global_total_sum_ += cit->second.k;
        }
        it->second.k = k(it->second);
    }
    global_total_sum_ = global_total_sum_ / 2;
}

// at beginning, graph should run this function to know its initial modularity
std::tuple<int,int> graph_clustering::calculate_modularity(unordered_map<int,cluster> & clusters) {
    
    int cluster_to_merge1=-1, cluster_to_merge2=-1;
    global_modularity_ = 0;
    for (std::unordered_map<int,cluster>::iterator i = clusters.begin(); i != clusters.end(); i++){
        for (std::unordered_map<int,cluster>::iterator j = std::next(i); j != clusters.end(); j++){

            // Check if cluster i and cluster j merger will exceed primer capacity limit
            if ((i->second.chunks_.size()+j->second.chunks_.size())>PRIMER_SIZE) {
                D(cout<<"Cluster size combined ("<<i->second.chunks_.size()+j->second.chunks_.size()<<") exceeded maximum PRIMER_SIZE("<<PRIMER_SIZE<<"), try next"<<endl);
                continue;
            }
            
            // Calculate modularity if cluster i and cluster j merge
            float Q = (A(i->second, j->second)-(i->second.k*j->second.k/global_total_sum_))/global_total_sum_;
            if(global_modularity_ < Q) {
                global_modularity_ = Q;
                cluster_to_merge1 = i->first;
                cluster_to_merge2 = j->first;
                D(cout<<"Cluster ("<<i->first << ","<<j->first<<")"<<endl);
                D(cout<<"Update Q="<<(A(i->second, j->second)-(i->second.k*j->second.k/global_total_sum_))/global_total_sum_<<endl);
            }
        }
    }
    return std::make_tuple(cluster_to_merge1,cluster_to_merge2);
}

void graph_clustering::merge_clusters(unordered_map<int, cluster>& clusters, std::tuple<int,int> vertices) {
    std::unordered_map<int,cluster>::iterator v1_it = clusters.find(get<0>(vertices));
    std::unordered_map<int,cluster>::iterator v2_it = clusters.find(get<1>(vertices));
    if(v1_it ==clusters.end() || v2_it ==clusters.end())
        return;

    // Insert every chunks(nodes) in cluster 2 into cluster 1
    for(std::unordered_map<string,node&>::iterator cit = v2_it->second.chunks_.begin(); cit != v2_it->second.chunks_.end(); cit++){
        v1_it->second.chunks_.emplace(cit->first,cit->second);
    }
    // Delete cluster 2 from clusters
    clusters.erase(v2_it);

    // Update cluster k
    v1_it->second.k = k(v1_it->second);
}

void graph_clustering::print_clusters(unordered_map<int,cluster> & clusters){
    for (auto const& cluster : clusters) {
        cout<<"Cluster "<<cluster.first<<endl;
        for (auto const& node: cluster.second.chunks_){
            cout<<"  "<<node.first<<endl;
        }
    }
}

/*when doing the hierarchical clustering, it does clustering based on the delta_modularity in a greedy way
it only merge two clusters who can increase most modularity
according the definition of modularity, to calculate the delta_modularity, you don't have to go over all node pairs
you only have to recalculate the nodes in the clusters that you try to merge
We have to do this step to decrease our computing complexity*/

void graph_clustering::calculate_delta_modularity(unordered_map<int,cluster> & clusters) {
    D(cout << "k0: " << k(clusters[0].chunks_.begin()->second));
    D(cout << "  << should be 3"<<endl);
    D(cout << "A04: " << A(clusters[0].chunks_.begin()->second,clusters[4].chunks_.begin()->second));
    D(cout << "  << should be 2" <<endl);
    D(cout << "A74: " << A(clusters[0].chunks_.begin()->second,clusters[7].chunks_.begin()->second));
    D(cout << "  << should be 0"<<endl);
    D(cout << "A12(cluster,node): " << A(clusters[1],clusters[2].chunks_.begin()->second));
    D(cout << "  << should be 1"<<endl);
    D(cout << "A15(clusters): "<<A(clusters[1],clusters[5]));
    D(cout << "  << should be 2" <<endl);
    D(cout << "A07(clusters): "<<A(clusters[0],clusters[7]));
    D(cout << "  << should be 0" <<endl);
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
void graph_clustering::clustering_chunk_based(unordered_map<int,cluster> & clusters) {
    calculate_delta_modularity(clusters);
    while (1){
        print_clusters(clusters);
        std::tuple<int,int> max_mod_clusters = calculate_modularity(clusters);
        if(get<0>(max_mod_clusters)==-1) {
            //No modularity increase
            break;
        }
        D(cout<<"Clusters("<<get<0>(max_mod_clusters)<<","<<get<1>(max_mod_clusters)<<") Modularity="<<global_modularity_<<endl);
        merge_clusters(clusters,max_mod_clusters);
    }
    D(cout<<"After clustering_chunk_based():"<<endl);
    print_clusters(clusters);
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
void graph_clustering::clustering_file_based(unordered_map<int,cluster> &) {

}


int32_t graph_clustering::k(node& n) {
    int32_t weight_sum=0;
    for (auto const& adj_chunk : n.adjacent_nodes_)
        weight_sum += adj_chunk.second;
    return weight_sum;
}

int32_t graph_clustering::k(cluster& c) {
    // Check for each nodes in a cluster, check if its adjacent nodes existed in the cluster
    // If not existed, add weight to weight_sum
    int32_t weight_sum=0;
    // for (std::unordered_map<string,node&>::iterator it= c.chunks_.begin(); it!=c.chunks_.end(); it++) {
    //     for(std::unordered_map<string,int>::iterator nit = it->second.adjacent_nodes_.begin(); nit != it->second.adjacent_nodes_.end(); nit++) {
    //         if (c.chunks_.find(nit->first) == c.chunks_.end()) {
    //             // The current adjacent node doesn't appear in the cluster
    //             // Add the weight from weight_sum
    //             weight_sum += nit->second;
    //         }
    //     }
    // }
    for (auto const& chunk: c.chunks_){
        for (auto const& adj_chunk: chunk.second.adjacent_nodes_) {
            if(c.chunks_.find(adj_chunk.first) == c.chunks_.end()){
                // The current adjacent node doesn't appear in the cluster
                // Add the weight from weight_sum
                weight_sum += adj_chunk.second;
            }
        }
    }
    return weight_sum;
}

int32_t graph_clustering::A(node& n1, node& n2) {
    unordered_map<string,int>::iterator it = n1.adjacent_nodes_.find(n2.chunk_ID_);
    return (it != n1.adjacent_nodes_.end())? it->second : 0;
}

int32_t graph_clustering::A(cluster& c, node& n) {
    int32_t weight_sum=0;
    for (auto const& chunk : c.chunks_)
        weight_sum += A(chunk.second, n);
    return weight_sum;
}

// Computational intensive since we need to match every chunk in every cluster. Better way?
int32_t graph_clustering::A(cluster& c1, cluster& c2) {
    int32_t weight_sum=0;
    for (auto const& chunk : c2.chunks_)
        weight_sum += A(c1,chunk.second);
    return weight_sum;
}

void graph_clustering::save_clusters(unordered_map<int, cluster>& clusters) {
    ofstream of;
	of.open(global_filename_);
	if(!of.is_open()){
		cerr<<"Error in writeing all FileRef to "<< global_filename_<<endl;
		return;
	}
	for(auto const & cluster : clusters) {
		// print cluster
		of << cluster.first <<endl;
		// print number of chunks in this cluster
		of << cluster.second.chunks_.size()<<endl;
		for (auto const& chunk : cluster.second.chunks_) {
		    // print chunkID
            of << chunk.first<<" ";
            // adjacent_node weight adjacent_node weight ...
            for (auto const& adj_node: chunk.second.adjacent_nodes_) {
                of << adj_node.first <<" "<<adj_node.second<<" ";
            }
		    of << endl;
        }
	}
	of.close();
}

void graph_clustering::load_clusters(unordered_map<int, cluster>& clusters) {
    ifstream fs(global_filename_, ios_base::in);
    if (fs.fail()) {
        cerr << "open clusters file:" << global_filename_ << " fails!\n";
		return;
    }
    string cluster_id, chunk_id, chunk_adj_nodes, adj_node, chunk_num;
    int adj_weight;
    stringstream ss;
    cluster *cl;
    node *n;
    while(getline(fs, cluster_id)) {
        if (cluster_id.size()<1 || cluster_id[0]== '#')
            continue;
        cl = new cluster(stoi(cluster_id));
        // read number of chunks in this cluster
        getline(fs, chunk_num);
        for (int i=0; i<stoi(chunk_num); i++){
            // get each chunk and its adjacent nodes
            getline(fs,chunk_adj_nodes);
            ss.clear();
            ss.str(chunk_adj_nodes);
		    // read adjacent_node weight adjacent_node weight ...
		    ss>>chunk_id;
            n = new node(chunk_id);
            while(1) {
                // get each adjacent node id and weight
                ss>>adj_node>>adj_weight;
                n->adjacent_nodes_.emplace(adj_node,adj_weight);
                if (ss.fail())
                    break;
            }
            n->k = k(*n);
            cl->chunks_.emplace(chunk_id, *n);
        }
        cl->k = k(*cl);
        clusters.emplace(cl->cluster_ID_,*cl);
    }
}