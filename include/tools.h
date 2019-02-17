#ifndef _TOOLS_H_
#define _TOOLS_H_

#include <stddef.h>

bool isdir(const char *path);
char *getrealpath(char *path, int MAXSIZE);
bool isDigit(char *num);
int rm_dir(const char *dir);
size_t filesize(char *file);
size_t readFile(char *file, char *&buf);
int copyfile(char *from, char *to);

#endif //_TOOLS_H_