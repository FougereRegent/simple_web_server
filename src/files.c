#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "files.h"

extern long file_size(const char *filename)
{
	struct stat stat_struct;
	int fp = open(filename, O_RDONLY, 0);
	
	if(fstat(fp, &stat_struct) == -1)
		return -1;
	close(fp);
	return (int) stat_struct.st_size;
}
extern void file_content(const char *filename, unsigned char *buffer, long size)
{
	int fp = open(filename, O_RDONLY, 0);
	read(fp, buffer, size);
	close(fp);
}

extern int file_exist(const char *filename)
{
	FILE *fp = fopen(filename, "r");
	if(fp == NULL) {
		return 0;
	}
	
	fclose(fp);
	return 1;
}
