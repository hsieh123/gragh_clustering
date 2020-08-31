#ifndef _HASHFILE_TO_MAP_H
#define _HASHFILE_TO_MAP_H

#include "data_structure.h"

/*
 * Function:  save_allfileref
 * --------------------
 * Save all_fileref data structure at dna_dedupe_data_structure.h to g_allfile_path.
 * Use load_allfileref to load the data structure
 *  filename: file name of the output that stores all_fileref data structure
 *
 *  returns: int `1` if success, `0` if fail
 */
int save_allfileref(string filename) {
	ofstream of;
	of.open(filename);
	if(!of.is_open()){
		cout<<"Error in writeing all FileRef to "<< filename<<endl;
		return -1;
	}
	for(pair<string, FileRef*> fp: all_fileref) {
		// print file ID
		of <<fp.first <<endl;
		// print chunk_num ref_num_sum
		of <<fp.second->chunk_num <<" "<<fp.second->ref_num_sum<<endl;
		// print chunkID chunkID chunkID...
		for (pair<string, ChunkRef*> cp : fp.second->internal_chunk_map) {
			of << cp.first<<" ";
		}
		of <<endl<<endl;
	}
	of.close();
	return 0;
}
int save_allchunkref(string filename) {
	ofstream of;
	of.open(filename);
	if(!of.is_open()){
		cout<<"Error in writeing all ChunkRef to "<< filename<<endl;
		return -1;
	}
	for(pair<string, ChunkRef*> cp: all_chunkref) {
		// print chunk ID
		of <<cp.first <<endl;
		// print ref_num_
		//of <<cp.second->ref_num_ <<endl;
		// print fileID fileID fileID...
		for (pair<string, FileRef*> fp : cp.second->file_ref_map) {
			of << fp.first<<" ";
		}
		of <<endl<<endl;
	}
	of.close();
	return 0;
}

int load_fileref(string struct_file){
    ifstream fs(struct_file, ios_base::in);
    if (fs.fail()) {
        cerr << "open structure file:" << struct_file << " fails!\n";
		return -1;
    }
    string file_hash, chunk_hash, chunk_hash_line;
	stringstream ss;
	// read fileID
    while(getline(fs, file_hash)) {
        if (file_hash.size()<=1 || file_hash[0]== '#')
            continue;

		FileRef *fileref = new FileRef;
		// read chunk_num ref_num_sum
		fs>>fileref->chunk_num>>fileref->ref_num_sum;
		// read linebreak
		getline(fs,chunk_hash_line);
		// read chunkID chunkID chunkID...
		getline(fs,chunk_hash_line);
		ss.clear();
        ss.str(chunk_hash_line);
        while(1) {
			ss>>chunk_hash;
			fileref->internal_chunk_map.insert({chunk_hash,NULL});
			if (ss.fail())
				break;
		}
		all_fileref.insert({file_hash,fileref});
    }
	fs.close();
	return 0;
}

int load_chunkref(string struct_file){
    ifstream fs(struct_file, ios_base::in);
    if (fs.fail()) {
        cerr << "open structure file:" << struct_file << " fails!\n";
		return -1;
    }
    string chunk_hash, file_hash, file_hash_all;
	stringstream ss;
	// read fileID
    while(getline(fs, chunk_hash)) {
        if (chunk_hash.size()<=1 || chunk_hash[0]== '#')
            continue;

		ChunkRef *chunkref = new ChunkRef;
		// read ref_num_
		//fs >> chunkref->ref_num_;
		// read linebreak
		getline(fs,file_hash_all);
		// read chunkID chunkID chunkID...
		getline(fs,file_hash_all);
		ss.clear();
        ss.str(file_hash_all);
        while(1) {
			ss>>file_hash;
			chunkref->file_ref_map.insert({file_hash, NULL});
			if (ss.fail())
				break;
		}
		all_chunkref.insert({chunk_hash, chunkref});
    }
	return 0;
}

/*
 * Function:  load_finish
 * --------------------
 * Link unordered_map in FileRef and ChunkRef to each other, 
 * !!Should be most time consuming!!
 *
 *  returns: int `1` if success, `0` if fail
 */

int load_finish() {
	// all_chunkref iterator
	unordered_map<string, ChunkRef*>::const_iterator cit;
	// fileref->internal_chunk_map iterator
	unordered_map<string, ChunkRef*>::iterator fcit;
	// Find ChunkRef* for each chunk in each file
	for(pair<string, FileRef*> fp: all_fileref){
		fcit = fp.second->internal_chunk_map.begin();
		while(fcit != fp.second->internal_chunk_map.end()){
			cit = all_chunkref.find(fcit->first);
			if(cit == all_chunkref.end()){
				cerr << "ERROR! ChunkRef* not found in all_chunkref. This cannot be right. Check the code."<<endl;
				return -1;
			}
			// Set ChunkRef* in File internal_chunk_map as the ChunkRef* found in all_chunkref
			// This is why object <string, ChunkRef> won't work, but pointer <string, ChunkRef*> works
			// Otherwise I need to create two objects and set them well at the same time.
			fcit->second = cit->second;
			
			// Add FileRef to ChunkRef file_ref_map in all_chunkRef along the way
			cit->second->file_ref_map.insert(fp);
			fcit++;
		}
	}
	return 0;
}

#endif /*_HASHFILE_TO_MAP_H*/