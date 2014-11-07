#ifndef FS_CMD_H
#define FS_CMD_H

void fs_init();
void fs_load();
void fs_ls(char *);
void fs_mkdir(char*);
void fs_rmdir(char*);
void fs_create(char*);
void fs_rm(char*);
void fs_write(char*, char*);
void fs_cat(char*);

#endif // FS_CMD_H
