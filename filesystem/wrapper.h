#ifndef WRAPPER_H
#define WRAPPER_H

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

#include "FuseFS.h"

#define INS(x) FuseFS::getInstance()->x

void setRootDir(const char *path) {
    INS(setRootDir(path));
}

int wrap_getAttr(const char * path, struct stat *st) {
    return INS(Getattr(path, st));
}

int wrap_readlink(const char * path, char *link, size_t size) {
    return INS(Readlink(path, link, size));
}  

int wrap_mknod(const char * path, mode_t mode, dev_t dev) {
    return INS(Mknod(path, mode, dev));
}

int wrap_mkdir(const char * path, mode_t mode) {
    return INS(Mkdir(path, mode));
}

int wrap_unlink(const char * path) {
    return INS(Unlink(path));
}

int wrap_rmdir(const char * path) {
    return INS(Rmdir(path));
}

int wrap_symlink(const char * path, const char *link) {
    return INS(Symlink(path, link));
}

int wrap_rename(const char * path, const char * newPath) {
    return INS(Rename(path, newPath));
}

int wrap_link(const char * path, const char *newPath) {
    return INS(Link(path, newPath));
}

int wrap_chmod(const char * path, mode_t mode) {
    return INS(Chmod(path, mode));
}

int wrap_chown(const char * path, uid_t uid, gid_t gid) {
    return INS(Chown(path, uid, gid));
}

int wrap_utime(const char * path, struct utimbuf *ubuf) {
    return INS(Utime(path, ubuf));
}

int wrap_truncate(const char * path, off_t newSize) {
    return INS(Truncate(path, newSize));
}

int wrap_open(const char * path, struct fuse_file_info *fileInfo) {
    return INS(Open(path, fileInfo));
}

int wrap_read(const char * path, char *buf, size_t size, 
        off_t offset, struct fuse_file_info *fileInfo) {
    return INS(Read(path, buf, size, offset, fileInfo));
}

int wrap_write(const char * path, const char *buf, size_t size,
        off_t offset, struct fuse_file_info *fileInfo) {
    return INS(Write(path, buf, size, offset, fileInfo));
}

int wrap_statfs(const char * path, struct statvfs *statInfo) {
    return INS(Statfs(path, statInfo));
}

int wrap_flush(const char * path, struct fuse_file_info *fileInfo) {
    return INS(Flush(path, fileInfo));
}
int wrap_release(const char * path, struct fuse_file_info *fileInfo) {
    return INS(Release(path, fileInfo));
}
int wrap_fsync(const char * path, int datasync, struct fuse_file_info *fileInfo) {
    return INS(Fsync(path, datasync, fileInfo));
}
int wrap_setxattr(const char * path, const char *name, const char *value, size_t size, int flags) {
    return INS(Setxattr(path, name, value, size, flags));
}
int wrap_getxattr(const char * path, const char *name, char *value, size_t size) {
    
    return INS(Getxattr(path, name, value, size));
}
int wrap_listxattr(const char * path, char *list, size_t size) {
    return INS(Listxattr(path, list, size));
}
int wrap_removexattr(const char * path, const char *name) {
    return INS(Removexattr(path, name));
}
int wrap_opendir(const char * path, struct fuse_file_info *fileInfo) {
    return INS(Opendir(path, fileInfo));
}
int wrap_readdir(const char * path, void *buf, fuse_fill_dir_t filler, off_t offset,
            struct fuse_file_info *fileInfo) {
    return INS(Readdir(path, buf, filler, offset, fileInfo));
}
int wrap_releasedir(const char * path, struct fuse_file_info *fileInfo) {
    return INS(Releasedir(path, fileInfo));
}
int wrap_fsyncdir(const char * path, int datasync, struct fuse_file_info *fileInfo) {
    return INS(Fsyncdir(path, datasync, fileInfo));
}
void *wrap_init(struct fuse_conn_info *conn) {
    return INS(Init(conn));
}

#endif
