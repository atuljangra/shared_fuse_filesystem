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

int wrap_getAttr(const char * path, struct stat *st) {
    return 0;
}

int wrap_readlink(const char * path, char *link, size_t size) {
    return 0;

}  


int wrap_mknod(const char * path, mode_t mode, dev_t dev) {
    return 0;
}

int wrap_mkdir(const char * path, mode_t mode) {
    return 0;
}

int wrap_unlink(const char * path) {
    return 0;
}

int wrap_rmdir(const char * path) {
    return 0;
}

int wrap_symlink(const char * path, const char *link) {
    return 0;
}

int wrap_rename(const char * path, const char * newPath) {
    return 0;
}

int wrap_link(const char * path, const char *newPath) {
    return 0;
}

int wrap_chmod(const char * path, mode_t mode) {
    return 0;
}

int wrap_chown(const char * path, uid_t uid, gid_t gid) {
    return 0;
}

int wrap_utime(const char * path, struct utimbuf *ubuf) {
    return 0;
}

int wrap_truncate(const char * path, off_t newSize) {
    return 0;
}

int wrap_open(const char * path, struct fuse_file_info *fileInfo) {
    return 0;
}

int wrap_read(const char * path, char *buf, size_t size, 
        off_t offset, struct fuse_file_info *fileInfo) {
    return 0;
}

int wrap_write(const char * path, const char *buf, size_t size,
        off_t offset, struct fuse_file_info *fileInfo) {
    return 0;
}

int wrap_statfs(const char * path, struct statvfs *statInfo) {
    return 0;
}

int wrap_flush(const char * path, struct fuse_file_info *fileInfo) {
    return 0;
}
int wrap_release(const char * path, struct fuse_file_info *fileInfo) {
    return 0;
}
int wrap_fsync(const char * path, int datasync, struct fuse_file_info *fileInfo) {
    return 0;
}
int wrap_setxattr(const char * path, const char *name, const char *value, size_t size, int flags) {
    return 0;
}
int wrap_getxattr(const char * path, const char *name, char *value, size_t size) {
    
    return 0;
}
int wrap_listxattr(const char * path, char *list, size_t size) {
    return 0;
}
int wrap_removexattr(const char * path, const char *name) {
    return 0;
}
int wrap_opendir(const char * path, struct fuse_file_info *fileInfo) {
    return 0;
}
int wrap_readdir(const char * path, void *buf, fuse_fill_dir_t filler, off_t offset,
            struct fuse_file_info *fileInfo) {
    return 0;
}
int wrap_releasedir(const char * path, struct fuse_file_info *fileInfo) {
    return 0;
}
int wrap_fsyncdir(const char * path, int datasync, struct fuse_file_info *fileInfo) {
    return 0;
}
void *wrap_init(struct fuse_conn_info *conn) {
    return 0;
}
