/***********************************************************************/
/*                                                                     */
/*                                OCaml                                */
/*                                                                     */
/*         Xavier Leroy and Damien Doligez, INRIA Rocquencourt         */
/*                                                                     */
/*  Copyright 1996 Institut National de Recherche en Informatique et   */
/*  en Automatique.  All rights reserved.  This file is distributed    */
/*  under the terms of the GNU Library General Public License, with    */
/*  the special exception on linking described in file ../LICENSE.     */
/*                                                                     */
/***********************************************************************/

#ifndef CAML_CONFIG_H
#define CAML_CONFIG_H

/* <include ../config/m.h> */
/* <include ../config/s.h> */
/* <private> */
#include "../../config/m.h"
#include "../../config/s.h"
/* </private> */

#ifndef CAML_NAME_SPACE
#include "compatibility.h"
#endif

#define SIZEOF_INT  4
typedef int int32;
typedef unsigned int uint32;
#define ARCH_INT32_PRINTF_FORMAT ""


#define SIZEOF_LONG  8
#define ARCH_INT64_TYPE long
#define ARCH_UINT64_TYPE unsigned long
#define ARCH_INT64_PRINTF_FORMAT "l"

typedef ARCH_INT64_TYPE int64;
typedef ARCH_UINT64_TYPE uint64;

#define SIZEOF_PTR  SIZEOF_INT
typedef int intnat;
typedef unsigned int uintnat;
#define ARCH_INTNAT_PRINTF_FORMAT ""

/* Endianness of floats */

/* ARCH_FLOAT_ENDIANNESS encodes the byte order of doubles as follows:
   the value [0xabcdefgh] means that the least significant byte of the
   float is at byte offset [a], the next lsb at [b], ..., and the
   most significant byte at [h]. */

#if defined(__arm__) && !defined(__ARM_EABI__)
#define ARCH_FLOAT_ENDIANNESS 0x45670123
#elif defined(ARCH_BIG_ENDIAN)
#define ARCH_FLOAT_ENDIANNESS 0x76543210
#else
#define ARCH_FLOAT_ENDIANNESS 0x01234567
#endif

/* We use threaded code interpretation if the compiler provides labels
   as first-class values (GCC 2.x). */

#if defined(__GNUC__) && __GNUC__ >= 2 && !defined(DEBUG) \
    && !defined (SHRINKED_GNUC) && !defined(CAML_JIT)
#define THREADED_CODE
#endif


/* Memory model parameters */

/* Initial size of stack (bytes). */
#define Stack_size (1024 * sizeof(value))

/* Minimum free size of stack (bytes); below that, it is reallocated. */
#define Stack_threshold (64 * sizeof(value))

/* Default maximum size of the stack (words). */
#define Max_stack_def 8192


/* Maximum size of a block allocated in the young generation (words). */
/* Must be > 4 */
#define Max_young_wosize 256


#endif /* CAML_CONFIG_H */
