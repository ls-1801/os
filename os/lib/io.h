#ifndef LIB_IO_H
#define LIB_IO_H

#include "../driver/io.h"

/*
    Uses the synchronized (blocking) uart printer.
*/
void kprintf(const char *format, ...);

/*
    Uses the nonblock uart printer.
*/
void printf(const char *format, ...);

char *sprintf(char *buf, const char *format, ...);
int kscanf(const char *format, ...);

#endif /*LIB_IO_H*/