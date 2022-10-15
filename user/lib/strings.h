#ifndef LIB_STRINGS_H
#define LIB_STRINGS_H
#include <stdint.h>

int __atoi_dec(const char *str);
unsigned __atou_dec(const char *str);

unsigned __atou_hex(const char *str);

char *__tolower(char *str);
char *__toupper(char *str);

char *__utoa_dec(unsigned num, char *buf);
char *__utoa_hex(unsigned num, char *buf);
char *__utoa_bin(unsigned num, char *buf);

char *__itoa_dec(int num, char *buf);

char *__itoa(int num, char *str, int base);
char *__utoa(unsigned num, char *str, int base);

char *__reverse(char *str, int length);

int __strncmp(const char *lhs, const char *rhs, unsigned length);
uint32_t __strnlen(const char *str, uint32_t max_length);

#endif /*LIB_STRINGS_H*/