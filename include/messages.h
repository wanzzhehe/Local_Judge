#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <ctime>
#include <cstring>
#include <cstdio>

typedef enum RESULT {
	RAC = 0,
	RWA = 1,
	RRE = 2,
	RSE = 3,
	RUE = 4
}RESULT;

constexpr auto AC = "\033[32mAccept\033[0m";
constexpr auto WA = "\033[31mWrong Answer\033[0m";
constexpr auto RE = "\033[31mRuntime Error\033[0m";
constexpr auto SE = "\033[41;30mSystem Error\033[0m";
constexpr auto UE = "\033[43;30mUnkown Error\033[0m";

typedef class MESSAGES {
private:
	struct NODE {
		RESULT res;
		int id;
		NODE *next;
		NODE(int i, RESULT r);
	};
	int maxn;
	NODE *first;
	char *result;
public:
	void crash(NODE *node);
	void make(char *res);
	void print(FILE *f = stdin);
	MESSAGES(void);
	~MESSAGES(void);
	MESSAGES(char *res);
} MESSAGES;

#endif /* message.h */