/** @file
  The text renderer using FreeType2.
  SPDX-License-Identifier: WTFPL
**/

#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>

#include <Library/FontLib.h>

#include <freetype/freetype.h>
#include <freetype/ftglyph.h>

extern FT_Face    Face;
extern double     ScaleFactor;//In GopComposerLib

// Plain or accelerated basic functions.
extern
VOID
SetTransparency
(
  UINT32       *Buffer,
  UINTN         BufferSize,
  CONST UINT8  *Value
);
extern
VOID
SetMemInt32
(
  UINT32       *Buffer,
  CONST UINTN   BufferSize,
  CONST UINT32  Value
);

EFI_STATUS
EFIAPI
RenderText
(
  IN CONST CHAR16   *Text,
  IN UINT32          FontSize,
  IN UINT32          Color,
  OUT UINT32       **Buffer,
  OUT UINT32        *BufferWidth,
  OUT UINT32        *BufferHeight
  //TODO: Auto-wrap policy
)
{
  FT_Error        Error;
  UINT32          GlyphNumber;
  UINT32          Width=0, Height=0;
  INT32           HeightAboveBaseline=0, HeightBelowBaseline=0;
  UINTN           TextLen = StrLen(Text);
  UINTN          *CharPositions = AllocatePool(TextLen*sizeof(UINTN));
  UINTN          *FontMarginToBaseline = AllocatePool(TextLen*sizeof(UINTN));
  FT_BitmapGlyph *Glyphs = AllocatePool(TextLen*sizeof(FT_BitmapGlyph *));
  Error = FT_Set_Char_Size(
          Face,             /* handle to face object         */
          0,                /* char_width in 1/64 of points  */
          FontSize*64,      /* char_height in 1/64 of points */
          96*ScaleFactor,   /* horizontal device resolution  */
          96*ScaleFactor);  /* vertical device resolution    */
  if(Error) {
    DEBUG ((DEBUG_ERROR,"Cannot set font size!\n"));
    return EFI_UNSUPPORTED;
  }
  for(UINTN i=0;i<TextLen;i++) {
    GlyphNumber = FT_Get_Char_Index(Face,Text[i]);
    if(GlyphNumber) { // GlyphNumber==0 means there is no such glyph.
      Error = FT_Load_Glyph(Face,GlyphNumber,FT_LOAD_RENDER);
      FT_Get_Glyph(Face->glyph,(FT_Glyph*)&Glyphs[i]);
      if(Error) {
        DEBUG ((DEBUG_ERROR,"Cannot load character %c(%d)!\n",Text[i],GlyphNumber));
        return EFI_UNSUPPORTED;
      }
      FontMarginToBaseline[i]=Face->glyph->metrics.horiBearingY;
      if(Face->glyph->metrics.horiBearingY>HeightAboveBaseline) {
        HeightAboveBaseline = Face->glyph->metrics.horiBearingY;
      }
      if(Face->glyph->metrics.height-Face->glyph->metrics.horiBearingY>HeightBelowBaseline) {
        HeightBelowBaseline = Face->glyph->metrics.height-Face->glyph->metrics.horiBearingY;
      }
      CharPositions[i] = Width;
      if(Text[i+1]!='\0' && Text[i+1]!='\n') {
        Width += (UINT32)(Face->glyph->metrics.horiAdvance/64+Face->glyph->bitmap_left);
      }
      else {
        Width += (UINT32)(Face->glyph->metrics.width/64+Face->glyph->bitmap_left);
      }
    }
  }
  Height = (HeightAboveBaseline+HeightBelowBaseline)/64;
  *Buffer = AllocatePool(Width*Height*sizeof(UINT32));
  if(!*Buffer) {
    DEBUG ((DEBUG_ERROR,"Cannot allocate buffer for image!\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  SetMem32(*Buffer,Width*Height*sizeof(UINT32),Color&0x00FFFFFF);
  *BufferWidth  = Width;
  *BufferHeight = Height;
  UINT32 PaintPos=0;
  for(UINTN i=0;i<TextLen;i++) {
    UINT32 HeightMargin = (HeightAboveBaseline-FontMarginToBaseline[i])/64;
    PaintPos = CharPositions[i];
    DEBUG ((DEBUG_ERROR,"Char %c:Advance %d,Left %d,Width %d\n",Text[i],CharPositions[i],Glyphs[i]->left,Glyphs[i]->bitmap.width));
    for(UINTN j=0;j<Glyphs[i]->bitmap.rows;j++) {
      SetTransparency(*Buffer+PaintPos+Glyphs[i]->left+((HeightMargin+j)*Width),Glyphs[i]->bitmap.width,&Glyphs[i]->bitmap.buffer[j*Glyphs[i]->bitmap.width]);
    }
    FT_Done_Glyph((FT_Glyph)Glyphs[i]);
  }
  FreePool(CharPositions);
  FreePool(Glyphs);
  FreePool(FontMarginToBaseline);
  return EFI_SUCCESS;
}
