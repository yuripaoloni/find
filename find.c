#include "find.h"

// Program to implement KMP Algorithm in C
int main(void)
{
	char* text = "ABaaaaaaaaaaaaaaaAAAAAAACABAABCABAC";
	char* pattern = "CAB";

	int n = strlen(text);
	int m = strlen(pattern);

	KMP(text, pattern, n, m);

	return 0;
}