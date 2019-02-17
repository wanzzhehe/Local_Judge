#include "include/tools.h"
#include <unistd.h>
#include <cstring>
#include <cctype>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdio>
#include <cstdlib>

bool isdir(const char *path) {
	struct stat s_buf;
	stat(path, &s_buf);
	if (S_ISDIR(s_buf.st_mode)) return true;
	else return false;
}

char *getrealpath(char *path, int MAXSIZE) {
	if (access(path, R_OK)) return NULL;
	char nowpath[1000], nextpath[1000], filename[1000];
	getcwd(nowpath, 1000);

	if (path[0] != '/') strcat(strcpy(nextpath, "./"), path);
	else strcpy(nextpath, path);
	int len = strlen(nextpath);
	if (nextpath[len - 1] == '/') nextpath[--len] = '\0';
	if (isdir(nextpath)) filename[0] = '\0';
	else {
		int i = len;
		while (nextpath[i - 1] != '/') --i;
		filename[0] = '/';
		strcpy(filename + 1, nextpath + i);
		nextpath[i] = '\0';
	}

	chdir(nextpath);
	getcwd(path, MAXSIZE);
	chdir(nowpath);

	return strcat(path, filename);
}

bool isDigit(char *num) {
	if (num[0] == '0' || num[0] == '\0') return false;
	int i = 0;
	while (isdigit(num[i++]));
	if (num[i - 1] == '\0') return true;
	else return false;
}

int rm_dir(const char *dir) {
	struct stat dir_stat;
	if (access(dir, F_OK)) return 0;
	if (lstat(dir, &dir_stat) < 0) {
		fprintf(stderr, "Can't get directory stat.\n");
		return 1;
	}
	if (S_ISDIR(dir_stat.st_mode)) {
		DIR *dirp = opendir(dir);
		struct dirent *dp;
		char nextdir[100];
		while ((dp = readdir(dirp)) != NULL) {
			if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue;
			sprintf(nextdir, "%s/%s", dir, dp->d_name);
			rm_dir(nextdir);
		}
		closedir(dirp);
		rmdir(dir);
	}
	else if (S_ISLNK(dir_stat.st_mode) || S_ISREG(dir_stat.st_mode)) remove(dir);
	return 0;
}

size_t filesize(char *file) {
	struct stat s_stat;
	if (stat(file, &s_stat) < 0) {
		fprintf(stderr, "Can't get file stat.\n");
		return -1;
	}
	else return s_stat.st_size;
}

size_t readFile(char *file, char *&buf) {
	FILE *f = fopen(file, "rb");
	size_t size = filesize(file);
	if (size == -1) {
		fprintf(stderr, "readFile: System Error.\n");
		return -1;
	}
	if ((buf = (char *)malloc(size + 1)) == NULL) {
		fprintf(stderr, "Memory is not enough.\n");
		return -1;
	}
	if (fread(buf, 1, size, f) != size) {
		fprintf(stderr, "Read file Error.\n");
		free(buf);
		return -1;
	}
	unsigned i = 0, j = 0;
	while (isspace(buf[i])) ++i;
	while (i < size) {
		if (isspace(buf[i]) && isspace(buf[i + 1])) ++i;
		else if (isspace(buf[i])) buf[j++] = ' ', ++i;
		else buf[j++] = buf[i++];
	}
	while (buf[j - 1] == ' ') --j;
	buf[j] = '\0';
	fclose(f);
	return j;
}

int copyfile(char *from, char *to) {
	FILE *in = fopen(from, "rb");
	FILE *out = fopen(to, "wb");
	register char buf[1 << 20];
	size_t size;
	while ((size = fread(buf, 1, 1 << 20, in))) fwrite(buf, 1, size, out);
	fclose(in); fclose(out);
	return 0;
}