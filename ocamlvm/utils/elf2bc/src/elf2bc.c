#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

/* Data structures to read the bytecode file */
struct section_descriptor {
  char name[4];                 /* Section name */
  uint32_t len;                   /* Length of data in bytes */
};
struct exec_trailer {
  uint32_t num_sections;          /* Number of sections */
  char magic[12];               /* The magic number */
  struct section_descriptor * section; /* Not part of file */
};
#define TRAILER_SIZE (4+12)
#define EXEC_MAGIC "Caml1999X008"
/*********************************************/

#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifndef SEEK_END
#define SEEK_END 2
#endif

#define IS_BIG_ENDIAN (!(union { uint16_t u16; unsigned char c; }){ .u16 = 1 }.c)

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


/* Read the trailer of a bytecode file */

static void fixup_endianness_trailer(uint32_t * p)
{
  if (!IS_BIG_ENDIAN)
    Reverse_32(p, p);
}

static int read_trailer(int fd, struct exec_trailer *trail)
{
  if (lseek(fd, (long) -TRAILER_SIZE, SEEK_END) == -1) // offset 12 bytes avant la fin
    return 1;
  if (read(fd, (char *) trail, TRAILER_SIZE) < TRAILER_SIZE) // lit 12 bytes et les met dans trail
    return 1;
  fixup_endianness_trailer(&trail->num_sections); // inverse trail si nécessaire
  if (strncmp(trail->magic, EXEC_MAGIC, 12) == 0)
    return 0;
  else
    return 1;
}

int caml_attempt_open(char *name, struct exec_trailer *trail,
                      int do_open_script)
{
  int fd;
  int err;

  fd = open(name, O_RDONLY | O_BINARY);
  if (fd == -1) {
    fprintf(stderr, "Fatal error: can't open %s\n", name); 
    exit (1);
  }
  err = read_trailer(fd, trail);
  if (err != 0) {
    close(fd);
    fprintf(stderr, "Fatal error : format non reconnu\n");
    exit (1);
  }
  return fd;
}

/* Read the section descriptors */

void caml_read_section_descriptors(int fd, struct exec_trailer *trail)
{
  int toc_size, i;

  toc_size = trail->num_sections * 8;
  trail->section = malloc(toc_size);
  lseek(fd, - (long) (TRAILER_SIZE + toc_size), SEEK_END);
  if (read(fd, (char *) trail->section, toc_size) != toc_size) {
    fprintf(stderr, "Fatal error: cannot read section table\n");
    exit (1);
  }
  /* Fixup endianness of lengths */
  for (i = 0; i < trail->num_sections; i++)
    fixup_endianness_trailer(&(trail->section[i].len));
}



int main(int argc, char **argv)
{
  int fd;
  struct exec_trailer trail; // struct définie dans exec.h

  if (argc < 2) {
    fprintf(stderr, "Usage: ./getBytecode input_file <output_file>\n");
    exit (1);
  }

  /* Open file and read magic and number of section */
  fd = caml_attempt_open(argv[1], &trail, 0);
  /* Read the table of contents (section descriptors) */
  caml_read_section_descriptors(fd, &trail);
  
  int size_tot = 16;
  for (int i = 0; i < trail.num_sections; i++) {
    size_tot += trail.section[i].len + 8;
  }
  
  int fd_out;
  char* out_name;
  if (argc == 3)
    out_name = argv[2];
  else 
    out_name = strdup("a.out");

  fd_out = open(out_name, O_WRONLY | O_CREAT | O_BINARY, 0777);
  if (fd_out == -1) {
    fprintf(stderr, "Fatal error: can't open %s\n", out_name); 
    exit (1);
  }
  char* buffer = malloc(sizeof(size_tot));
  lseek(fd, (long) -size_tot, SEEK_END);
  if (read(fd, buffer, size_tot) != size_tot) {
    fprintf(stderr, "Fatal error: error while reading %s\n", argv[1]);
    exit (1);
  }
  write(fd_out, "#!/home/dada/.opam/4.01.0/bin/ocamlrun\n", 40);
  
  if (write(fd_out, buffer, size_tot) == -1) {
    fprintf(stderr, "Fatal error: error while writing in %s\n", out_name);
    exit (1);
  }
  
  fprintf(stderr, "Your bytecode is now in %s\n", out_name);

  return 0;
}
