#include "strings.h"
#include "memory.h"
#include <common.h>
#include <limits.h>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

char __tolowerc(char c) {
  if (c >= 'A' && c <= 'Z')
    c = 'a' + (c - 'A');
  return c;
}

char *__tolower(char *str) {
  if (*str == '\0')
    return str;

  do {
    if (*str >= 'A' && *str <= 'Z')
      *str = 'a' + (*str - 'A');

  } while (*++str != '\0');

  return str;
}

char *__toupper(char *str) {
  if (*str == '\0') {
    return str;
  }

  do {
    if (*str >= 'a' && *str <= 'z')
      *str = 'A' + (*str - 'a');

  } while (*++str != '\0');

  return str;
}

int __atoi_dec(const char *str) {
  int sign = 1;
  int base = 0;
  int i = 0;

  while (str[i] == ' ') {
    i++;
  }

  if (str[i] == '-') {
    sign = -1;
    str++;
  } else if (str[i] == '+') {
    str++;
  }

  while (str[i] >= '0' && str[i] <= '9') {

    if (base > INT_MAX / 10 || (base == INT_MAX / 10 && str[i] - '0' > 7)) {
      if (sign == 1)
        return INT_MAX;
      else
        return INT_MIN;
    }
    base = 10 * base + (str[i++] - '0');
  }

  return base * sign;
}

static unsigned hex_val(char c) {
  if (c >= '0' && c <= '9')
    return c - '0';

  if (c >= 'a' && c <= 'f')
    return (c - 'a') + 10;

  if (c >= 'A' && c <= 'F')
    return (c - 'A') + 10;

  return 0;
}

unsigned __atou_hex(const char *str) {
  unsigned base = 0;
  int i = 0;

  while (str[i] == ' ') {
    i++;
  }

  if (str[i] == '0' && (str[i + 1] == 'X' || str[i + 1] == 'x')) {
    str += 2;
  }

  while ((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'f') ||
         (str[i] >= 'A' && str[i] <= 'F')) {

    if (base > UINT_MAX / 16) {
      return UINT_MAX;
    }
    base = 16 * base + hex_val(str[i++]);
  }
  return base;
}

unsigned __atou_dec(const char *str) {
  unsigned base = 0;
  int i = 0;

  while (str[i] == ' ') {
    i++;
  }

  while (str[i] >= '0' && str[i] <= '9') {

    if (base > UINT_MAX / 10 || (base == UINT_MAX / 10 && str[i] - '0' > 7)) {
      return UINT_MAX;
    }
    base = 10 * base + (str[i++] - '0');
  }

  return base;
}

char *__reverse(char *str, int length) {
  int i;

  for (i = 0; i < length / 2; i++) {
    char tmp = str[i];
    str[i] = str[length - 1 - i];
    str[length - 1 - i] = tmp;
  }

  return str;
}

char *__utoa_dec(unsigned num, char *buf) { return __utoa(num, buf, 10); }
char *__utoa_hex(unsigned num, char *buf) { return __utoa(num, buf, 16); }
char *__utoa_bin(unsigned num, char *buf) { return __utoa(num, buf, 2); }

char *__itoa_dec(int num, char *buf) { return __itoa(num, buf, 10); }

char *__utoa(unsigned num, char *str, int base) {
  unsigned i = 0;

  if (num == 0) {
    str[i++] = '0';
    str[i] = '\0';
    return str;
  }

  while (num != 0) {
    unsigned rem = num % base;
    str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
    num = num / base;
  }

  str[i] = '\0';

  __reverse(str, i);

  return str;
}

char *__itoa(int num, char *str, int base) {
  int i = 0;
  bool isNegative = false;

  if (num == 0) {
    str[i++] = '0';
    str[i] = '\0';
    return str;
  }

  if (num == INT_MIN) {
    __memcpy(str, "-2147483648", sizeof("-2147483648"));
    return str;
  }

  if (num < 0 && base == 10) {
    isNegative = true;
    num = -num;
  }

  while (num != 0) {
    int rem = num % base;
    str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
    num = num / base;
  }

  if (isNegative)
    str[i++] = '-';

  str[i] = '\0';

  __reverse(str, i);

  return str;
}

int __strncmp(const char *lhs, const char *rhs, unsigned strlen) {
  unsigned i;
  for (i = 0; i < strlen + 1; i++) {
    if (lhs[i] != rhs[i]) {
      return lhs[i] < rhs[i] ? -1 : 1;
    }

    if (lhs[i] == '\0') {
      return 0;
    }
  }

  return 0;
}

uint32_t __strnlen(const char *str, uint32_t max_length) {
  uint32_t i;
  for (i = 0; i < max_length; i++) {
    if (str[i] == '\0') {
      break;
    }
  }

  return i;
}