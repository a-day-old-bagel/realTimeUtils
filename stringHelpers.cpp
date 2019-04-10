
#include <cstdlib>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <cassert>

#include "stringHelpers.hpp"

int cstrcmp(const char *str1, const char *str2) {
	int d;
	while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) {
		str1++;
		str2++;
	}
	return d;
}
int cstrncmp(const char *str1, const char *str2, int n) {
	int d = 0;
	while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) {
		str1++;
		str2++;
		n--;
	}
	return d;
}
char *cstrdup(const char *str) {
	size_t len = strlen(str) + 1;
	void *buf = malloc(len);
	assert(buf);
	return (char *) memcpy(buf, (const void *) str, len);
}
void cstrtrim(char *str) {
	char *str_end = str + strlen(str);
	while (str_end > str && str_end[-1] == ' ') str_end--;
	*str_end = 0;
}
