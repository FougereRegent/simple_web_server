#ifndef _HANDLE_FILES_H
#include<stdio.h>
#include<stdlib.h>
#endif

#define NOT_FOUND -1
#define NON_AUTHORISATION -2

struct file_info {
	FILE *fp;
	char *filename;
	long size;
};

extern struct file_info *open_file(const char *filename);
extern char *file_content(const FILE *fp);
extern long file_size(const char *filename);
