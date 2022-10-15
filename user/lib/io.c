#include "io.h"
#include "printer/printer.h"
#include "printer/string_printer.h"
#include "strings.h"
#include <common.h>
#include <stdarg.h>
#include <svc.h>

void svc_puts(const char *str) {
  asm volatile("MOV r0, %0" ::"r"(str));
  syscall(SVC_PUTS);
}

uint32_t svc_gets(char *buf, uint32_t length) {
  uint32_t read;
  asm volatile("MOV r0, %0" ::"r"(buf));
  asm volatile("MOV r1, %0" ::"r"(length));
  syscall(SVC_GETS);
  asm volatile("MOV %0, r0" : "=r"(read));
  return read;
}

static char *pad(char *buf, uint32_t length, char c, const char *str) {
  char *ptr = buf;
  uint32_t i;
  int strLen = __strnlen(str, length);
  for (i = 0; i < length - strLen; i++) {
    *ptr++ = c;
  }

  while (*str != '\0') {
    *ptr++ = *str++;
  }

  *ptr = '\0';

  return buf;
}

void __vfprintf(printer_t p, const char *format, va_list args) {
  int arg;
  const char *s;
  char buf[50];
  char padd_buffer[20];

  while (*format != '\0') {
    if (*format != '%') {
      __putc(p, *format++);
    } else {
      switch (*++format) {
      case '%':
        __putc(p, '%');
        break;

      case 'c':
        arg = va_arg(args, int);
        __putc(p, (char)arg);
        break;

      case 'i':
      case 'd':
        arg = va_arg(args, int);
        __puts(p, __itoa_dec(arg, buf));
        break;

      case 'u':
        arg = va_arg(args, unsigned int);
        __puts(p, __utoa_dec((unsigned)arg, buf));
        break;

      case 'p':
      case 'x':
        arg = va_arg(args, unsigned int);
        __putc(p, '0');
        __putc(p, 'x');
        __puts(p, pad(padd_buffer, 8, '0', __utoa_hex((unsigned)arg, buf)));
        break;

      case 's':
        s = va_arg(args, const char *);
        __puts(p, s);
        break;
      }
      format++;
    }
  }
  p->close(p);
  va_end(args);
}

void __kprintf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  char buf[1024];
  struct StringPrinter printer = __get_string_printer(buf);
  __vfprintf((printer_t)&printer, format, args);
  svc_puts(buf);
  va_end(args);
}

char *__sprintf(char *buf, const char *format, ...) {
  va_list args;
  va_start(args, format);
  struct StringPrinter printer = __get_string_printer(buf);
  __vfprintf((printer_t)&printer, format, args);
  va_end(args);
  return buf;
}