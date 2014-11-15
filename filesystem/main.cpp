/*
 * This is the main driver program for the filesystem.
 */
#include <fuse.h>
#include <cstdio>

#include "wrapper.h"
struct fuse_operations filesystem_oper;

#define DFUSE_USE_VERSION 26

int main (int argc, char *argv[]) {
    
    // Adding fuse bindings;
    // TODO add all other operations.
    filesystem_oper.getattr = wrap_getAttr;  
    filesystem_oper.readlink = wrap_readlink;
    filesystem_oper.mknod = wrap_mknod;
    filesystem_oper.mkdir = wrap_mkdir;
    filesystem_oper.unlink = wrap_unlink;
    filesystem_oper.rmdir = wrap_rmdir;
    filesystem_oper.symlink = wrap_symlink;
    filesystem_oper.rename = wrap_rename;
    filesystem_oper.link = wrap_link;
    filesystem_oper.chmod = wrap_chmod;
    filesystem_oper.chown = wrap_chown;
    filesystem_oper.utime = wrap_utime;
    filesystem_oper.truncate = wrap_truncate;
    filesystem_oper.open = wrap_open;
    filesystem_oper.read = wrap_read;
    filesystem_oper.write = wrap_write;
    filesystem_oper.statfs = wrap_statfs;
    filesystem_oper.flush = wrap_flush;
    filesystem_oper.release = wrap_release;
    filesystem_oper.fsync = wrap_fsync;
    filesystem_oper.setxattr = wrap_setxattr;
    filesystem_oper.getxattr = wrap_getxattr;
    filesystem_oper.listxattr = wrap_listxattr;
    filesystem_oper.removexattr = wrap_removexattr;
    filesystem_oper.opendir = wrap_opendir;
    filesystem_oper.readdir = wrap_readdir;
    filesystem_oper.releasedir = wrap_releasedir;
    filesystem_oper.fsyncdir = wrap_fsyncdir;
    filesystem_oper.init = wrap_init;
        
    printf("Attempting network connection.\n"); 
    // Start the network connection.    
    // Running the file system;
    //
    return 0;
}
