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

#ifdef HAS_STDINT_H
#include <stdint.h>
#endif

/* Types for 32-bit integers, 64-bit integers, and
   native integers (as wide as a pointer type) */

#ifndef ARCH_INT32_TYPE
#if SIZEOF_INT == 4
#define ARCH_INT32_TYPE int
#define ARCH_UINT32_TYPE unsigned int
#define ARCH_INT32_PRINTF_FORMAT ""
#elif SIZEOF_LONG == 4
#define ARCH_INT32_TYPE long
#define ARCH_UINT32_TYPE unsigned long
#define ARCH_INT32_PRINTF_FORMAT "l"
#elif SIZEOF_SHORT == 4
#define ARCH_INT32_TYPE short
#define ARCH_UINT32_TYPE unsigned short
#define ARCH_INT32_PRINTF_FORMAT ""
#else
#error "No 32-bit integer type available"
#endif
#endif

#ifndef ARCH_INT64_TYPE
#if SIZEOF_LONGLONG == 8
#define ARCH_INT64_TYPE long long
#define ARCH_UINT64_TYPE unsigned long long
#define ARCH_INT64_PRINTF_FORMAT "ll"
#elif SIZEOF_LONG == 8
#define ARCH_INT64_TYPE long
#define ARCH_UINT64_TYPE unsigned long
#define ARCH_INT64_PRINTF_FORMAT "l"
#else
#error "No 64-bit integer type available"
#endif
#endif

#ifndef HAS_STDINT_H
/* Not a C99 compiler, typically MSVC.  Define the C99 types we use. */
typedef ARCH_INT32_TYPE int32_t;
typedef ARCH_UINT32_TYPE uint32_t;
typedef ARCH_INT64_TYPE int64_t;
typedef ARCH_UINT64_TYPE uint64_t;
#endif
typedef int int32;
typedef unsigned int uint32;
typedef ARCH_INT64_TYPE int64;
typedef ARCH_UINT64_TYPE uint64;

#if SIZEOF_PTR == SIZEOF_LONG
/* Standard models: ILP32 or I32LP64 */
typedef long intnat;
typedef unsigned long uintnat;
#define ARCH_INTNAT_PRINTF_FORMAT "l"
#elif SIZEOF_PTR == SIZEOF_INT
/* Hypothetical IP32L64 model */
typedef int intnat;
typedef unsigned int uintnat;
#define ARCH_INTNAT_PRINTF_FORMAT ""
#elif SIZEOF_PTR == 8
/* Win64 model: IL32P64 */
typedef int64_t intnat;
typedef uint64_t uintnat;
#define ARCH_INTNAT_PRINTF_FORMAT ARCH_INT64_PRINTF_FORMAT
#else
#error "No integer type available to represent pointers"
#endif

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


/* Memory model parameters */

/* Initial size of stack (bytes). */
#define Stack_size (512 * sizeof(value))

/* Minimum free size of stack (bytes); below that, it is reallocated. */
#define Stack_threshold (64 * sizeof(value))

/* Default maximum size of the stack (words). */
#define Max_stack_def 1024

/* Size (constant) of the heap (words)*/
#define Heap_size 4096


/* Maximum size of a block allocated in the young generation (words). */
/* Must be > 4 */
#define Max_young_wosize 1024


#endif /* CAML_CONFIG_H */
