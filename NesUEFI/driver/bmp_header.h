#ifndef BMP_HEADER
#define BMP_HEADER

typedef struct {
    CHAR8  CharB;
    CHAR8  CharM;
    UINT32 Size;
    UINT16 Reserved[2];
    UINT32 ImageOffset;
    UINT32 HeaderSize;
    UINT32 PixelWidth;
    UINT32 PixelHeight;
    UINT16 Planes;
    UINT16 BitPerPixel;
    UINT32 CompressionType;
    UINT32 ImageSize;
    UINT32 XPixelsPerMeter;
    UINT32 YPixelsPerMeter;
    UINT32 NumberOfColors;
    UINT32 ImportantColors;
} __attribute__((__packed__)) BMP_IMAGE_HEADER;

// Read more: https://blog.fpmurphy.com/2015/08/display-bmp-details-and-image-in-uefi-shell.html#ixzz7SHLiufWS

#endif