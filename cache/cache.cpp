

#include <cache.h>

cache_block::cache_block(int chunkid, char *data, char *path) {

    this->isValid = true;
    this->chunkid = chunkid;
    this->path = (char *) malloc(MAXPATHSIZE);
    strncpy(this->path, path, MAXPATHSIZE);
    this->data = (char *) malloc(MAXBLOCKSIZE);
    strncpy(this->data, data, MAXBLOCKSIZE);
    time = 0;

}

char *cache_block::get_data() {

    if(isValid)
        return data;
    else
        return NULL;

}

void *cache_block::update_data(char *data) {

   strncpy(this->data, data, MAXBLOCKSIZE); 
   validate();

}

void cache_block::invalidate() {

    isValid = false;

}

void cache_block::validate() {

    isValid = true;

}

cache_block *cache::get_block(int chunkid) {

    cache_it it = blocks.find(chunkid);
    if(it) {
    
        return it->second;
    
    }
    else
        return NULL;

}

void *cache::update_block(int chunkid, char *data) {

    if(blocks.count(chunkid) > 0) {

        blocks[chunkid].update_data(data); 

    }

}

void *cache::add_block(int chunkid, char *data) {

    cache_block *block = new cache_block(chunkid, data);
    block->time = last_time++;
    if(blocks.size == MAXCACHESIZE) {

        int to_remove = -1;
        int time = INT_MAX;
        for(auto &x : blocks) {

            if(x.insert_time < time) {

                time = x.insert_time;
                to_remove = x.chunk_id;

            }

        }
        blocks.erase(to_remove);

    }
    blocks[block->chunk_id] = block;

}
void cache::invalidate(int chunkid) {

    if(blocks.count(chunkid) > 0)
        blocks.erase(chunkid);

}
