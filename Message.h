#ifndef MESSAGE_H
#define MESSAGE_H

// Serialization and deserialization of all the messages.

#include <cstdlib>
#include <iostream>
#include <fuse.h>

class Message {
    public:
        Message(int, std::string);
        int _code;
        std::string _msg;
        std::string getMessage();
        ~Message();
        Message(const Message &m) {
            _code = m._code;
            _msg = m._msg;
        }
        Message() {
            _code = -1;
            _msg = "NULL";
        }
        Message(int code) {
            _code = code;
            _msg = "NULL";
        }
        /*
         * * Message creaters.
         * * Modify already instantiated Message.
         * */
        void create_getAttr(const char * path, struct stat *st);
        void create_readlink(const char * path, char *link, size_t size);
        void create_mknod(const char * path, mode_t mode, dev_t dev);
        void create_mkdir(const char * path, mode_t mode);
        void create_unlink(const char * path);
        void create_rmdir(const char * path);
        void create_symlink(const char * path, const char *link);
        void create_rename(const char * path, const char * newPath);
        void create_link(const char * path, const char *newPath);
        void create_chmod(const char * path, mode_t mode);
        void create_chown(const char * path, uid_t uid, gid_t gid);
        void create_utime(const char * path, struct utimbuf *ubuf);
        void create_truncate(const char * path, off_t newSize);
        void create_open(const char * path, struct fuse_file_info *fileInfo);
        void create_read(const char * path, char *buf, size_t size, 
                off_t offset, struct fuse_file_info *fileInfo);
        void create_write(const char * path, const char *buf, size_t size,
                off_t offset, struct fuse_file_info *fileInfo);
        void create_statfs(const char * path, struct statvfs *statInfo);
        void create_flush(const char * path, struct fuse_file_info *fileInfo);
        void create_release(const char * path, struct fuse_file_info *fileInfo);
        void create_fsync(const char * path, int datasync, struct fuse_file_info *fileInfo);
        void create_setxattr(const char * path, const char *name, const char *value, 
                size_t size, int flags);
        void create_getxattr(const char * path, const char *name, char *value, size_t size);
        void create_listxattr(const char * path, char *list, size_t size);
        void create_removexattr(const char * path, const char *name);
        void create_opendir(const char * path, struct fuse_file_info *fileInfo);
        void create_readdir(const char * path, void *buf, fuse_fill_dir_t filler, off_t offset,
                struct fuse_file_info *fileInfo);
        void create_releasedir(const char * path, struct fuse_file_info *fileInfo);
        void create_fsyncdir(const char * path, int datasync, struct fuse_file_info *fileInfo);
        void create_init(struct fuse_conn_info *conn);
		
		// Networking stuff.
		const char *serialize();
		static Message * toMessage(char *);
};

#endif
