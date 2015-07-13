#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "strings.h"

/**
 * Return the size of a string_array. The number of strings in the array.
 *
 * @param string_array The string_array which we want to know the size.
 * @return The size of string_array.
 */
size_t size_strar(char ** str_ar) {
  size_t count = 0;
  for (int i = 0 ; str_ar[i][0] != '\0' ; i++) {
    count++;
  }
  return count;
}

/**
 * Split a string with delimitor delim and return an array of strings.
 *
 * @param string The string to split.
 * @param delim The delimitor.
 * @return A string array.
 */
strar strsplit(char * str, char delim) {
  strar strres;
  size_t count = 0;

  for (unsigned int i = 0 ; i < strlen(str) ; i++) {
    if (str[i] == delim || str[i] == '\n' || str[i] == '\0')
      count++;
  }
  strres = malloc(++count * sizeof *strres);

  char buffer[100]; int actual = 0, place = 0;
  for (unsigned int i = 0 ; i < strlen(str) ; i++) {
    if (str[i] != delim && str[i] != '\n' && str[i] != '\0') {
      buffer[actual++] = str[i];
    }
    else {
      buffer[actual] = '\0';
      actual = 0;
      strres[place] = malloc((strlen(buffer) + 3) * sizeof *strres[place]);
      strres[place] = strcpy(strres[place], buffer);
      place++;
    }
  }

  strres[place] = malloc(sizeof *strres[place]);
  strres[place][0] = '\0';

  return strres;
}

/**
 * Free an entire string_array.
 *
 * @param strar The string_array to free.
 */
void free_strar(char ** str_ar) {
  for (unsigned int i = 0 ; i < size_strar(str_ar) ; i++) {
    free(str_ar[i]);
  }
  free(str_ar);
}
