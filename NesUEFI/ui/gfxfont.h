// Font structures for newer Adafruit_GFX (1.1 and later).
// Example fonts are included in 'Fonts' directory.
// To use a font in your Arduino sketch, #include the corresponding .h
// file and pass address of GFXfont struct to setFont().  Pass NULL to
// revert to 'classic' fixed-space bitmap font.

#ifndef _GFXFONT_H_
#define _GFXFONT_H_

#include <efi.h>
#include <efilib.h>
/// Font data stored PER GLYPH
typedef struct {
  UINT32 bitmapOffset; ///< Pointer into GFXfont->bitmap
  UINT8 width;         ///< Bitmap dimensions in pixels
  UINT8 height;        ///< Bitmap dimensions in pixels
  UINT8 xAdvance;      ///< Distance to advance cursor (x axis)
  INT8 xOffset;        ///< X dist from cursor pos to UL corner
  INT8 yOffset;        ///< Y dist from cursor pos to UL corner
} GFXglyph;

/// Data stored for FONT AS A WHOLE
typedef struct {
  UINT8 *bitmap;  ///< Glyph bitmaps, concatenated
  GFXglyph *glyph;  ///< Glyph array
  UINT32 first;   ///< ASCII extents (first char)
  UINT32 last;    ///< ASCII extents (last char)
  UINT8 yAdvance; ///< Newline distance (y axis)
} GFXfont;

#endif // _GFXFONT_H_
