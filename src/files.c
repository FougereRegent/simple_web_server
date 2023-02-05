#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "files.h"

#define FILE_PATH "www-data/"


static int check_file(const char *filename);

extern struct file_info *open_file(const char *filename) {
	return NULL;
}

extern char *file_content(const FILE *fp) {
	return NULL;
}

extern long file_size(const FILE *fp) {
	return 0;
}

static int check_file(const char *filename) {
	int status;
	struct stat s;

	return 0;
}
