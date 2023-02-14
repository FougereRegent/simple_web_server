#ifndef _HANDLE_FILES_H
#include<stdio.h>
#include<stdlib.h>
#endif

#define ERROR_NOT_FOUND -1
#define ERROR_NON_AUTHORISATION -2

struct file_info {
	FILE *fp;
	char *filename;
	long size;
};

extern struct file_info *open_file(const char *filename);
extern long file_size(const char *filename);
extern void free_struct(struct file_info* info);
