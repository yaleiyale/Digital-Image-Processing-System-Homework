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
    int imgSize;
} IMGINFO;

IMGINFO openImg(char *filename) {
    IMGINFO imgInfo;
    FILE *bmp;
    fopen_s(&bmp, filename, "rb");
    //�����ļ�ͷ
    fread(&imgInfo.fileHeader, sizeof(BITMAPFILEHEADER), 1, bmp);
    //������Ϣͷ
    fread(&imgInfo.infoHeader, sizeof(BITMAPINFOHEADER), 1, bmp);
    //�����ɫ��
    if (imgInfo.infoHeader.biBitCount == 8) {
        fread(&imgInfo.pRGB, sizeof(RGBQUAD), 256, bmp);
    }
    int bmpHeight = imgInfo.infoHeader.biHeight;
    int bmpWidth = imgInfo.infoHeader.biWidth * imgInfo.infoHeader.biBitCount / 8;
    //���ת��
    while (bmpWidth % 4 != 0) {
        bmpWidth++;
    }
    imgInfo.imgSize = bmpHeight * bmpWidth;
    //ƫ����
    int bmpOffset = (int) imgInfo.fileHeader.bfOffBits;
    //ָ����ת��ͼ�����ݿ�ʼλ
    fseek(bmp, bmpOffset, 0);
    //����ԭͼ����
    auto *img = new unsigned char[imgInfo.imgSize];
    fread(img, sizeof(unsigned char), imgInfo.imgSize, bmp);
    imgInfo.img = img;
    fclose(bmp);
    return imgInfo;
}

#endif //DIP_OPENIMG_H
