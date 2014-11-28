/* 
 * This is the file that handles the filesystem calls on the server side of
 * the sharedfile system.
 *
 * All the paths presented here are absolute local file system paths.
 *
 */
#ifndef FILEHANDLER_H
#define FILEHANDLER_H

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
#include <sys/stat.h>


#include "../utils.h"

int file_getAttr(const char * path, struct stat *st) {
    int result;
    result = lstat(path, st);
    printf("getAttr %s ret %d\n", path, result);
    return RET_ERRNO(result);
}

int file_readlink(const char * path, char *link, size_t size) {
}  

int file_mknod(const char * path, mode_t mode, dev_t dev) {
}

int file_mkdir(const char * path, mode_t mode) {
}

int file_unlink(const char * path) {
}

int file_rmdir(const char * path) {
}

int file_symlink(const char * path, const char *link) {
}

int file_rename(const char * path, const char * newPath) {
}

int file_link(const char * path, const char *newPath) {
}

int file_chmod(const char * path, mode_t mode) {
}

int file_chown(const char * path, uid_t uid, gid_t gid) {
}

int file_utime(const char * path, struct utimbuf *ubuf) {
}

int file_truncate(const char * path, off_t newSize) {
}

int file_open(const char * path, struct fuse_file_info *fileInfo) {
}

int file_read(const char * path, char *buf, size_t size, 
        off_t offset, struct fuse_file_info *fileInfo) {
}

int file_write(const char * path, const char *buf, size_t size,
        off_t offset, struct fuse_file_info *fileInfo) {
}

int file_statfs(const char * path, struct statvfs *statInfo) {
    int result = RET_ERRNO(statvfs(path, statInfo));
    printf("statfs for %s ret %d\n", path, result);
    return result;
}

int file_flush(const char * path, struct fuse_file_info *fileInfo) {
}
int file_release(const char * path, struct fuse_file_info *fileInfo) {
}
int file_fsync(const char * path, int datasync, struct fuse_file_info *fileInfo) {
}
int file_setxattr(const char * path, const char *name, const char *value, size_t size, int flags) {
}
int file_getxattr(const char * path, const char *name, char *value, size_t size) {
}
int file_listxattr(const char * path, char *list, size_t size) {
}
int file_removexattr(const char * path, const char *name) {
}
int file_opendir(const char * path, struct fuse_file_info *fileInfo) {
}
/*
 * Open, Read and close.
 */
int file_readdir(const char * path, void *buf, fuse_fill_dir_t filler, off_t offset,
            struct fuse_file_info *fileInfo) {
    DIR *dp;
}
int file_releasedir(const char * path, struct fuse_file_info *fileInfo) {
}
int file_fsyncdir(const char * path, int datasync, struct fuse_file_info *fileInfo) {
}
void *file_init(struct fuse_conn_info *conn) {
}

#endif
