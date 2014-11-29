#ifndef MESSAGE_H
#define MESSAGE_H

// Serialization and deserialization of all the messages.

#include <cstdlib>
#include <iostream>
#include <fuse.h>
#include <sstream>
#include <vector>
// Message codes.
#define MAX_SIZE 512

#define CONNECT_CODE 1 
#define GETATTR 2
#define STATFS 3
#define OPENDIR 4
#define READDIR 5
#define CLOSEDIR 6

class Message {
    public:
        Message(int, std::string);
        int _code;
        int _ret;
        // Size that makes sense in the continued buffer
        int _size;
        char _buffer[MAX_SIZE];
        ~Message() {
        }

        Message(const Message &m) {
            _code = m._code;
            _ret = m._ret;
            _size = m._size;
            for (int i = 0; i < _size; i++) 
                _buffer[i] = m._buffer[i];
        }
        Message() {
            _code = -1;
            _ret = -1;
            _size = 0;
        }
        /*
         * * Message creaters.
         * * Modify already instantiated Message.
         * */
        void create_getAttr(const char * path);
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
        void create_statfs(const char * path);
        void create_flush(const char * path, struct fuse_file_info *fileInfo);
        void create_release(const char * path, struct fuse_file_info *fileInfo);
        void create_fsync(const char * path, int datasync, struct fuse_file_info *fileInfo);
        void create_setxattr(const char * path, const char *name, const char *value, 
                size_t size, int flags);
        void create_getxattr(const char * path, const char *name, char *value, size_t size);
        void create_listxattr(const char * path, char *list, size_t size);
        void create_removexattr(const char * path, const char *name);
        void create_opendir(const char * path);
        void create_readdir(const char * path);

        void create_releasedir(const char * path, struct fuse_file_info *fileInfo);
        void create_fsyncdir(const char * path, int datasync, struct fuse_file_info *fileInfo);
        void create_init(struct fuse_conn_info *conn);
	    
        void create_closedir(const char * path);
		// Networking stuff.
		const char *serialize();
        
        static Message * toMessage(char *buffer) {
            Message *msg = new Message();
            memcpy(&msg -> _code, buffer, sizeof(int));
            memcpy(&msg -> _ret, buffer + sizeof(int), sizeof(int));
            memcpy(&msg -> _size, buffer + 2 * sizeof(int), sizeof(int));
            memcpy(msg -> _buffer, buffer + 3 * sizeof(int), msg -> _size);
            
            return msg;
        }
        static void fillMessage(Message *msg, char *buffer) {
            memcpy(&msg -> _code, buffer, sizeof(int));
            memcpy(&msg -> _ret, buffer + sizeof(int), sizeof(int));
            memcpy(&msg -> _size, buffer + 2 * sizeof(int), sizeof(int));
            memcpy(msg -> _buffer, buffer + 3 * sizeof(int), msg -> _size);
        }
};

#endif
