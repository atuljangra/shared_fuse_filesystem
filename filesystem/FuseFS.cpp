#include <string>
#include <sstream>

#include "FuseFS.h"
#include "../utils.h"

#define log(...) \
                do { if (!OUT) fprintf(stdout, ##__VA_ARGS__); \
                    else fprintf(f, ##__VA_ARGS__); } while (0)

using namespace std;
/*
 * TODO: This functions will serve as the basis for the handlers on server side.
 * TODO: Create message for each of the method.
 * TODO: Send all the requests over the network.
 * TODO: Error renaming, permission denied. Possible problem with chmod, chown.
 * TODO: Flush, Release Sync. What can be the problem?
 * TODO: Error 61 getxattr while renaming.
 * TODO: Write -2. getattr -2 frequest while writing
 *
 */
FuseFS * FuseFS::_instance = 0;

const char * FuseFS::_fullPath(const char * path) {
    stringstream ss;
    ss << _root << path;
    return ss.str().c_str();
}

void FuseFS::setRootDir(const char * path) {
    _root = path;
    log("Setting FS root to %s\n", path);
}

int FuseFS::Getattr(const char *path, struct stat *statbuf) {
    log("GetAttr(%s)\n", path);
    Message *msg = new Message();
    msg -> create_getAttr(path); 
    Message *retMsg = new Message();
    _network->send(msg, true, retMsg);
    int ret = 0; 
    log("getAttr ret %d\n", ret);
    return ret;
}

int FuseFS::Readlink(const char *path, char *link, size_t size) {
    const char *fullPath = _fullPath(path);
    log("readLink(path=%s, link=%s, size=%lud)\n", path, link, size);
    int ret = 0; // RET_ERRNO(readlink(fullPath, link, size));
    log("readLink ret %d\n", ret);
    return ret;
}

int FuseFS::Mknod(const char *path, mode_t mode, dev_t dev) {
    const char *fullPath = _fullPath(path);
    log("mknod(path=%s, mode=%ud)\n", fullPath, mode); 
    int ret = 0; // RET_ERRNO(mknod(fullPath, mode, dev));
    log("mknod ret %d\n", ret);
    return ret;
}

int FuseFS::Mkdir(const char *path, mode_t mode) {
    const char *fullPath = _fullPath(path);
    log("mkdir(path=%s, mode=%ud)\n", fullPath, mode); 
    int ret = 0; // RET_ERRNO(mkdir(fullPath, mode));
    log("mkdir ret %d\n", ret);
    return ret;
}

int FuseFS::Unlink(const char *path) {
    const char *fullPath = _fullPath(path);
    log("unlink(path=%s)\n", fullPath); 
    int ret = 0; // RET_ERRNO(unlink(fullPath));
    log("unlink ret %d\n", ret);
    return ret;
}

int FuseFS::Rmdir(const char *path) {
    const char *fullPath = _fullPath(path);
    log("rmdir(path=%s)\n", fullPath); 
    int ret = 0; // RET_ERRNO(rmdir(fullPath));
    log("rmdir ret %d\n", ret);
    return ret;
}

int FuseFS::Symlink(const char *path, const char *link) {
    const char *fullPath = _fullPath(path);
    log("symlink(path=%s, link=%s)\n", fullPath, link); 
    int ret = 0;// RET_ERRNO(symlink(fullPath, link));
    log("symlink ret %d\n", ret);
    return ret;
}
int FuseFS::Rename(const char *path, const char *newpath) {
    const char *fullPath = _fullPath(path);
    log("rename(path=%s, newPath=%s)\n", fullPath, newpath); 
    int ret = 0; //RET_ERRNO(rename(fullPath, newpath));
    log("rename ret %d\n", ret);
    return ret;
}
int FuseFS::Link(const char *path, const char *newpath) {
    const char *fullPath = _fullPath(path);
    const char *newPath = _fullPath(newpath);
    log("link(path=%s, newpath=%s)\n", fullPath, newPath); 
    int ret = 0; //RET_ERRNO(link(fullPath, newPath));
    log("link ret %d\n", ret);
    return ret;
}
int FuseFS::Chmod(const char *path, mode_t mode) {
    const char *fullPath = _fullPath(path);
    log("chmod(path=%s, mode=%ud)\n", fullPath, mode); 
    int ret = 0; //RET_ERRNO(chmod(fullPath, mode));
    log("chmod ret %d\n", ret);
    return ret;
}
int FuseFS::Chown(const char *path, uid_t uid, gid_t gid) {
    const char *fullPath = _fullPath(path);
    log("chown(path=%s, uid=%d, gid=%d)\n", fullPath, uid, gid); 
    int ret = 0; //RET_ERRNO(chown(fullPath, uid, gid));
    log("chown ret %d\n", ret);
    return ret;
}
int FuseFS::Truncate(const char *path, off_t newSize) {
    const char *fullPath = _fullPath(path);
    log("truncate(path=%s, newSize=%d)\n", fullPath, (int)newSize); 
    int ret = 0; //RET_ERRNO(truncate(fullPath, newSize));
    log("truncate ret %d\n", ret);
    return ret;
}
int FuseFS::Truncate(const char *path, off_t offset, struct fuse_file_info *fileInfo) {
    const char *fullPath = _fullPath(path);
    log("truncate(path=%s, offset=%d)\n", fullPath, (int)offset); 
    int ret = 0; //RET_ERRNO(ftruncate(fileInfo -> fh, offset));
    log("truncate fh ret %d\n", ret);
    return ret;
}

int FuseFS::Utime(const char *path, struct utimbuf *ubuf) {
    const char *fullPath = _fullPath(path);
    log("utime(path=%s)\n", fullPath); 
    int ret = 0; //RET_ERRNO(utime(fullPath, ubuf));
    log("utime ret %d\n", ret);
    return ret;
}
int FuseFS::Open(const char *path, struct fuse_file_info *fileInfo) {
    const char *fullPath = _fullPath(path);
    log("open(path=%s)\n", fullPath); 
    fileInfo -> fh = 0; //open(fullPath, fileInfo -> flags);
    log("open filehandle is %lud\n", fileInfo -> fh);
    return 0;
}
int FuseFS::Read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    const char *fullPath = _fullPath(path);
    log("read(path=%s, size=%d, offset=%d)\n", fullPath,(int)size, (int)offset); 
    int ret = 0; //RET_ERRNO(pread(fileInfo -> fh, buf, size, offset));
    log("Read ret %d\n", ret);
    return ret;
}
int FuseFS::Write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    const char *fullPath = _fullPath(path);
    log("write(path=%s, size=%d, offset=%d)\n", fullPath, (int)size, (int)offset); 
    int ret = 0; //RET_ERRNO(pwrite(fileInfo -> fh, buf, size, offset));
    log("Write ret %d\n", ret);
    return ret;
}

int FuseFS::Statfs(const char *path, struct statvfs *statInfo) {
    const char *fullPath = _fullPath(path);
    log("statfs(path=%s)\n", fullPath); 
    int ret = 0; //RET_ERRNO(statvfs(fullPath, statInfo));
    log("statfs ret %d\n", ret);
    return ret;
}
int FuseFS::Flush(const char *path, struct fuse_file_info *fileInfo) {
    const char *fullPath = _fullPath(path);
    log("flush(path=%s)\n", fullPath); 
    log("METHOD NOT IMPLEMENTED\n");
    return 0;
}
int FuseFS::Release(const char *path, struct fuse_file_info *fileInfo) {
    const char *fullPath = _fullPath(path);
    log("release(path=%s)\n", fullPath); 
    int ret = 0; //RET_ERRNO(close(fileInfo -> fh));
    return ret;
}
int FuseFS::Fsync(const char *path, int datasync, struct fuse_file_info *fi) {
    const char *fullPath = _fullPath(path);
    log("fsync(path=%s, datasync=%d)\n", fullPath, datasync); 
    int ret = 0; //(datasync) ? RET_ERRNO(fdatasync(fi -> fh)) : RET_ERRNO(fsync(fi -> fh));
    log("fsycn ret %d\n", ret);
    return ret;
}

int FuseFS::Setxattr(const char *path, const char *name, const char *value, size_t size, int flags) {
    const char *fullPath = _fullPath(path);
    log("setxattr(path=%s, name=%s, value=%s, size=%lud, flags=%d)\n", 
            fullPath, name, value, size, flags); 
    int ret = 0; //RET_ERRNO(lsetxattr(fullPath, name, value, size, flags));
    log("setxattr ret %d\n", ret);
    return ret;
}
int FuseFS::Getxattr(const char *path, const char *name, char *value, size_t size) {
    const char *fullPath = _fullPath(path);
    log("getxattr(path=%s, name=%s, size=%d)\n", fullPath, name, (int)size); 
    // TODO check this
    int ret = 0; //RET_ERRNO(lgetxattr(fullPath, name, value, size));
    log("getxattr ret %d\n", ret);
    return ret;
}

int FuseFS::Listxattr(const char *path, char *list, size_t size) {
    const char *fullPath = _fullPath(path);
    log("listxattr(path=%s, size=%d)\n", fullPath, (int)size); 
    int ret = 0; //RET_ERRNO(llistxattr(fullPath, list, size));
    log("listxattr ret %d\n", ret);
    return ret;
}
int FuseFS::Removexattr(const char *path, const char *name) {
    const char *fullPath = _fullPath(path);
    log("Removexattr(path=%s, name=%s)\n", fullPath, name); 
    int ret = 0; //RET_ERRNO(lremovexattr(fullPath, name));
    log("removexattr ret %d\n", ret);
    return ret;
}
int FuseFS::Opendir(const char *path, struct fuse_file_info *fileInfo) {
    const char *fullPath = _fullPath(path);
    log("opendir(path=%s)\n", fullPath); 
    DIR *dir = 0; //opendir(fullPath);
    fileInfo -> fh = (uint64_t)dir;
    log("opendir fh=%lud\n", fileInfo -> fh);
    return NULL== dir ? -errno : 0;
}
int FuseFS::Readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo) {
    const char *fullPath = _fullPath(path);
    log("readdir(path=%s, offset=%lud)\n", fullPath, offset); 
    DIR *dir = (DIR *)fileInfo -> fh;
    struct dirent *de = 0; //readdir(dir);
    
    return 0;
    
    /*if (NULL == de) {
        return -errno;
    }
    else {
        do {
            if (filler(buf, de ->d_name, NULL, 0) != 0) {
                return ENOMEM;
            }
        } while ((de = readdir(dir)) != NULL);
    }
    return 0;
*/
}


int FuseFS::Releasedir(const char *path, struct fuse_file_info *fileInfo) {
    const char *fullPath = _fullPath(path);
    log("releasedir(path=%s)\n", fullPath); 
    int ret = 0; //RET_ERRNO(closedir((DIR *)fileInfo -> fh));
    log("Releasedir ret %d\n", ret);
    return ret;
}
int FuseFS::Fsyncdir(const char *path, int datasync, struct fuse_file_info *fileInfo) {
    const char *fullPath = _fullPath(path);
    log("fsyncdir (path=%s)\n", fullPath); 
    log("METHOD NOT IMPLEMENTED");
    return 0;
}
void * FuseFS::Init(struct fuse_conn_info *conn) {
    log("Returning 0 from Init");
    return 0;
}

