/** @file
  Here we override some platform functions that cannot be re-defined in macro.
  SPDX-License-Identifier: WTFPL
**/

#include "ftstdlib.h"
#include "ftdebug.h"
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>

extern EFI_SYSTEM_TABLE  *gST;
extern EFI_BOOT_SERVICES *gBS;

FT_BASE( void )
ft_debug_init( void )
{
  // We do not need to init.
}

FT_Error
EFIAPI
FtLogEdk2(FT_Error Level, char* Format,...)
{
  VA_LIST va;
  VA_START (va,Format);
  DebugVPrint(DEBUG_INFO,Format,va);
  VA_END (va);
  (VOID) Level;
  return -1;
}
FT_Error
EFIAPI
FtErrEdk2   (char *Format,...)
{
  VA_LIST va;
  VA_START (va,Format);
  DebugVPrint(DEBUG_ERROR,Format,va);
  VA_END (va);
  return -1;
}
FT_Error
EFIAPI
FtPanicEdk2 (char *Format,...)
{
  VA_LIST va;
  VA_START (va,Format);
  DebugVPrint(DEBUG_ERROR,Format,va);
  VA_END (va);
  return -1;
}

VOID*
Edk2Calloc (size_t num, size_t size)
{
  //The EDK2's Memory Allocation Library DO NOT cache the Memory size.
  //For this reason, we MUST cache them ourselves.
  UINT64 *Buffer = AllocateZeroPool(num*size+sizeof(UINT64));
  if(Buffer == NULL) {
      return Buffer;
  }
  Buffer[0] = size;
  return &Buffer[1];
}

VOID
Edk2Free (void *ptr)
{
  UINT64 *Buffer = ptr;
  FreePool((VOID*)(Buffer-1));
}

VOID*
Edk2Malloc (size_t size)
{
  //The EDK2's Memory Allocation Library DO NOT cache the Memory size.
  //For this reason, we MUST cache them ourselves.
  UINT64 *Buffer = AllocatePool(size+sizeof(UINT64));
  if(Buffer == NULL) {
      return Buffer;
  }
  Buffer[0] = size;
  return &Buffer[1];
}

VOID*
Edk2Realloc (void *ptr, size_t new_size)
{
  //The EDK2's Memory Allocation Library DO NOT cache the Memory size.
  //For this reason, we MUST load the cache from our own.
  UINT64 *Buffer = ptr;
  Buffer --;
  Buffer = ReallocatePool((UINTN)Buffer[0]+sizeof(UINT64),new_size+sizeof(UINT64),Buffer);
  if(Buffer == NULL) {
      return Buffer;
  }
  Buffer[0] = new_size;
  return &Buffer[1];
}

void
ft_qsort (
  void *base,
  size_t num,
  size_t width,
  long long EFIAPI ( *compare )(const void *, const void *)
  )
{
  PerformQuickSort (base, (UINTN)num, (UINTN)width,compare);
  return;
}
