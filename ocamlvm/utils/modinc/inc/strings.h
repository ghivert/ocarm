#include <unistd.h>

/* Define the type strar, which represents an array of strings. */
#define strar char**

/* Return the size of an array of strings. */
size_t size_strar(char ** str_ar);

/* Split a string into an array of strings. */
strar strsplit(char * str, char delim);

/* Free an array of strings. */
void free_strar(char ** str_ar);
