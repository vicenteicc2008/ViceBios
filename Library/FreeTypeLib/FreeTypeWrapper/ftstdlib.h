/*
  Here we override the default FreeType's stdlib dependency since the complete Libc is not provided in edk2.
  SPDX-License-Identifier: WTFPL
 */
#ifndef FTSTDLIB_H_
#define FTSTDLIB_H_
#include <Uefi.h>
#include <Library/BaseLib.h>

#include <stddef.h>

#define ft_ptrdiff_t  ptrdiff_t


  /**************************************************************************
   *
   *                          integer limits
   *
   * `UINT_MAX` and `ULONG_MAX` are used to automatically compute the size of
   * `int` and `long` in bytes at compile-time.  So far, this works for all
   * platforms the library has been tested on.  We also check `ULLONG_MAX`
   * to see whether we can use 64-bit `long long` later on.
   *
   * Note that on the extremely rare platforms that do not provide integer
   * types that are _exactly_ 16 and 32~bits wide (e.g., some old Crays where
   * `int` is 36~bits), we do not make any guarantee about the correct
   * behaviour of FreeType~2 with all fonts.
   *
   * In these cases, `ftconfig.h` will refuse to compile anyway with a
   * message like 'couldn't find 32-bit type' or something similar.
   *
   */


#include <limits.h>

#define FT_CHAR_BIT    CHAR_BIT
#define FT_USHORT_MAX  USHRT_MAX
#define FT_INT_MAX     INT_MAX
#define FT_INT_MIN     INT_MIN
#define FT_UINT_MAX    UINT_MAX
#define FT_LONG_MIN    LONG_MIN
#define FT_LONG_MAX    LONG_MAX
#define FT_ULONG_MAX   ULONG_MAX
#ifdef LLONG_MAX
#define FT_LLONG_MAX   LLONG_MAX
#endif
#ifdef LLONG_MIN
#define FT_LLONG_MIN   LLONG_MIN
#endif
#ifdef ULLONG_MAX
#define FT_ULLONG_MAX  ULLONG_MAX
#endif


  /**************************************************************************
   *
   *                character and string processing
   *
   */


#include <Library/BaseMemoryLib.h>

#define ft_memchr(buf,ch,count)     ScanMem8(buf,count,ch)
#define ft_memcmp(dest,src,n)       CompareMem(dest,src,n)
#define ft_memcpy(dest,src,n)       CopyMem(dest,src,n)
#define ft_memmove(dest,src,n)      CopyMem(dest,src,n)  //EDK says CopyMem "must handle the case where SourceBuffer overlaps DestinationBuffer."
#define ft_memset(dest,ch,count)    SetMem(dest,count,ch)
#define ft_strcat   strcat
#define ft_strcmp   AsciiStrCmp
#define ft_strcpy   strcpy
#define ft_strlen   AsciiStrLen
#define ft_strncmp  AsciiStrnCmp
#define ft_strncpy  strncpy
#define ft_strrchr  strrchr
#define ft_strstr   AsciiStrStr


  /**************************************************************************
   *
   *                          file handling
   *
   */
// The EDK2 Build DO NOT support FILE operations!
#if 0

#include <stdio.h>

#define FT_FILE     FILE
#define ft_fclose   fclose
#define ft_fopen    fopen
#define ft_fread    fread
#define ft_fseek    fseek
#define ft_ftell    ftell
#define ft_sprintf  sprintf

#endif
  /**************************************************************************
   *
   *                            sorting
   *
   */

#include <Library/SortLib.h>

void
ft_qsort (
  void *base,
  size_t num,
  size_t width,
  long long EFIAPI ( *compare )(const void *, const void *)
  );


  /**************************************************************************
   *
   *                       memory allocation
   *
   */

VOID*
Edk2Calloc (size_t num, size_t size);

VOID
Edk2Free (void *ptr);

VOID*
Edk2Malloc (size_t size);

VOID*
Edk2Realloc (void *ptr, size_t new_size);


#define ft_scalloc   Edk2Calloc
#define ft_sfree     Edk2Free
#define ft_smalloc   Edk2Malloc
#define ft_srealloc  Edk2Realloc


  /**************************************************************************
   *
   *                         miscellaneous
   *
   */
// The EDK2 Build DO NOT support StrTol & GetEnv operations!
#if 0

#define ft_strtol  strtol
#define ft_getenv  getenv
#endif

  /**************************************************************************
   *
   *                        execution control
   *
   */


#include <setjmp.h>

#define ft_jmp_buf     jmp_buf  /* note: this cannot be a typedef since  */
                                /*       `jmp_buf` is defined as a macro */
                                /*       on certain platforms            */

#define ft_longjmp      __builtin_longjmp
#define ft_setjmp( b )  __builtin_setjmp( *(ft_jmp_buf*) &(b) ) /* same thing here */

#endif
