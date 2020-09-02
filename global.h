//
// Created by wyx on 19-6-20.
//

#ifndef CONFIGURABLE_DEDUP_GLOBAL_H
#define CONFIGURABLE_DEDUP_GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <ctime>
#include <cstring>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>
#include <set>
#include <sstream>
#include <time.h>
#include <deque>
#include <algorithm>
#include <fstream>
#include <cassert>
#include <bitset>
#include <utility>
#include <math.h>
#include <inttypes.h>
using namespace std;

extern long g_chunk_size;
extern long g_container_size;
extern string g_dedup_trace_dir;
extern string g_trace_summary_file;
extern long g_file_number;
int Parse(string cfgfile);

typedef std::uint64_t hash_t;
constexpr hash_t prime = 0x100000001B3ull;
constexpr hash_t basis = 0xCBF29CE484222325ull;
constexpr hash_t hash_(char const* str, hash_t last_value = basis)
{
    return *str ? hash_(str+1, (*str ^ last_value) * prime) : last_value;
}

string g_allfile_path = "allfile.dat";
string g_allchunk_path = "allchunk.dat";
string g_file_filename_ = "allfile_64k_full.dat";
string g_chunk_filename_ = "allchunk_64k_full.dat";

#endif //CONFIGURABLE_DEDUP_GLOBAL_H