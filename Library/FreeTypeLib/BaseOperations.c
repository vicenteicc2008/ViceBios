/** @file
  Base operations related to FreeType operations, which may increase overall performance via assembly.
  This file contains plain C version.
  SPDX-License-Identifier: WTFPL
**/

#if (!defined MDE_CPU_X64)
#include <Uefi.h>

#define COLOR_BIT_MASK      0x00FFFFFF
#define ALPHA_BIT_SHIFT     24

VOID
SetMemInt32
(
  UINT32       *Buffer,
  CONST UINTN   BufferSize,
  CONST UINT32  Value
)
{
  for (UINTN i=0;i<BufferSize;i++) {
    Buffer[i] = Value;
  }
  return;
}

VOID
SetTransparency
(
  UINT32       *Buffer,
  UINTN         BufferSize,
  CONST UINT8  *Value
)
{
  for (UINTN i=0;i<BufferSize;i++) {
    UINT32 Transparency = Value[i] << ALPHA_BIT_SHIFT;
    Buffer[i] &= COLOR_BIT_MASK;
    Buffer[i] |= Transparency;
  }
  return;
}
#endif
