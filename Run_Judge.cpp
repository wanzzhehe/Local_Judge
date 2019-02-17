#include "include/Run_Judge.h"
#include "include/tools.h"
#include <unistd.h>
#include <sys/wait.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

int compile(int argc, char *argv[]) {
	/*
	pid_t fpid = fork();
	if (fpid < 0) {
		fprintf(stderr, "compile(): fork error.\n");
		return 1;
	}
	else if (fpid == 0) if (execvp("g++", argv)) return fprintf(stderr, "compile(): exec Error.\n"), 1;
	else {
		int status;
		if (wait(&status)) {
			fprintf(stderr, "Unkown Error.\n");
			return 1;
		}
		else if (WIFEXITED(status)) return 0;
		else return fprintf(stderr, "Compile Failed.\n"), 1;
	}
	*/
	return 0;
}

int run(char *elf, char *input_file, char *output_file) {
	pid_t fpid = fork();
	if (fpid < 0) {
		fprintf(stderr, "run(): fork error.\n");
		return 1;
	}
	else if (fpid == 0) {
		if (input_file)  freopen(input_file,  "r", stdin );
		if (output_file) freopen(output_file, "w", stdout);
		if (execl(elf, NULL)) {
			fprintf(stderr, "exec: System Error.\n");
			return 1;
		}
	}
	else {
		int status;
		wait(&status);
		if (WIFEXITED(status)) {
			if (WEXITSTATUS(status) == 1) {
				fprintf(stderr, "System Error.\n");
				return 2;
			}
		}
		else {
			printf("%s: Runtime Error.\n", elf);
			return 1;
		}
	}
	return 0;
}

int compare(char *file1, char *file2) {
	char *buf1, *buf2;
	int EXITCODE = 0;
	size_t size1, size2;
	if ((size1 = readFile(file1, buf1)) == -1) EXITCODE = 2;
	else if ((size2 = readFile(file2, buf2)) == -1) EXITCODE = 2;
	else if (strcmp(buf1, buf2)) EXITCODE = 1;
	if(EXITCODE != 2) free(buf1), free(buf2);
	return EXITCODE;
}