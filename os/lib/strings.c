#include "strings.h"
#include "memory.h"
#include <limits.h>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

char tolowerc(char c) {
  if (c >= 'A' && c <= 'Z')
    c = 'a' + (c - 'A');
  return c;
}

char *tolower(char *str) {
  if (*str == '\0')
    return str;

  do {
    if (*str >= 'A' && *str <= 'Z')
      *str = 'a' + (*str - 'A');

  } while (*++str != '\0');

  return str;
}

char *toupper(char *str) {
  if (*str == '\0') {
    return str;
  }

  do {
    if (*str >= 'a' && *str <= 'z')
      *str = 'A' + (*str - 'a');

  } while (*++str != '\0');

  return str;
}

int atoi_dec(const char *str) {
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

unsigned atou_hex(const char *str) {
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

unsigned atou_dec(const char *str) {
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

char *reverse(char *str, int length) {
  int i;

  for (i = 0; i < length / 2; i++) {
    char tmp = str[i];
    str[i] = str[length - 1 - i];
    str[length - 1 - i] = tmp;
  }

  return str;
}

char *utoa_dec(unsigned num, char *buf) { return utoa(num, buf, 10); }
char *utoa_hex(unsigned num, char *buf) { return utoa(num, buf, 16); }
char *utoa_bin(unsigned num, char *buf) { return utoa(num, buf, 2); }

char *itoa_dec(int num, char *buf) { return itoa(num, buf, 10); }

char *utoa(unsigned num, char *str, int base) {
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

  reverse(str, i);

  return str;
}

char *itoa(int num, char *str, int base) {
  int i = 0;
  bool isNegative = false;

  if (num == 0) {
    str[i++] = '0';
    str[i] = '\0';
    return str;
  }

  if (num == INT_MIN) {
    memcpy(str, "-2147483648", sizeof("-2147483648"));
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

  reverse(str, i);

  return str;
}

int strncmp(const char *lhs, const char *rhs, unsigned strlen) {
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

int strnlen(const char *str, unsigned max_length) {
  unsigned i;
  for (i = 0; i < max_length; i++) {
    if (str[i] == '\0') {
      break;
    }
  }

  return i;
}