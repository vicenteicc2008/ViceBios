/** @file
  The Wrapper file that calls FreeType2.
  SPDX-License-Identifier: WTFPL
**/

#include <Library/FontLib.h>
#include <freetype/freetype.h>
#include <Library/DebugLib.h>
#include <Theme.h>

extern EFI_BOOT_SERVICES *gBS;
extern EFI_SYSTEM_TABLE  *gST;

FT_Face    Face;
FT_Library Library;

EFI_STATUS
EFIAPI
PrepareFont
(
  VOID
)
{
  FT_Error Status;
  // Loads FreeType Library.
  Status = FT_Init_FreeType(&Library);
  if(Status) {
    DEBUG ((DEBUG_ERROR,"Cannot init FreeType Library!\n"));
    gST->StdErr->OutputString(gST->StdErr,L"Cannot init FreeType Library!\n");
    return EFI_UNSUPPORTED;
  }
  // Loads the font.
  Status = FT_New_Memory_Face(Library,FontFile,FontSize,0,&Face);
  if(Status) {
    DEBUG ((DEBUG_ERROR,"Cannot open font file!\n"));
    gST->StdErr->OutputString(gST->StdErr,L"Cannot open font file!\n");
    return EFI_UNSUPPORTED;
  }
  DEBUG ((DEBUG_INFO,"Font file open done!\n"));
  // Select Unicode charmap.
  Status = FT_Select_Charmap(Face, FT_ENCODING_UNICODE);
  if(Status) {
    DEBUG ((DEBUG_ERROR,"Font does not support Unicode!\n"));
    return EFI_UNSUPPORTED;
  }
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
DestroyFont
(
  VOID
)
{
  FT_Error Status;
  Status = FT_Done_FreeType(Library);
  if(Status) {
    DEBUG ((DEBUG_INFO,"Cannot destroy FreeType Library!\n"));
    gST->StdErr->OutputString(gST->StdErr,L"Cannot destroy FreeType Library!\n");
    return EFI_UNSUPPORTED;
  }
  return EFI_SUCCESS;
}
