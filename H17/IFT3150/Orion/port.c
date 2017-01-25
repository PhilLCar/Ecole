#include <stdio.h>
#include <string.h>

#ifdef __linux__
#define open_port  _open_port
#define close_port _close_port
#define readchar   _readchar
#define peekchar   _peekchar
#define write_char _write_char
#define eof        _eof
#endif

struct PORT {
  int p;
  FILE *f;
};

struct PORT open_port(char *filename, int mode) {
  struct PORT port;
  port.p = 0;
  if (mode) {
    if (strcmp(filename, "stdout"))
      port.f = fopen(filename, "wb+");
    else
      port.f = stdout;
  } else {
    if (strcmp(filename, "stdin"))
      port.f = fopen(filename, "rb");
    else 
      port.f = stdin;
  }
  return port;
}

int close_port(struct PORT *port)
{
  fclose(port->f);
}

int peekchar(struct PORT *port)
{
  if (!port->p) {
    port->p = fgetc(port->f);
  }
  return port->p;
}

int readchar(struct PORT *port)
{
  int r;
  if (port->p)
    r = port->p;
  else 
    r = fgetc(port->f);
  port->p = 0;
  return r;
}

void writechar(char c, struct PORT *port)
{
  fputc(c, port->f);
}

int eof(struct PORT *port) {
  return peekchar(port) == EOF;
}
