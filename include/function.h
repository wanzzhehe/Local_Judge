#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include <cstdio>

typedef enum RunMod {
	MAKE_INPUT = 0,
	DATA_INPUT = 1
}RunMod;

typedef struct DataMod {
	int NUM;
	char in_pre[20], in_suf[20];
	char out_pre[20], out_suf[20];
	DataMod();
}DataMod;

/* BASE CONFIGURATIONS */
extern const char *helpmes; // HELP MESSAGE
extern RunMod RUNMOD; // RUN MOD
extern char MAIN[100]; // THE SOURCE WILL BE TESTED.
extern char MAIN_ELF[100];
extern char RUNDIR[18];
/* END */
/* RUN IN MAKE_INPUT MOD */
extern unsigned MAXCOUNT; // MAX REPEAT TIME; DEFAULT 1000; -1 IS UNLIMITED
extern char STD[100]; // THE STANDARD SOURCE FILE
extern char MAKE[100]; // THE DATA MAKER SOURCE FILE
extern const char STD_ELF[100];
extern const char MAKE_ELF[100];
/* END */
/* RUN IN DATA_INPUT MOD */
extern char dataDir[100]; // THE DATA DIRECTORY
extern char dataConf[100]; // THE DATA CONFIG
extern DataMod DATAMOD;
/* END */

int getArgument(int argc, char *argv[]);
void printHelp(void);
int prepare(void);
void finish_clean(int mod = 0, char *path = NULL);

/* RUN IN DATA_INPUT MOD */
int readDataConfig(FILE *config, DataMod *dm);
/* END */

#endif // _FUNCTION_H_