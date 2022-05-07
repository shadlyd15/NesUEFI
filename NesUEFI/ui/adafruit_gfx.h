#ifndef _ADAFRUIT_GFX_H
#define _ADAFRUIT_GFX_H

// #include "types.h"
#include <efi.h>
#include <efilib.h>
#include <math.h>
#include <stddef.h>
#include <stdbool.h>
#include "gfxfont.h"



#ifndef pgm_read_byte
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif
#ifndef pgm_read_word
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif
#ifndef pgm_read_dword
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#endif

/// A generic graphics superclass that can handle all sorts of drawing. At a
/// minimum you can subclass and provide drawPixel(). At a maximum you can do a
/// ton of overriding to optimize. Used for any/all Adafruit displays!

  void adafruit_gfx_init(INT16 w, INT16 h); // Constructor

  /**********************************************************************/
  /*!
    @brief  Draw to the screen/framebuffer/etc.
    Must be overridden in subclass.
    @param  x    X coordinate in pixels
    @param  y    Y coordinate in pixels
    @param color  16-bit pixel color.
  */
  /**********************************************************************/
   void drawPixel(INT16 x, INT16 y, UINT32 color);

  // TRANSACTION API / CORE DRAW API
  // These MAY be overridden by the subclass to provide device-specific
  // optimized code.  Otherwise 'generic' versions are used.
   void startWrite(void);
   void writePixel(INT16 x, INT16 y, UINT32 color);
   void writeFillRect(INT16 x, INT16 y, INT16 w, INT16 h,
                             UINT32 color);
   void writeFastVLine(INT16 x, INT16 y, INT16 h, UINT32 color);
   void writeFastHLine(INT16 x, INT16 y, INT16 w, UINT32 color);
   void writeLine(INT16 x0, INT16 y0, INT16 x1, INT16 y1,
                         UINT32 color);
   void endWrite(void);

  // CONTROL API
  // These MAY be overridden by the subclass to provide device-specific
  // optimized code.  Otherwise 'generic' versions are used.
   void setRotation(UINT8 r);
   void invertDisplay(bool i);

  // BASIC DRAW API


   void drawFastVLine(INT16 x, INT16 y, INT16 h, UINT32 color);
   void drawFastHLine(INT16 x, INT16 y, INT16 w, UINT32 color);
   void fillRect(INT16 x, INT16 y, INT16 w, INT16 h,
                        UINT32 color);
   void fillScreen(UINT32 color);
  // Optional and probably not necessary to change
   void drawLine(INT16 x0, INT16 y0, INT16 x1, INT16 y1,
                        UINT32 color);
   void drawRect(INT16 x, INT16 y, INT16 w, INT16 h,
                        UINT32 color);

  // These exist only with Adafruit_GFX (no subclass overrides)
  void drawCircle(INT16 x0, INT16 y0, INT16 r, UINT32 color);
  void drawCircleHelper(INT16 x0, INT16 y0, INT16 r, UINT8 cornername,
                        UINT32 color);
  void fillCircle(INT16 x0, INT16 y0, INT16 r, UINT32 color);
  void fillCircleHelper(INT16 x0, INT16 y0, INT16 r, UINT8 cornername,
                        INT16 delta, UINT32 color);
  void drawTriangle(INT16 x0, INT16 y0, INT16 x1, INT16 y1, INT16 x2,
                    INT16 y2, UINT32 color);
  void fillTriangle(INT16 x0, INT16 y0, INT16 x1, INT16 y1, INT16 x2,
                    INT16 y2, UINT32 color);
  void drawRoundRect(INT16 x0, INT16 y0, INT16 w, INT16 h,
                     INT16 radius, UINT32 color);
  void fillRoundRect(INT16 x0, INT16 y0, INT16 w, INT16 h,
                     INT16 radius, UINT32 color);

  void drawBitmap(INT16 x, INT16 y, UINT8 *bitmap, INT16 w, INT16 h, UINT32 color, UINT32 bg);

  void drawXBitmap(INT16 x, INT16 y, const UINT8 bitmap[], INT16 w,
                   INT16 h, UINT32 color);

  void drawGrayscaleBitmap(INT16 x, INT16 y, UINT8 *bitmap, UINT8 *mask, INT16 w, INT16 h);

  void drawRGBBitmap(INT16 x, INT16 y, UINT32 *bitmap, UINT8 *mask,
                     INT16 w, INT16 h);

  void drawChar(INT16 x, INT16 y, unsigned char c, UINT32 color,
                UINT32 bg, UINT8 size_x, UINT8 size_y);
  void getTextBounds(const char *string, INT16 x, INT16 y, INT16 *x1,
                     INT16 *y1, UINT32 *w, UINT32 *h);

  size_t write(UINT8 c);
  void setTextSize(UINT8 s);
  void setFont(const GFXfont *f);

  void setCursor(INT16 x, INT16 y);

  void setTextColor(UINT32 c);
  // void setTextColor(UINT32 c, UINT32 bg);
  void setTextWrap(bool w);
  void cp437(bool x);
  INT16 width(void);
  INT16 height(void);
  UINT8 getRotation(void);
  INT16 getCursorX(void);
  INT16 getCursorY(void);
  void charBounds(unsigned char c, INT16 *x, INT16 *y, INT16 *minx,
                  INT16 *miny, INT16 *maxx, INT16 *maxy);

void print(char *s);
void print_16(UINT16 *s);
#endif // _ADAFRUIT_GFX_H
