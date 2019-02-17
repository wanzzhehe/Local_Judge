#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <csignal>
#include "include/function.h"
#include "include/messages.h"
#include "include/Run_Judge.h"
#include "include/tools.h"

char firstpath[1000];

void finish(int sigsum) {
	finish_clean(sigsum);
	return ;
}

int main(int argc, char *argv[]) {
	signal(SIGINT, finish);
	getcwd(firstpath, 1000);
	if (getArgument(argc, argv) != 0) return 1;
	if (prepare() != 0) return 1;
	switch (RUNMOD) {
		case DATA_INPUT: {
			if (system("g++ main.cpp -o main.elf -O2 -Wall -DLOCAL")) {
				printf("main.cpp compile error.\n");
				return 1;
			}
			/* TEST */
			//run("./main.elf", "./data/data1.in", "./data.out");
			/* END */
			char input_file[100];
			char *output_file = "./testdata.out";
			char ansput_file[100];
			for (int i = 1; i <= DATAMOD.NUM; ++i) {
				sprintf(input_file, "./data/%s%d.%s", DATAMOD.in_pre, i, DATAMOD.in_suf);
				sprintf(ansput_file, "./data/%s%d.%s", DATAMOD.out_pre, i, DATAMOD.out_suf);
				int RUNSTATUS = run("./main.elf", input_file, output_file);
				if (RUNSTATUS != 0) {
					if (RUNSTATUS == 1) printf("%d: %s.\n", i, RE);
					else printf("%s.\n", SE), exit(1);
				}
				int COMSTATUS = compare(output_file, ansput_file);
				if(COMSTATUS == 0) printf("%d: %s.\n", i, AC);
				else if (COMSTATUS == 1) printf("%d: %s.\n", i, WA);
				else if (COMSTATUS == 2) printf("$s.\n", SE), exit(1);
				else printf("%s.\n", UE), exit(1);
			}
			break;
		}
		case MAKE_INPUT: {
			char *input_file = "./testdata.in";
			char *output_file = "./testdata.out";
			char *ansput_file = "./testdata.ans";
			if (system("g++ main.cpp -o main.elf -O2 -Wall -DLOCAL")) {
				printf("%s compile error.\n", MAIN);
				return 1;
			}
			if (system("g++ make.cpp -o make.elf -O2 -Wall -DLOCAL")) {
				printf("%s compile error.\n", MAKE);
				return 1;
			}
			if (system("g++ std.cpp -o std.elf -O2 -Wall -DLOCAL")) {
				printf("%s compile error.\n", STD);
				return 1;
			}
			int RUNSTATUS, COMSTATUS;
			for (int i = 1; i != MAXCOUNT; ++i) {
				RUNSTATUS = run("./make.elf", NULL, "testdata.in");
				if (RUNSTATUS != 0) {
					if (RUNSTATUS == 1) printf("%d: %s: Runtime Error.\n", i, MAKE);
					else if (RUNSTATUS == 2) printf("System Error.\n"), exit(1);
				}
				RUNSTATUS = run("./std.elf", "testdata.in", "testdata.ans");
				if (RUNSTATUS != 0) {
					if (RUNSTATUS == 1) printf("%d: %s: Runtime Error.\n", i, STD);
					else if (RUNSTATUS == 2) printf("System Error.\n"), exit(1);
				}
				RUNSTATUS = run("./main.elf", "testdata.in", "testdata.out");
				if (RUNSTATUS != 0) {
					if (RUNSTATUS == 1) printf("%d: %s: Runtime Error.\n", i, MAIN);
					else if (RUNSTATUS == 2) printf("System Error.\n"), exit(1);
				}
				COMSTATUS = compare("./testdata.out", "./testdata.ans");
				if (COMSTATUS == 0) printf("%d: Accept.\n", i);
				else if (COMSTATUS == 1) {
					printf("%d: Wrong Answer.\n", i);
					finish_clean(1, firstpath); exit(0);
				}
				else if (COMSTATUS == 2) printf("System Error.\n"), exit(1);
			}
			break;
		}
	}
	finish_clean();
	return 0;
}