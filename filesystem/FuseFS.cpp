#include <string>
#include <sstream>
#include <assert.h>

#include "FuseFS.h"
#include "../utils.h"

#define log(...) \
                do { if (OUT) fprintf(stdout, ##__VA_ARGS__); \
                    else fprintf(f, ##__VA_ARGS__); } while (0)

using namespace std;
/*
 * TODO: Error renaming, permission denied. Possible problem with chmod, chown.
 * TODO: Flush, Release Sync. What can be the problem?
 * TODO: Error 61 getxattr while renaming.
 * TODO: Write -2. getattr -2 frequest while writing
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
    int ret = retMsg -> _ret; 
    if (ret >= 0) {
        memcpy(statbuf, retMsg -> _buffer, 
                retMsg -> _size);
    }

    log("GetAttr %s ret %d\n",path, ret);
    return ret;
}

int FuseFS::Readlink(const char *path, char *link, size_t size) {
    log("readLink(path=%s, link=%s, size=%lud)\n", path, link, size);
    int ret = 0; // RET_ERRNO(readlink(fullPath, link, size));
    log("readLink ret %d\n", ret);
    return ret;
}

int FuseFS::Mknod(const char *path, mode_t mode, dev_t dev) {
    log("mknod(path=%s, mode=%u)\n", path, mode); 
    Message *msg = new Message();
    msg -> create_mknod(path, mode, dev);
    Message *response = new Message();
    _network -> send(msg, true, response);
    int ret = response-> _ret;
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
    log("rename(path=%s)\n", path); 
    Message *msg = new Message();
    msg -> create_unlink(path) ;
    Message *response = new Message();
    _network -> send(msg, true, response);
    if (response -> _ret != 0) {
        log("Unlink Error %s %d\n", path , response -> _ret);
    } 
    log("unlinkret %d\n", response -> _ret);
    return response -> _ret;
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
int FuseFS::Rename(const char *path, const char *newPath) {
    log("rename(path=%s, newPath=%s)\n", path, newPath); 
    Message *msg = new Message();
    msg -> create_rename(path, newPath);
    Message *response = new Message();
    _network -> send(msg, true, response);
    if (response -> _ret != 0) {
        log("Rename Error %s %s %d\n", path, newPath, response -> _ret);
    } 
    log("rename ret %d\n", response -> _ret);
    return response -> _ret;
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
// Open and close the file.
int FuseFS::Open(const char *path, struct fuse_file_info *fileInfo) {
    log("open(path=%s)\n", path);
    Message *openMsg = new Message();
    openMsg -> create_open(path);
    Message *response = new Message();
    _network -> send(openMsg, true, response);
    if (response -> _ret != 0) {
        log("Error opening %s\n", path);
        return response -> _ret;
    }

    // Now close the file.
    Message *closeMsg = new Message();
    closeMsg -> create_close(path);
    int cleanSize = 3 * sizeof(int) + response -> _size;
    memset(response, 0, cleanSize);
    _network -> send(closeMsg, true, response);
    if (response -> _ret != 0) {
        log("Error while closing %s", path);
    }

    log("open filehandle is %lud\n", fileInfo -> fh);
    return 0;
}
int FuseFS::Read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
    log("read(path=%s, size=%d, offset=%d)\n", path,(int)size, (int)offset);

    // See if we have this file chunk cached.
    
    // else  
    // Trying to open the file first
    Message *openMsg = new Message();
    openMsg -> create_open(path);
    Message *response = new Message();
    _network -> send(openMsg, true, response);
    if (response -> _ret != 0) {
        log("Error opening %s\n", path);
        return response -> _ret;
    }

    Message * readMsg = new Message();
    // Currently not supporting reads.
    // assert(size <= MAX_MSG_SIZE);
    readMsg -> create_read(path, size, offset);
    int cleanSize = 3 * sizeof(int) + response -> _size;
    memset(response, 0, cleanSize);
    _network -> send(readMsg, true, response);
    if (response -> _ret != 0) {
        log("Error while reading %s\n", path);
        return response -> _ret;
    }

    // Copy things into the buffer.
    memcpy(buf, response -> _buffer, response -> _size);

    // Now close the file.
    Message *closeMsg = new Message();
    closeMsg -> create_close(path);
    cleanSize = 3 * sizeof(int) + response -> _size;
    memset(response, 0, cleanSize);
    _network -> send(closeMsg, true, response);
    if (response -> _ret != 0) 
        log("Error while closing %s", path);
    
    log("Read ret %d read %d bytes\n", response -> _ret, response->_size);
    return response -> _ret;
}

int FuseFS::Write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
     log("write(path=%s, size=%d, offset=%d)\n", path,(int)size, (int)offset);

    // See if we have this file chunk cached.
    
    // else  
    // Trying to open the file first
    Message *openMsg = new Message();
    openMsg -> create_open(path);
    Message *response = new Message();
    _network -> send(openMsg, true, response);
    if (response -> _ret != 0) {
        log("Error opening %s\n", path);
        return response -> _ret;
    }

    Message * writeMsg = new Message();
    // Currently not supporting reads.
    assert(size <= MAX_MSG_SIZE);
    writeMsg-> create_write(path, buf, size, offset);
    int cleanSize = 3 * sizeof(int) + response -> _size;
    memset(response, 0, cleanSize);
    _network -> send(writeMsg, true, response);
    if (response -> _ret != 0) {
        log("Error while writing %s\n", path);
        return response -> _ret;
    }

    // Now close the file.
    Message *closeMsg = new Message();
    closeMsg -> create_close(path);
    cleanSize = 3 * sizeof(int) + response -> _size;
    memset(response, 0, cleanSize);
    _network -> send(closeMsg, true, response);
    if (response -> _ret != 0) 
        log("Error while closing %s", path);
    
    log("Write ret %d \n", response -> _ret);
    return response -> _ret;
}

int FuseFS::Access(const char *path, int mode) {
    log("Access %s mode: %d", path, mode);
    Message *msg = new Message();
    msg -> create_access(path, mode);
    Message *response = new Message();
    _network -> send(msg, true, response);
    int ret = response -> _ret;
    if (ret != 0) {
        log("Error access %s\n", path);
    }
    log("Access %s ret:%d\n", path, ret);
    return ret;
}

int FuseFS::Statfs(const char *path, struct statvfs *statInfo) {
    log("statfs(path:%s)\n", path);
    Message *msg = new Message();
    msg -> create_statfs(path);
    Message *retMsg = new Message();
    _network->send(msg, true, retMsg);
    int ret = retMsg -> _ret; 
    if (ret >= 0) {
        memcpy(statInfo, retMsg -> _buffer, 
                retMsg -> _size);
    }
     // int ret = 0; //RET_ERRNO(statvfs(fullPath, statInfo));
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
/*
 * So open is called when you want to read or write. 
 * We are working with path names. Thus files will not be opened as such.
 */
int FuseFS::Opendir(const char *path, struct fuse_file_info *fileInfo) {
    log("opendir(path=%s)\n", path); 
    return 0;
}
int FuseFS::Readdir(const char *path, void *buf, fuse_fill_dir_t filler, 
        off_t offset, struct fuse_file_info *fileInfo) {
    log("readdir(path=%s, offset=%lud)\n", path, offset); 
    // struct dirent *de = 0; //readdir(dir);
    
    log("Attempting to open dir %s... ", path);
    Message *opendirMsg = new Message();
    opendirMsg -> create_opendir(path);
    Message *response = new Message();
    _network-> send(opendirMsg, true, response);
    // Make sure that we were able to open the dir.
    if (response -> _ret != 0) {
        log("Problem while opening the dir. Returning");
        return response -> _ret;
    }

    // Attempt readdir.
    struct dirent *de = (struct dirent *)malloc(sizeof(struct dirent));
    log("Readdir on %s\n", path);
    Message *readDirMsg = new Message();
    readDirMsg -> create_readdir(path);
    // Clean response.
    int cleanSize = 3 * sizeof(int) + response -> _size;
    memset(response, 0, 3 * cleanSize);
    _network -> send(readDirMsg, true, response);
    // Or the length of returned message is not 0.
    while(response -> _ret == 0 && response -> _size != 0) {
        struct stat st;
        memset(&st, 0, sizeof(st));
        assert(response -> _size == sizeof(struct dirent));
        memcpy(de, response -> _buffer, response -> _size);
        st.st_ino = de -> d_ino;
        st.st_mode = de -> d_type << 12;
        if (filler(buf, de -> d_name, &st, 0))
            break;

        // Do the readdir again.
        log("Readdir on path %s\n", path);
        readDirMsg -> create_readdir(path);
        // Clean the response message.
        cleanSize = 3 * sizeof(int) + response -> _size;
        memset(response, 0, cleanSize);
        _network ->send(readDirMsg, true, response);
        log("size: %d\n", response -> _size);
    }
    
    log("Closing dir %s\n", path);
    // Attempting to close the directory.
    Message *closeMsg = new Message();
    closeMsg -> create_closedir(path);
    cleanSize = 3 * sizeof(int) + response -> _size;
    memset(response, 0, cleanSize);
    _network ->send(closeMsg, true, response);
    if (response -> _ret != 0) 
        log("Problem while closing the dir %s\n", path);

    return 0;
}


int FuseFS::Releasedir(const char *path, struct fuse_file_info *fileInfo) {
    log("releasedir(path:%s)\n", path); 
    return 0;
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

