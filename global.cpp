//
// Created by wyx on 19-6-20.
//

#include "global.h"

long g_chunk_size=0;
long g_container_size=0;
string g_allfile_path = "allfile.dat";
string g_allchunk_path = "allchunk.dat";
//unsigned short g_primer_capacity=736;
unsigned short g_k_ratio=3;
string g_dedup_trace_dir;
string g_trace_summary_file;
string g_recipe_file;
string g_dedup_engine;
string g_selection_policy="level";
long g_file_number = 0;

int Parse(string cfgfile){
    ifstream filestream(cfgfile, ios_base::in);
    if (filestream.fail()) {
        cerr << "open cfgfile:" << cfgfile << " fails!\n";
        return -1;
    }
    string line;

    while(getline(filestream, line)) {
        if (line.size()<=1 || line[0]== '#')
            continue;

        stringstream ss(line);
        string key, value;
        getline(ss, key, ' ');
        getline(ss, value, ' ');

        switch(hash_(key.c_str())){
            case hash_("dedup_trace_dir"):
                g_dedup_trace_dir = value;
                break;
            case hash_("trace_summary_file"):
                g_trace_summary_file = value;
                break;
            case hash_("file_number"):
                g_file_number = stol(value);
                break;
            default:
                cout<<"unknown cfg: "<<key<<endl;
                return -1;
        }
    }
    return 0;
}
