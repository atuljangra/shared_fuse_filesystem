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
    _size = strlen(path);
    _ret = -1;
    memcpy(_buffer, path, _size);
    printf("%d %s %s %lu\n", _size, _buffer, path, strlen(path));
    _code = GETATTR;
}

void Message::create_statfs(const char *path) {
    memset(_buffer, 0, _size);
    _size = strlen(path);
    _ret = -1;
    memcpy(_buffer, path, _size);
    _code = STATFS; 
}

void Message::create_opendir(const char *path) {
    memset(_buffer, 0, _size);
    _size = strlen(path);
    _ret  = -1;
    memcpy(_buffer, path, _size);
    _code = OPENDIR;
}

void Message::create_readdir(const char *path) {
    memset(_buffer, 0, _size);
    _size = strlen(path);
    _ret  = -1;
    memcpy(_buffer, path, _size);
    _code = READDIR;
}

void Message::create_access(const char *path, int mode) {
    memset(_buffer, 0, _size);
    _size = sizeof(int) + strlen(path);
    _ret  = -1;
    memcpy(_buffer, &mode, sizeof(int));
    memcpy(_buffer + sizeof(int), path, _size - sizeof(int));
    _code = ACCESS;
}

void Message::create_closedir(const char *path) {
    memset(_buffer, 0, _size);
    _size = strlen(path);
    _ret  = -1;
    memcpy(_buffer, path, _size);
    _code = CLOSEDIR;
}

void Message::create_open(const char *path) {
    memset(_buffer, 0, _size);
    _size = strlen(path);
    _ret = -1;
    memcpy(_buffer, path, _size);
    _code = OPEN;
}

void Message::create_close(const char *path) {
    memset(_buffer, 0, _size);
    _size = strlen(path);
    _ret = -1;
    memcpy(_buffer, path, _size);
    _code = CLOSE;
}

void Message::create_read(const char *path, size_t size, off_t offset) {
    memset(_buffer, 0, _size);
    _size = (2 * sizeof(int)) + strlen(path);
    _ret = -1;
    _code = READ;
    memcpy(_buffer, &size, sizeof(int));
    memcpy(_buffer, &offset, sizeof(int));
    int sizeofPath = strlen(path);
    memcpy(_buffer, path, sizeofPath);
}

void Message::create_mknod(const char *path, mode_t mode, dev_t dev) {
    memset(_buffer, 0, _size);
    _size = sizeof(mode_t) + sizeof(dev_t) + strlen(path);
    _ret = -1;
    _code = MKNOD;
    memcpy(_buffer, &mode, sizeof(mode_t));
    memcpy(_buffer + sizeof(mode_t), &dev, sizeof(dev_t));
    int sizeOfPath = strlen(path);
    memcpy(_buffer + sizeof(mode_t) + sizeof(dev_t), path, sizeOfPath);
}

