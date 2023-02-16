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
	info->size = file_size(filename);
	if((info->fp = open(info->filename, O_RDONLY, 0)) == -1)
		return NULL;
	return info;
}

extern long file_size(const char *filename) {
	long file_size = 0;
	char *pathfile =  reel_path(filename);
	struct stat st;

	if(check_file(pathfile) == -1)
		return ERROR_NON_AUTHORISATION;

	if(stat(filename, &st) == -1)
		return ERROR_NOT_FOUND;

	file_size = st.st_size;
	free(pathfile);
	return file_size;
}

extern void free_struct(struct file_info* info) {
	free(info->filename);
	close(info->fp);
}

static int check_file(const char *filename) {
	return access(filename, F_OK | R_OK);
}

static char* reel_path(const char* filename) {
	char *pathfile =  (char*)malloc(sizeof(char) * 255);
	memset((char*)pathfile, 0, 255);
	sprintf(pathfile, "%s/%s", FILE_PATH, filename);
	return pathfile;
}
