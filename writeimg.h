#ifndef DIP_WRITEIMG_H
#define DIP_WRITEIMG_H

#include "imgstruct.h"
#include <iostream>

void
write(BITMAPFILEHEADER fileHeader, BITMAPINFOHEADER infoHeader, unsigned char *img, const char *filename, int size) {
    FILE *file;
    fopen_s(&file, filename, "wb");
    fwrite(&fileHeader, sizeof(fileHeader), 1, file);
    fwrite(&infoHeader, sizeof(infoHeader), 1, file);
    fwrite(img, sizeof(unsigned char), size, file);
    fclose(file);
}

void
write(BITMAPFILEHEADER fileHeader, BITMAPINFOHEADER infoHeader, RGBQUAD *pRGB, unsigned char *img, const char *filename,
      int size) {
    FILE *file;
    fopen_s(&file, filename, "wb");
    fwrite(&fileHeader, sizeof(fileHeader), 1, file);
    fwrite(&infoHeader, sizeof(infoHeader), 1, file);
    fwrite(pRGB, sizeof(RGBQUAD), 256, file);
    fwrite(img, sizeof(unsigned char), size, file);
    fclose(file);
}

#endif //DIP_WRITEIMG_H
