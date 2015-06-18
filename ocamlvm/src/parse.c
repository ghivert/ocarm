#include "parse.h"

int Big_endian;
/* Check for big or little endian */
void is_big_endian(void) {
  union {
    int i; // TODO : remplacer int par un int32
    char c[4];
  } bint = { 0x01020304 };
  Big_endian = (bint.c[0] != 1);
}

static void fixup_endianness_trailer(uint32 * p)
{
  if (Big_endian)
    Reverse_32(p, p);
}


static int read_trailer(int fd, struct exec_trailer *trail)
{
  if (lseek(fd, (long) -TRAILER_SIZE, SEEK_END) == -1) // offset 12 bytes avant la fin
    return BAD_BYTECODE;
  if (read(fd, (char *) trail, TRAILER_SIZE) < TRAILER_SIZE) // lit 12 bytes et les met dans trail
    return BAD_BYTECODE;
  fixup_endianness_trailer(&trail->num_sections); // inverse trail si nécessaire
  if (strncmp(trail->magic, EXEC_MAGIC, 12) == 0)
    return 0;
  else
    return BAD_BYTECODE;
}

int caml_attempt_open(char **name, struct exec_trailer *trail,
                      int do_open_script)
{
  char * truename;
  int fd;
  int err;
  char buf [2];

  //truename = caml_search_exe_in_path(*name);
  //*name = truename;
  truename = *name;
  fprintf(stderr, "Opening bytecode executable %s\n", truename);
  fd = open(truename, O_RDONLY | O_BINARY);
  if (fd == -1) {
    fprintf(stderr, "Cannot open file\n", 0);
    return FILE_NOT_FOUND;
  }
  if (!do_open_script) {
    err = read (fd, buf, 2);
    if (err < 2 || (buf [0] == '#' && buf [1] == '!')) {
      close(fd);
      fprintf(stderr, "Rejected #! script\n", 0);
      return BAD_BYTECODE;
    }
  }
  err = read_trailer(fd, trail);
  if (err != 0) {
    close(fd);
    fprintf(stderr, "Not a bytecode executable\n", 0);
    return err;
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
  if (read(fd, (char *) trail->section, toc_size) != toc_size)
    fprintf(stderr, "Fatal error: cannot read section table\n"); fflush(stderr);
  /* Fixup endianness of lengths */
  for (i = 0; i < trail->num_sections; i++)
    fixup_endianness_trailer(&(trail->section[i].len));
}

/* Position fd at the beginning of the section having the given name.
   Return the length of the section data in bytes, or -1 if no section
   found with that name. */

int32 caml_seek_optional_section(int fd, struct exec_trailer *trail, char *name)
{
  long ofs;
  int i;

  ofs = TRAILER_SIZE + trail->num_sections * 8;
  for (i = trail->num_sections - 1; i >= 0; i--) {
    ofs += trail->section[i].len;
    if (strncmp(trail->section[i].name, name, 4) == 0) {
      lseek(fd, -ofs, SEEK_END);
      return trail->section[i].len;
    }
  }
  return -1;
}

/* Position fd at the beginning of the section having the given name.
   Return the length of the section data in bytes. */

int32 caml_seek_section(int fd, struct exec_trailer *trail, char *name)
{
  int32 len = caml_seek_optional_section(fd, trail, name);
  if (len == -1)
    fprintf(stderr,"Fatal_error: section `%s' is missing\n", name);
  return len;
}

/* Read and return the contents of the section having the given name.
   Add a terminating 0.  Return NULL if no such section. */

static char * read_section(int fd, struct exec_trailer *trail, char *name)
{
  int32 len;
  char * data;

  len = caml_seek_optional_section(fd, trail, name);
  if (len == -1) return NULL;
  data = malloc(len + 1);
  if (read(fd, data, len) != len)
    fprintf(stderr, "Fatal error: error reading section %s\n", name);
  data[len] = 0;
  return data;
}

int main (int argc, char* argv[]) {

  struct exec_trailer trail;
  char * exe_name;
  int fd;
  
  exe_name = argv[1]; // bytecode file must be the 1st arg (-> argv[1] )
  fd = caml_attempt_open(&exe_name, &trail, 1);

  /* Read the table of contents (section descriptors) */
  caml_read_section_descriptors(fd, &trail);

  return 0;
}
