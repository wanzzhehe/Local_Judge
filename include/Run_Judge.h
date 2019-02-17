#ifndef _RUN_JUDGE_H_
#define _RUN_JUDGE_H_

int compile(int argc, char *argv[]); // THIS IS A PIECE OF SHIT
int run(char *elf, char *input_file, char *output_file);
int compare(char *file1, char *file2);

#endif