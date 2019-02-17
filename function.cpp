#include "include/function.h"
#include "include/tools.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

DataMod::DataMod() {
	NUM = 10;
	strcpy(in_pre, "data");
	strcpy(in_suf, "in");
	strcpy(out_pre, "data");
	strcpy(out_pre, "out");
}

const int MAX_LEN = 100;

RunMod RUNMOD = MAKE_INPUT;
DataMod DATAMOD;
char MAIN[MAX_LEN] = "main.cpp";
char RUNDIR[18] = "/tmp/CHECK.XXXXXX";
char STD[MAX_LEN] = "std.cpp"; 
char MAKE[MAX_LEN] = "make.cpp";
unsigned MAXCOUNT = 1000;
char dataDir[MAX_LEN] = "./data";
char dataConf[MAX_LEN] = "./data/data.conf";
const char *helpmes = "\
\
";

bool usedConf;

void printHelp(void) {
	printf("%s", helpmes);
}

int getArgument(int argc, char *argv[]) {
	if (argc == 0) {
		printHelp();
		exit(1);
	}
	int o;
	const char *optstring = "Mhm:s:f:d:C:N:";
	while ((o = getopt(argc, argv, optstring)) != -1) {
		switch (o) {
		case 'h':
			printHelp(); exit(0);		break;
		case 'M':
			RUNMOD   = DATA_INPUT;		break;
		case 'm':
			strcpy(MAKE, optarg);		break;
		case 's':
			strcpy(STD,  optarg);		break;
		case 'f':
			strcpy(MAIN, optarg);		break;
		case 'd':
			strcpy(dataDir, optarg);	break;
		case 'C':
			usedConf = true;
			strcpy(dataConf, optarg);	break;
		case 'N':
			sscanf(optarg, "%d", &MAXCOUNT); break;
		case '?':
			fprintf(stderr, "Bad argument %c", optopt);
			fprintf(stderr, "getopt error code: %d", opterr);
			break;
		default:
			fprintf(stderr, "\033[31mError!\033[0m");
			return -1;
		}
	}
	return 0;
}

int prepare(void) {
	char tmp[MAX_LEN];
	mkdtemp(RUNDIR);
	switch (RUNMOD) {
		case DATA_INPUT:
			/* Link dataDir to RUNDIR */
			if (getrealpath(dataDir, MAX_LEN) == NULL) {
				fprintf(stderr, "Data Directory %s: Not Exists.\n", dataDir);
				finish_clean(); return 1;
			}
			strcat(strcpy(tmp, RUNDIR), "/data");
			symlink(dataDir, tmp);
			/* END */
			/* Link MAIN to RUNDIR */
			if (getrealpath(MAIN, MAX_LEN) == NULL) {
				fprintf(stderr, "MAIN sources %s: Not Exists.\n", MAIN);
				finish_clean(); return 1;
			}
			strcat(strcpy(tmp, RUNDIR), "/main.cpp");
			symlink(MAIN, tmp);
			/* END */
			/* Read data config */
			if (!usedConf) strcat(strcpy(dataConf, dataDir), "/data.conf");
			if (getrealpath(dataConf, MAX_LEN)) {
				FILE *f = fopen(dataConf, "r");
				if (readDataConfig(f, &DATAMOD)) {
					fprintf(stderr, "Something going wrong while reading data config\n");
					finish_clean();  return 1;
				}
				fclose(f);
			}
			else /*if (usedConf)*/ return fprintf(stderr, "Can't Find data config: %s\n", dataConf), 1;
			/* END */
			break;
		case MAKE_INPUT:
			/* Link MAIN to RUNDIR */
			if (getrealpath(MAIN, MAX_LEN) == NULL) {
				fprintf(stderr, "MAIN sources %s: Not Exists.\n", MAIN);
				finish_clean(); return 1;
			}
			strcat(strcpy(tmp, RUNDIR), "/main.cpp");
			symlink(MAIN, tmp);
			/* END */
			/* Link STD to RUNDIR */
			if (getrealpath(STD, MAX_LEN) == NULL) {
				fprintf(stderr, "STD sources %s: Not Exists.\n", STD);
				finish_clean(); return 1;
			}
			strcat(strcpy(tmp, RUNDIR), "/std.cpp");
			symlink(STD, tmp);
			/* END */
			/* Link MAKE to RUNDIR */
			if (getrealpath(MAKE, MAX_LEN) == NULL) {
				fprintf(stderr, "MAKE sources %s: Not Exists.\n", MAKE);
				finish_clean(); return 1;
			}
			strcat(strcpy(tmp, RUNDIR), "/make.cpp");
			symlink(MAKE, tmp);
			/* END */
			break;
	}
	chdir(RUNDIR);
	return 0;
}

int readDataConfig(FILE *config, DataMod *dm) {
	bool check[5] = { false, false, false, false, false };
	int cnt = 0;
	char pre[100], suf[100];
	while (fscanf(config, "%s = %s \n", pre, suf) != EOF) {
		if (!strcmp(pre, "NUM")) {
			if (check[0]) return fprintf(stderr, "To many NUM config.\n"), 1;
			check[0] = true, ++cnt;
			if (!isDigit(suf)) return fprintf(stderr, "%s: Bad number: %s\n", dataConf, suf), 1;
			sscanf(suf, "%d", &dm->NUM);
		}
		else if (!strcmp(pre, "IN_PRE")) {
			if (check[1]) return fprintf(stderr, "%s: To many IN_PRE config.\n", dataConf), 1;
			check[1] = true, ++cnt;
			strcpy(dm->in_pre, suf);
		}
		else if (!strcmp(pre, "IN_SUF")) {
			if (check[2]) return fprintf(stderr, "%s: To many IN_SUF config.\n", dataConf), 1;
			check[2] = true, ++cnt;
			strcpy(dm->in_suf, suf);
		}
		else if (!strcmp(pre, "OUT_PRE")) {
			if (check[3]) return fprintf(stderr, "%s: To many OUT_PRE config.\n", dataConf), 1;
			check[3] = true, ++cnt;
			strcpy(dm->out_pre, suf);
		}
		else if (!strcmp(pre, "OUT_SUF")) {
			if (check[4]) return fprintf(stderr, "%s: To many OUT_SUF config.\n", dataConf), 1;
			check[4] = true, ++cnt;
			strcpy(dm->out_suf, suf);
		}
		else return fprintf(stderr, "%s: Unkown name: %s\n", dataConf, pre), 1;
	}
	if (cnt != 5) return fprintf(stderr, "%s: Too few arguments.\n", dataConf), 1;
	return 0;
}

void finish_clean(int mod, char *path) {
	if (mod == 1) {
		char file[1000];
		strcat(strcpy(file, path), "/testdata.in");
		copyfile("./testdata.in", file);
		strcat(strcpy(file, path), "/testdata.out");
		copyfile("./testdata.out", file);
		strcat(strcpy(file, path), "/testdata.ans");
		copyfile("./testdata.ans", file);
	}
	if (rm_dir(RUNDIR) != 0) fprintf(stderr, "clean failed.\n");
	return ;
}