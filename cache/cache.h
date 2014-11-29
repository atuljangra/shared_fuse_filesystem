

#include <stdio.h>
#include <unordered_map>

#define MAXBOCKSIZE 400
#define MAXCACHESIZE 8
#define std::unordered_map<int,cache_block*>::const_iterator cache_it

class cache {

    std::unordered_map<int, cache_block*> blocks;
    int last_time = 0;

    public:
        char *get_block(int);
        void update_block(int, char*);
        void add_block(cache_block *);
        void invalidate(int);

}

class cache_block {

    char *data;
    bool isValid;
    int chunk_id;
    const char *path;

    public:
        int time;
        cache_block(int id, char *data) : cache_block(id, data, NULL);
        cache_block(int, char*, char*);
        char *get_data();
        void update_data(char* );
        void invalidate();
        void validate();

}
