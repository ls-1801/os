#ifndef LIB_STRINGS_H
#define LIB_STRINGS_H

int atoi_dec(const char *str);
unsigned atou_dec(const char *str);

unsigned atou_hex(const char *str);

char *tolower(char *str);
char *toupper(char *str);

char *utoa_dec(unsigned num, char *buf);
char *utoa_hex(unsigned num, char *buf);
char *utoa_bin(unsigned num, char *buf);

char *itoa_dec(int num, char *buf);

char *itoa(int num, char *str, int base);
char *utoa(unsigned num, char *str, int base);

char *reverse(char *str, int length);

int strncmp(const char *lhs, const char *rhs, unsigned length);
int strnlen(const char *str, unsigned max_length);

#endif /*LIB_STRINGS_H*/