#include "messages.h"

void MESSAGES::crash(NODE *node) {
	NODE *next;
	while (node != NULL) {
		next = node->next;
		delete node;
		node = node->next;
	}
	return ;
}

void MESSAGES::make(char *res) {
	maxn = strlen(res);
	result = new char[maxn + 1];
	strcpy(result, res);
	if (maxn == 0) return;
	int id = 0;
	first = new NODE(1, RAC);
	if (res[0] == 'A') first->res = RAC;
	else if (res[0] == 'W') first->res = RWA;
	else if (res[0] == 'R') first->res = RRE;
	else if (res[0] == 'U') first->res = RUE;
	else if (res[0] == 'S') {
		first->res = RSE; maxn = 1; return ;
	}
	NODE *now = first;
	while (res[++id] != '\0') {
		now->next = new NODE(id + 1, RAC);
		now = now->next;
		if (res[id] == 'A') now->res = RAC;
		else if (res[id] == 'W') now->res = RWA;
		else if (res[id] == 'R') now->res = RRE;
		else if (res[id] == 'U') now->res = RUE;
		else if (res[id] == 'S') {
			crash(first); maxn = 1;
			first = new NODE(1, RSE);
			return ;
		}
	}
}

void MESSAGES::print(FILE *f) {
	fprintf(f, "%s\n", result);
	NODE *now = first;
	while (now) {
		switch (now->res) {
		case RAC:
			fprintf(f, "No.%d : %s\n", now->id, AC);
			break;
		case RWA:
			fprintf(f, "No.%d : %s\n", now->id, WA);
			break;
		case RRE:
			fprintf(f, "No.%d : %s\n", now->id, RE);
			break;
		case RSE:
			fprintf(f, "No.%d : %s\n", now->id, SE);
			break;
		case RUE:
			fprintf(f, "No.%d : %s\n", now->id, UE);
			break;
		}
	}
}

MESSAGES::MESSAGES(void) {
	first = NULL;
	maxn = 0;
}

MESSAGES::~MESSAGES(void) {
	crash(this->first);
}

MESSAGES::MESSAGES(char *res) {
	make(res);
}

MESSAGES::NODE::NODE(int i, RESULT r) {
	id = i, res = r;
	next = NULL;
}
