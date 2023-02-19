#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "files.h"

#define FILE_PATH "www-data"

static int check_file(const char *filename);
static char* reel_path(const char* filename);

extern struct file_info *open_file(const char *filename) {
	struct file_info *info = (struct file_info*) malloc(sizeof(struct file_info));
	info->filename = reel_path(filename);
	info->size = file_size(info->filename);
	if((info->fp = open(info->filename, O_RDONLY, 0)) == -1)
		return NULL;

	info->data = (unsigned char*)malloc(info->size);
	memset(info->data, 0, info->size);
	read(info->fp, (unsigned char*)info->data, info->size);
	return info;
}

extern long file_size(const char *filename) {
	long file_size = 0;
	struct stat st;

	if(check_file(filename) == -1)
		return ERROR_NON_AUTHORISATION;

	if(stat(filename, &st) == -1)
		return ERROR_NOT_FOUND;

	file_size = st.st_size;
	return file_size;
}

extern void free_struct(struct file_info* info) {
	free(info->filename);
	free(info->data);
	close(info->fp);
	free(info);
}

extern int file_exist(const char* filename) {
	char *filepath = reel_path(filename);
	int result = 0;
	
	if(access(filepath, F_OK) == 0)
		result = 0;
	else
		result = ERROR_NOT_FOUND;

	free(filepath);
	return result;
}

static int check_file(const char *filename) {
	return access(filename, F_OK | R_OK);
}

static char* reel_path(const char* filename) {
	char *pathfile =  (char*)malloc(sizeof(char) * 255);
	memset((char*)pathfile, 0, 255);
	strncat(pathfile, FILE_PATH, strlen(FILE_PATH));
	strncat(pathfile, "/", 1);
	strncat(pathfile, filename, strlen(filename));
	return pathfile;
}
