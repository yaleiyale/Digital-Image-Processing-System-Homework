#ifndef DIP_OPENIMG_H
#define DIP_OPENIMG_H
#pragma pack(2)

#include <iostream>
#include "imgstruct.h"
typedef struct tagIMGINFO {
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    RGBQUAD pRGB[256];
    unsigned char *img;
    int actual_width;
} IMGINFO;

IMGINFO openImg(char *filename) {
    IMGINFO imgInfo;
    FILE *bmp;
    fopen_s(&bmp, filename, "rb");
    //读入文件头
    fread(&imgInfo.fileHeader, sizeof(BITMAPFILEHEADER), 1, bmp);
    //读入信息头
    fread(&imgInfo.infoHeader, sizeof(BITMAPINFOHEADER), 1, bmp);
    //读入调色板
    if (imgInfo.infoHeader.biBitCount == 8) {
        fread(&imgInfo.pRGB, sizeof(RGBQUAD), 256, bmp);
    }
    int bmpWidth = imgInfo.infoHeader.biWidth * (imgInfo.infoHeader.biBitCount / 8);
    //宽度转换
    while (bmpWidth % 4 != 0) {
        bmpWidth++;
    }
    imgInfo.actual_width = bmpWidth;
    //偏移量
    int bmpOffset = (int) imgInfo.fileHeader.bfOffBits;
    //指针跳转到图像数据开始位
    fseek(bmp, bmpOffset, 0);
    //读入原图数据
    auto *img = new unsigned char[imgInfo.infoHeader.biSizeImage];
    fread(img, sizeof(unsigned char), imgInfo.infoHeader.biSizeImage, bmp);
    imgInfo.img = img;
    fclose(bmp);
    return imgInfo;
}

#endif //DIP_OPENIMG_H
