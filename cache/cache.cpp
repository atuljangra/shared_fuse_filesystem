

#include <cache.h>

cache_block::cache_block(int chunkid, char *data, char *path) {

    this->isValid = true;
    this->chunkid = chunkid;
    this->path = (char *) malloc(MAXPATHSIZE);
    strncpy(this->path, path, MAXPATHSIZE);
    this->data = (char *) malloc(MAXBLOCKSIZE);
    strncpy(this->data, data, MAXBLOCKSIZE);

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

void cache::invalidate(int chunkid) {

    if(blocks.count(chunkid) > 0) {

        blocks[chunkid].invalidate(); 

    }

}
