#include <cstdio>
#include <ctime>
#include <cstdlib>

int main() { 
	srand(time(NULL));
	printf("%d %d\n", rand(), rand());
	return 0;
}
