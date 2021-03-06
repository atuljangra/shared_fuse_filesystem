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
#include <map>
#include <string>

#include "../utils.h"

struct Dirp { 
    DIR *dp;
};

struct Filep {
    int fp;
};

// Assuming paths are null terminating.
struct cmp_str {
    bool operator() (const char * a, const char * b) {
        return strcmp(a, b) < 0;
    }
};
using namespace std;

map <string, Dirp *> mapDirp;
map <string, Filep *> mapFilep;

void initMapdirp() {
}

int file_getAttr(const char * path, struct stat *st) {
    int result;
    result = lstat(path, st);
    printf("getAttr %s ret %d errno %d\n", path, result, errno);
    return RET_ERRNO(result);
}

int file_readlink(const char * path, char *link, size_t size) {
return 0;
}  

int file_mknod(const char * path, mode_t mode, dev_t dev) {
    int result = mknod(path, mode, dev);
    if (result == -1) {
        printf("Mknod %s error %d\n", path, errno);
        return -errno;
    }
    printf("Mknod %s ret %d\n", path, result);
    return 0;
}

int file_access(const char *path, int mode) {
    int result = access(path, mode);
    if (result == -1) {
        printf("Access %s error %d\n", path, errno);
        return -errno;
    }
    printf("Access %s ret %d\n", path, result);
    return 0;
}

int file_mkdir(const char * path, mode_t mode) {
return 0;
}

int file_unlink(const char * path) {
    int result = unlink(path);
    
    if (result == -1) {
        printf("Unlink%s error %d\n", path, errno);
        return -errno;
    }
    printf("Unlink %s ret %d\n", path, result);
    return 0;
}

int file_rmdir(const char * path) {
return 0;
}

int file_symlink(const char * path, const char *link) {
return 0;
}

int file_rename(const char * path, const char * newPath) {
    int result = rename(path, newPath);
    
    if (result == -1) {
        printf("Rename Error: %s %s error %d\n", path, newPath, errno);
        return -errno;
    }
    printf("Rename  %s %s ret %d\n", path, newPath, result);
    return 0;
}

int file_link(const char * path, const char *newPath) {
return 0;
}

int file_chmod(const char * path, mode_t mode) {
return 0;
}

int file_chown(const char * path, uid_t uid, gid_t gid) {
return 0;
}

int file_utime(const char * path, struct utimbuf *ubuf) {
return 0;
}

int file_truncate(const char * path, off_t newSize) {
return 0;
}

int file_open(const char * path) {
    string key(path);
    printf("FileOpen %s %s \n", path, key.data());
    int fp = open(path, O_RDWR);
    if (fp == -1) {
        printf("Error while opening file error: %d\n", errno);
        return -errno;
    }
    struct Filep filep;
    filep.fp = fp;
    mapFilep[key] = &filep;
    printf("Adding %s to map\n", key.data());
    return 0;
}

int file_read(const char * path, char *buf, size_t size, off_t offset) {
    string key(path);
    printf("File read %s %s \n", path, key.data());
    struct Filep *filep;
    filep = mapFilep[key];
    if (filep == NULL) {
        printf("Error reading while getting %s\n", key.data());
        return -1;
    }
    int result = pread(filep->fp, buf, size, offset);
    if (result == -1) {
        printf("Error while reading %s errno: %d\n", path, errno);
        return -errno;
    }
    printf("Read %d bytes\n", result);
    return result;
}

int file_close(const char * path) {
    string key(path);
    printf("File close: %s %s\n", path, key.data());
    struct Filep *filep;
    filep = mapFilep[key];
    if (filep == NULL) {
        printf("Error while getting %s\n", key.data());
        return -1;
    }
    close(filep -> fp);
    // Remove from the hashmap.
    mapFilep.erase(key);
    printf("Removed %s\n", key.data());
    return 0;
}

int file_write(const char * path, const char *buf, size_t size,
        off_t offset) {
    string(key);
    printf("File write %s %s \n", path, key.data());
    struct Filep *filep;
    filep = mapFilep[key];
    if (filep == NULL) {
        printf("Error write while getting %s\n", key.data());
        return -1;
    }
    int result = pwrite(filep->fp, buf, size, offset);
    if (result == -1) {
        printf("Error while writing %s errno: %d\n", path, errno);
        return -errno;
    }
    printf("write %d bytes\n", result);
    return result;
}

int file_statfs(const char * path, struct statvfs *statInfo) {
    int result = RET_ERRNO(statvfs(path, statInfo));
    printf("statfs for %s ret %d\n", path, result);
    return result;
}

int file_flush(const char * path, struct fuse_file_info *fileInfo) {
return 0;
}
int file_release(const char * path, struct fuse_file_info *fileInfo) {
return 0;
}
int file_fsync(const char * path, int datasync, struct fuse_file_info *fileInfo) {
return 0;
}
int file_setxattr(const char * path, const char *name, const char *value, size_t size, int flags) {
return 0;
}
int file_getxattr(const char * path, const char *name, char *value, size_t size) {
return 0;
}
int file_listxattr(const char * path, char *list, size_t size) {
return 0;
}
int file_removexattr(const char * path, const char *name) {
return 0;
}
int file_opendir(const char * path) {
    DIR *dp;
    string key(path);
    printf("Trying to open %s %s\n", path, key.data());
    dp = opendir(path);
    if (dp == NULL)
        return -errno;

    // Add dp to the hashmap;
    struct Dirp *dirp = (struct Dirp *)malloc(sizeof(struct Dirp));
    dirp -> dp = dp;
    mapDirp[key] = dirp;
    printf("Added %s to hashmap\n", key.data());
    return 0;
}

int file_closedir(const char *path) {
    // Get the file pointer.
    string key(path);
    struct Dirp *dirp = mapDirp[key];
    // make sure that this is not null.
    if (!dirp) {
        printf("Error while getting DIR for %s\n", path);
        return -1;
    }
    DIR * dp = dirp -> dp;
    closedir(dp);
    mapDirp.erase(key);
    printf("Removed %s to hashmap\n", key.data());
    return 0;
}

int file_readdir(const char * path, struct dirent *de, bool &finish) {
    // Get the file pointer.
    string key(path);
    struct Dirp *dirp = mapDirp[key];
    if (!dirp) {
        printf("Error while getting DIR for %s\n", path);
        return -1;
    }
    DIR *dp = dirp -> dp;
    int prevErrno = errno;
    de = readdir(dp);

    int result = 0;
    // NULL and -errno is same, then we are good. But -errno is changed then we
    // need to return -eerno.
    finish = false;
    if (de == NULL && errno != prevErrno) { 
        finish = true;
        result = -errno;
    }
    if (de == NULL) {
        finish = true;
        printf("NULL reached ..");
    }

    printf("Readdir returning %d, data: %s\n", result, (char *)de);

    return result; 
}

int file_releasedir(const char * path, struct fuse_file_info *fileInfo) {
return 0;
}
int file_fsyncdir(const char * path, int datasync, struct fuse_file_info *fileInfo) {
return 0;
}
void *file_init(struct fuse_conn_info *conn) {
return 0;
}

#endif
