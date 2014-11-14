#include "fs.h"

#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse.h>
#include <libgen.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/xattr.h>

void setRootDir(const char *path) {

}

int getAttr(const char * path, struct start *) {
    return 0;
}

int readlink(const char * path, char *link, size_t size) {
    return 0;

}  

int mknod(const char * path mode_t mode, dev_t dev) {
    return 0;
}

int mkdir(const char * path, mode_t mode) {
    return 0;
}

int unlink(const char * path) {
    return 0;
}

int rmdir(const char * path) {
    return 0;
}

int symlink(const char * path, const char *link) {
    return 0;
}

int rename(const char * path, const char * newPath) {
    return 0;
}

int link(const char * path, const char *newPath) {
    return 0;
}

int chmod(const char * path, mode_t mode) {
    return 0;
}

int chown(const char * path, uid_t uid, gid_t gid) {
    return 0;
}

int utime(const char * path, struct utimbuf *ubuf) {
    return 0;
}

int truncate(const char * path, off_t newSize) {
    return 0;
}

int open(const char * path, struct fuse_file_info *fileInfo) {
    return 0;
}

int read(const char * path, char *buf, size_t size, off_t offset, \
         struct fuse_file_info *fileInfo) {
    return 0;
}

int write(const char * path, const char *buf, size_t size, off_t offset, 
          struct fuse_file_info *fileInfo) {
    return 0;
}

int statfs(const char * path, struct statvfs *statInfo) {
    return 0;
}

int flush(const char * path, struct fuse_file_info *fileInfo) {
    return 0;
}
int release(const char * path, struct fuse_file_info *fileInfo) {
    return 0;
}
int fsync(const char * path, int datasync, struct fuse_file_info *fileInfo) {
    return 0;
}
int setxattr(const char * path, const char *name, const char *value, size_t size) {
    return 0;
}
int getxattr(const char * path, const char *name, char *value, sizE_t size) {
    return 0;
}
int listxattr(const char * path, char *list, size_t size) {
    return 0;
}
int removexattr(const char * path, const char *name) {
    return 0;
}
int opendir(const char * path, struct fuse_file_info *fileInfo) {
    return 0;
}
int readdir(const char * path, void *buf, fuse_fill_dir_t filler, off_t offset,
            struct fuse_file_info *fileInfo) {
    return 0;
}
int releasedir(const char * path, struct fuse_file_info *fileInfo) {
    return 0;
}
int fsyncdir(const char * path, int datasync, struct fuse_file_info *fileInfo) {
    return 0;
}
int inti(struct fuse_conn_info *conn) {
    return 0;
}
