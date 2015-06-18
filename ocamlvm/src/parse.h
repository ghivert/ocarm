#ifndef __PARSE__
#define __PARSE__

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "exec.h"

#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifndef SEEK_END
#define SEEK_END 2
#endif

#define Reverse_32(dst,src) {                                               \
  char * _p, * _q;                                                          \
  char _a, _b;                                                              \
  _p = (char *) (src);                                                      \
  _q = (char *) (dst);                                                      \
  _a = _p[0];                                                               \
  _b = _p[1];                                                               \
  _q[0] = _p[3];                                                            \
  _q[1] = _p[2];                                                            \
  _q[3] = _a;                                                               \
  _q[2] = _b;                                                               \
}

enum { FILE_NOT_FOUND = -1, BAD_BYTECODE  = -2 };

extern int Big_endian;

void is_big_endian(void);

static void fixup_endianness_trailer(uint32 * p);

int caml_attempt_open(char **name, struct exec_trailer *trail,
                      int do_open_script);

void caml_read_section_descriptors(int fd, struct exec_trailer *trail);

int32 caml_seek_optional_section(int fd, struct exec_trailer *trail, char *name);

int32 caml_seek_section(int fd, struct exec_trailer *trail, char *name);

static char * read_section(int fd, struct exec_trailer *trail, char *name);


#endif
