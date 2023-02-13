#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "files.h"

#define FILE_PATH "www-data"


static int check_file(const char *filename);

extern struct file_info *open_file(const char *filename) {
	return NULL;
}

extern char *file_content(const FILE *fp) {
	return NULL;
}

extern long file_size(const char *filename) {
	long file_size = 0;
	char pathfile[255] = {'\0'};
	struct stat st;

	sprintf(pathfile, "%s%s", FILE_PATH, filename);

	if(check_file(pathfile) == -1)
		return NON_AUTHORISATION;

	if(stat(filename, &st) == -1)
		return -1;
	file_size = st.st_size;
	return file_size;
}

static int check_file(const char *filename) {
	return access(filename, F_OK | R_OK);
}
