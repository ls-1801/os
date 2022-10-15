#ifndef _LIB_PRINTER_PRINTER_H_
#define _LIB_PRINTER_PRINTER_H_

typedef struct Printer *printer_t;

typedef void (*putc_fn)(printer_t, char);
typedef void (*close_fn)(printer_t);

struct Printer {
  putc_fn putc;
  close_fn close;
};

void __putc(printer_t p, char);
void __puts(printer_t p, const char *);

#endif /* _LIB_PRINTER_PRINTER_H_ */
