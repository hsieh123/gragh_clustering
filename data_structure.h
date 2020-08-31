#ifndef DNA_DEDUPE_DATA_STRUCTURE_H
#define DNA_DEDUPE_DATA_STRUCTURE_H

#include "global.h"

struct ChunkRef;

struct FileRef {
    int chunk_num;

    int ref_num_sum; // summation of all chunks' ref_num

    unordered_map<string, ChunkRef*> internal_chunk_map; // <chunk ID, chunkRef>
};

struct ChunkRef {
    //int ref_num_; // if one file contain this chunk twice, only count once

    vector<int> primer_ids;// final result, after assignment, each chunk will be stored in several primers (depends on whether replicate)

    unordered_map<string, FileRef*> file_ref_map; // <file name , fileRef>
};

unordered_map<string, FileRef*> all_fileref;      //<file_path, fileRef>
unordered_map<string, ChunkRef*> all_chunkref;    //<chunk_ID, chunkRef>

#endif //DNA_DEDUPE_DATA_STRUCTURE_H