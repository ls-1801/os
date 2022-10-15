#include "io.h"
#include "../driver/uart/printer/sync_uart_printer.h"
#include "../driver/uart/printer/uart_printer.h"
#include "printer/printer.h"
#include "printer/string_printer.h"
#include "strings.h"
#include <common.h>
#include <stdarg.h>

char cpy_word(char *buf) {
  char c = getc();

  while (c != ' ' && c != '\n' && c != '\r') {
    *buf++ = c;
    c = getc();
  }

  *buf = '\0';
  return c;
}

char *pad(char *buf, uint32_t length, char c, const char *str) {
  char *ptr = buf;
  uint32_t i;
  int strLen = strnlen(str, length);
  for (i = 0; i < length - strLen; i++) {
    *ptr++ = c;
  }

  while (*str != '\0') {
    *ptr++ = *str++;
  }

  *ptr = '\0';

  return buf;
}

int kscanf(const char *format, ...) {
  char current_character = '\0';
  bool read_again = true;

  int counter = 0;
  void *arg;
  char buf[50];
  va_list args;
  va_start(args, format);
  while (*format != '\0') {

    if (*format == '%') {
      switch (*++format) {
      case '%':
        if (getc() != '%')
          return counter;
        read_again = true;
        break;

      case 'c':
        arg = va_arg(args, char *);
        *(char *)arg = getc();
        read_again = true;
        counter++;
        break;

      case 's':
        arg = va_arg(args, char *);
        current_character = cpy_word(arg);
        read_again = false;
        counter++;
        break;

      case 'i':
      case 'd':
        arg = va_arg(args, int *);
        current_character = cpy_word(buf);
        read_again = false;
        counter++;

        *(int *)arg = atoi_dec(buf);
        break;

      case 'p':
      case 'x':
        arg = va_arg(args, unsigned *);
        current_character = cpy_word(buf);
        read_again = false;
        counter++;

        *(unsigned *)arg = atou_hex(buf);
        break;

      case 'u':
        arg = va_arg(args, unsigned *);
        current_character = cpy_word(buf);
        read_again = false;
        counter++;

        *(unsigned *)arg = atou_dec(buf);
      }

    } else {

      if (read_again) {
        current_character = getc();
      }

      if (*format == current_character) {
        read_again = true;
      } else {
        return counter++;
      }
    }

    format++;
  }
  va_end(args);
  return counter++;
}

void vfprintf(printer_t p, const char *format, va_list args) {
  int arg;
  const char *s;
  char buf[50];
  char padd_buffer[20];

  while (*format != '\0') {
    if (*format != '%') {
      putc(p, *format++);
    } else {
      switch (*++format) {
      case '%':
        putc(p, '%');
        break;

      case 'c':
        arg = va_arg(args, int);
        putc(p, (char)arg);
        break;

      case 'i':
      case 'd':
        arg = va_arg(args, int);
        puts(p, itoa_dec(arg, buf));
        break;

      case 'u':
        arg = va_arg(args, unsigned int);
        puts(p, utoa_dec((unsigned)arg, buf));
        break;

      case 'p':
      case 'x':
        arg = va_arg(args, unsigned int);
        puts(p, "0x");
        puts(p, pad(padd_buffer, 8, '0', utoa_hex((unsigned)arg, buf)));
        break;

      case 's':
        s = va_arg(args, const char *);
        puts(p, s);
        break;
      }
      format++;
    }
  }
  p->close(p);
  va_end(args);
}

void kprintf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(get_printer_uart_sync(), format, args);
  va_end(args);
}

char *sprintf(char *buf, const char *format, ...) {
  va_list args;
  va_start(args, format);
  struct StringPrinter printer = get_string_printer(buf);
  vfprintf((printer_t)&printer, format, args);
  va_end(args);
  return buf;
}

void printf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(get_printer_uart(), format, args);
  va_end(args);
}