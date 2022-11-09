#ifndef _FAT_FS_OPS_H
#define _FAT_FS_OPS_H

#include "fat_fs_tree.h"
#include <fuse/fuse.h>


// Remove file
int fat_fuse_unlink(const char *path);

// Remove only empty directories
int fat_fuse_rmdir(const char *path);

/* Get file attributes (file descriptor version) */
int fat_fuse_fgetattr(const char *path, struct stat *stbuf,
                      struct fuse_file_info *fi);

/* Get file attributes (path version) */
int fat_fuse_getattr(const char *path, struct stat *stbuf);

/* Open a file */
int fat_fuse_open(const char *path, struct fuse_file_info *fi);

/* Open a directory */
int fat_fuse_opendir(const char *path, struct fuse_file_info *fi);

int fat_fuse_mkdir(const char *path, mode_t mode);

/* Creates a new file in @path. @mode and @dev are ignored. */
int fat_fuse_mknod(const char *path, mode_t mode, dev_t dev);

/* Read data from a file */
int fat_fuse_read(const char *path, char *buf, size_t size, off_t offset,
                  struct fuse_file_info *fi);

/* Add entries of a directory in @fi to @buf using @filler function. */
int fat_fuse_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                     off_t offset, struct fuse_file_info *fi);

/* Close a file */
int fat_fuse_release(const char *path, struct fuse_file_info *fi);

/* Close a directory */
int fat_fuse_releasedir(const char *path, struct fuse_file_info *fi);

int fat_fuse_utime(const char *path, struct utimbuf *buf);

/* Shortens the file at the given offset.*/
int fat_fuse_truncate(const char *path, off_t offset);

/* Write data from a file */
int fat_fuse_write(const char *path, const char *buf, size_t size, off_t offset,
                   struct fuse_file_info *fi);

#endif /* _FAT_FS_OPS_H */
