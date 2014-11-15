#include "FuseFS.h"
FuseFS * FuseFS::_instance = 0;

void FuseFS::setRootDir(const char * path) {

}

int FuseFS::Getattr(const char *path, struct stat *statbuf) {
    return 0;
}

int FuseFS::Readlink(const char *path, char *link, size_t size) {
    return 0;
}

int FuseFS::Mknod(const char *path, mode_t mode, dev_t dev) {
    return 0;
}

int FuseFS::Mkdir(const char *path, mode_t mode) {
    return 0;
}

int FuseFS::Unlink(const char *path) {
    return 0;
}

int FuseFS::Rmdir(const char *path) {
    return 0;
}

int FuseFS::Symlink(const char *path, const char *link) {
    return 0;
}
int FuseFS::Rename(const char *path, const char *newpath) {
    return 0;
}
int FuseFS::Link(const char *path, const char *newpath) {
    return 0;
}
int FuseFS::Chmod(const char *path, mode_t mode) {
    return 0;
}
int FuseFS::Chown(const char *path, uid_t uid, gid_t gid) {
    return 0;
}
int FuseFS::Truncate(const char *path, off_t newSize) {
    return 0;
}
int FuseFS::Utime(const char *path, struct utimbuf *ubuf) {
    return 0;
}
int FuseFS::Open(const char *path, struct fuse_file_info *fileInfo) {
    return 0;
}
int FuseFS::Read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    return 0;
}
int FuseFS::Write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    return 0;
}
int FuseFS::Statfs(const char *path, struct statvfs *statInfo) {
    return 0;
}
int FuseFS::Flush(const char *path, struct fuse_file_info *fileInfo) {
    return 0;
}
int FuseFS::Release(const char *path, struct fuse_file_info *fileInfo) {
    return 0;
}
int FuseFS::Fsync(const char *path, int datasync, struct fuse_file_info *fi) {
    return 0;
}
int FuseFS::Setxattr(const char *path, const char *name, const char *value, size_t size, int flags) {
    return 0;
}
int FuseFS::Getxattr(const char *path, const char *name, char *value, size_t size) {
    return 0;
}
int FuseFS::Listxattr(const char *path, char *list, size_t size) {
    return 0;
}
int FuseFS::Removexattr(const char *path, const char *name) {
    return 0;
}
int FuseFS::Opendir(const char *path, struct fuse_file_info *fileInfo) {
    return 0;
}
int FuseFS::Readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo) {
    return 0;
}
int FuseFS::Releasedir(const char *path, struct fuse_file_info *fileInfo) {
    return 0;
}
int FuseFS::Fsyncdir(const char *path, int datasync, struct fuse_file_info *fileInfo) {
    return 0;
}
void * FuseFS::Init(struct fuse_conn_info *conn) {
}
int FuseFS::Truncate(const char *path, off_t offset, struct fuse_file_info *fileInfo) {
    return 0;
}

