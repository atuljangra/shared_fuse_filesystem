#include <sstream>
#include <string.h>
#include "Message.h"

using namespace std;
#define STRUCT_SIZE (3 * sizeof(int)) + _size

const char *Message::serialize() {
    char * buffer = (char *) malloc(STRUCT_SIZE);
    memcpy(buffer, &_code, sizeof(int));
    memcpy(buffer + sizeof(int), &_ret, sizeof(int));
    memcpy(buffer + 2 * sizeof(int), &_size, sizeof(int));
    memcpy(buffer + 3 *sizeof(int), _buffer, _size);
    return buffer;
}

void Message::create_getAttr(const char *path) {
    memset(_buffer, 0, _size);
    _size = sizeof(path);
    _ret = -1;
    memcpy(_buffer, path, _size);
    _code = GETATTR;
}

void Message::create_statfs(const char *path) {
    memset(_buffer, 0, _size);
    _size = sizeof(path);
    _ret = -1;
    memcpy(_buffer, path, _size);
    _code = STATFS; 
}

void Message::create_opendir(const char *path) {
    memset(_buffer, 0, _size);
    _size = sizeof(path);
    _ret  = -1;
    memcpy(_buffer, path, _size);
    _code = OPENDIR;
}

void Message::create_readdir(const char *path) {
    memset(_buffer, 0, _size);
    _size = sizeof(path);
    _ret  = -1;
    memcpy(_buffer, path, _size);
    _code = READDIR;
}

void Message::create_closedir(const char *path) {
    memset(_buffer, 0, _size);
    _size = sizeof(path);
    _ret  = -1;
    memcpy(_buffer, path, _size);
    _code = CLOSEDIR;
}
