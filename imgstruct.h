#ifndef DIP_IMGSTRUCT_H
#define DIP_IMGSTRUCT_H
typedef struct tagBITMAPFILEHEADER {
    [[maybe_unused]] unsigned short bfType;
    unsigned long bfSize;
    [[maybe_unused]] unsigned short bfReserved1;
    [[maybe_unused]] unsigned short bfReserved2;
    unsigned long bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
    [[maybe_unused]] unsigned long biSize;
    long biWidth;
    long biHeight;
    [[maybe_unused]] unsigned short biPlanes;
    unsigned short biBitCount;
    [[maybe_unused]] unsigned long biCompression;
    unsigned long biSizeImage;
    [[maybe_unused]] long biXPelsPerMeter;
    [[maybe_unused]] long biYPelsPerMeter;
    [[maybe_unused]] unsigned long biClrUsed;
    [[maybe_unused]] unsigned long biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagRGBQUAD {
    [[maybe_unused]] unsigned char rgbBlue;
    [[maybe_unused]] unsigned char rgbGreen;
    [[maybe_unused]] unsigned char rgbRed;
    [[maybe_unused]] unsigned char rgbReserved;
} RGBQUAD;
#endif //DIP_IMGSTRUCT_H
