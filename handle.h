#ifndef DIP_HANDLE_H
#define DIP_HANDLE_H

#include "writeimg.h"
#include "openimg.h"

int getMax(const int *arr, int count) {
    int temp = arr[0];
    for (int i = 1; i < count; i++) {
        if (temp < arr[i]) {
            temp = arr[i];
        }
    }
    return temp;

}

//奇数个数据求中间值
int getMiddle(const int *arr, int count) {
    int *copy = (int *) malloc(sizeof(int) * count);
    for (int i = 0; i < count; i++) {
        copy[i] = arr[i];
    }
    for (int i = 0; i < count - 1; i++) {
        if (copy[i] > copy[i + 1]) {
            int temp;
            temp = copy[i];
            copy[i] = copy[i + 1];
            copy[i + 1] = temp;
        }
    }
    return copy[count / 2] + 1;
}

//24位分色
void ColorSeparation(char *filename) {
    IMGINFO imginfo = openImg(filename);
    int bmpHeight = imginfo.infoHeader.biHeight;
    int bmpWidth = imginfo.imgSize / imginfo.infoHeader.biHeight;
    //分色图像区
    auto *img_r = new unsigned char[imginfo.imgSize];
    auto *img_g = new unsigned char[imginfo.imgSize];
    auto *img_b = new unsigned char[imginfo.imgSize];
    //分离
    for (int i = 0; i < bmpHeight; i++) {
        for (int j = 0; j < bmpWidth; j++) {
            //按BGR取存到内存中
            switch (j % 3) {
                //B区
                case 0:
                    img_r[i * bmpWidth + j] = 0;
                    img_g[i * bmpWidth + j] = 0;
                    img_b[i * bmpWidth + j] = imginfo.img[i * bmpWidth + j];
                    break;
                    //G区
                case 1:
                    img_r[i * bmpWidth + j] = 0;
                    img_g[i * bmpWidth + j] = imginfo.img[i * bmpWidth + j];
                    img_b[i * bmpWidth + j] = 0;
                    break;
                    //R区
                case 2:
                    img_r[i * bmpWidth + j] = imginfo.img[i * bmpWidth + j];
                    img_g[i * bmpWidth + j] = 0;
                    img_b[i * bmpWidth + j] = 0;
                    break;
            }
        }
    }
    write(imginfo.fileHeader, imginfo.infoHeader, img_r, R"(..\resources\1.1\r.bmp)", imginfo.imgSize);
    write(imginfo.fileHeader, imginfo.infoHeader, img_g, R"(..\resources\1.1\g.bmp)", imginfo.imgSize);
    write(imginfo.fileHeader, imginfo.infoHeader, img_b, R"(..\resources\1.1\b.bmp)", imginfo.imgSize);
}

//24位灰度化
void Grayscale(char *filename) {
    IMGINFO imgInfo = openImg(filename);
    int new_height = imgInfo.infoHeader.biHeight;
    int new_width = imgInfo.infoHeader.biWidth;
    while (new_width % 4 != 0) {
        new_width++;
    }
    int new_size = new_height * new_width;
    auto *img_grey = new unsigned char[new_size];
    int bmpHeight = imgInfo.infoHeader.biHeight;
    int bmpWidth = imgInfo.imgSize / imgInfo.infoHeader.biHeight;
    for (int i = 0; i < bmpHeight; i++) {
        for (int j = 1; j <= bmpWidth; j++) {
            if (j % 3 == 0) {
                double temp = imgInfo.img[i * bmpWidth + j - 3] * 0.114 + imgInfo.img[i * bmpWidth + j - 2] * 0.587 +
                              imgInfo.img[i * bmpWidth + j - 1] * 0.299;
                img_grey[i * new_width + j / 3 - 1] = temp;
            }
        }
    }
    RGBQUAD pRGB[256];
    //写入调色板数据
    for (int i = 0; i < 256; i++) {
        pRGB[i].rgbBlue = pRGB[i].rgbGreen = pRGB[i].rgbRed = i;
        pRGB[i].rgbReserved = 0;
    }
    imgInfo.fileHeader.bfSize -= 54;
    imgInfo.fileHeader.bfSize /= 3;
    imgInfo.fileHeader.bfSize += 54 + sizeof(RGBQUAD) * 256;
    imgInfo.fileHeader.bfOffBits += sizeof(RGBQUAD) * 256;
    imgInfo.infoHeader.biBitCount = 8;
    imgInfo.infoHeader.biSizeImage /= 3;
    write(imgInfo.fileHeader, imgInfo.infoHeader, pRGB, img_grey, R"(..\resources\1.2\grey.bmp)", new_size);
}

//8位反色
void Invert(char *filename) {
    IMGINFO imginfo = openImg(filename);
    int bmpHeight = imginfo.infoHeader.biHeight;
    int bmpWidth = imginfo.imgSize / imginfo.infoHeader.biHeight;
    auto *img_invert = new unsigned char[imginfo.imgSize];
    for (int i = 0; i < bmpHeight; i++) {
        for (int j = 0; j < bmpWidth; j++) {
            img_invert[i * bmpWidth + j] = 255 - imginfo.img[i * bmpWidth + j];
        }
    }
    write(imginfo.fileHeader, imginfo.infoHeader, imginfo.pRGB, img_invert, R"(..\resources\1.3\invert.bmp)",
          imginfo.imgSize);
}

//直方图
void Histogram(char *filename) {
    int count[256] = {0};
    IMGINFO imginfo = openImg(filename);
    int bmpHeight = imginfo.infoHeader.biHeight;
    int bmpWidth = imginfo.imgSize / imginfo.infoHeader.biHeight;
    for (int i = 0; i < bmpHeight; i++) {
        for (int j = 0; j < bmpWidth; j++) {
            int now = imginfo.img[i * bmpWidth + j];
            count[now]++;
        }
    }
    int k = 0;
    int clrUsed = 0;
    while (k < 256) {
        if (count[k] > 0) {
            clrUsed++;
        }
        k++;
    }
    //直方图文件头、信息头
    BITMAPFILEHEADER fh;
    BITMAPINFOHEADER ih;
    fh.bfType = 19778;
    fh.bfReserved1 = 0;
    fh.bfReserved2 = 0;
    fh.bfOffBits = 54;
    ih.biSize = 40;
    ih.biWidth = 256;
    ih.biHeight = getMax(count, 256) / 10 * 10 + 10;
    fh.bfSize = ih.biWidth * ih.biHeight * 3 + fh.bfOffBits;
    ih.biPlanes = 1;
    ih.biBitCount = 24;
    ih.biCompression = 0;
    ih.biSizeImage = fh.bfSize + 10;
    ih.biXPelsPerMeter = 4724;
    ih.biYPelsPerMeter = 4724;
    ih.biClrUsed = clrUsed;
    ih.biClrImportant = 0;
    int x = ih.biWidth * 3;
    int y = ih.biHeight;
    int actual_size = x * y;
    auto *show_img = new unsigned char[actual_size];
    //绘制直方图
    for (int j = 0; j < x; j += 3) {
        int i = 0;
        while (count[j / 3] > 0) {
            show_img[i * x + j] = 0;
            show_img[i * x + j + 1] = 0;
            show_img[i * x + j + 2] = 0;
            count[j / 3]--;
            i++;
        }
        while (i < y) {
            show_img[i * x + j] = 255;
            show_img[i * x + j + 1] = 255;
            show_img[i * x + j + 2] = 255;
            i++;
        }
    }
    write(fh, ih, show_img, R"(..\resources\2.1\Histogram.bmp)", actual_size);
}

//均衡化
void Equalization(char *filename) {
    IMGINFO imginfo = openImg(filename);
    int bmpWidth = imginfo.infoHeader.biWidth;
    int bmpHeight = imginfo.infoHeader.biHeight;
    //当前直方图解析
    int count[256] = {0};
    for (int i = 0; i < bmpWidth * 3; i += 3) {
        for (int j = 0; j < bmpHeight; j++) {
            if ((imginfo.img[j * bmpWidth * 3 + i] == 0) & (imginfo.img[j * bmpWidth * 3 + i + 1] == 0) &&
                (imginfo.img[j * bmpWidth * 3 + i + 2] == 0)) {
                count[i / 3]++;//每个灰度的统计量
            }
        }
    }
    double p[256] = {0};//每个灰度的概率
    double sum = 0.0;
    for (int k : count) {
        sum += k;//直方图反应的像素总量
    }
    for (int k = 0; k < 256; k++) {
        p[k] = ((double) count[k]) / ((double) sum);
    }
    double pi[256] = {0};//累加概率
    pi[0] = p[0];
    for (int k = 1; k < 256; k++) {
        pi[k] = pi[k - 1] + p[k];
    }
    int temp[256] = {0};
    for (int k = 0; k < 256; k++) {
        temp[k] = (int) (255 * pi[k] + 0.5);
    }
    int new_count[256] = {0};//新直方图计数
    for (int k = 0; k < 256; k++) {
        new_count[temp[k]] += (int) (sum * p[k]);
    }

    int clrUsed = 0;
    for (int k : count) {
        if (k > 0) {
            clrUsed++;
        }
    }
    BITMAPFILEHEADER fh;
    BITMAPINFOHEADER ih;
    fh.bfType = 19778;
    fh.bfReserved1 = 0;
    fh.bfReserved2 = 0;
    fh.bfOffBits = 54;
    ih.biSize = 40;
    ih.biWidth = 256;
    ih.biHeight = getMax(new_count, 256) / 10 * 10 + 10;
    fh.bfSize = ih.biWidth * ih.biHeight * 3 + fh.bfOffBits;
    ih.biPlanes = 1;
    ih.biBitCount = 24;
    ih.biCompression = 0;
    ih.biSizeImage = fh.bfSize + 10;
    ih.biXPelsPerMeter = 4724;
    ih.biYPelsPerMeter = 4724;
    ih.biClrUsed = clrUsed;
    ih.biClrImportant = 0;
    int x = ih.biWidth * 3;
    int y = ih.biHeight;
    int actual_size = x * y;
    auto *show_img = new unsigned char[actual_size];
    //绘制直方图
    for (int j = 0; j < x; j += 3) {
        int i = 0;
        while (new_count[j / 3] > 0) {
            show_img[i * x + j] = 0;
            show_img[i * x + j + 1] = 0;
            show_img[i * x + j + 2] = 0;
            new_count[j / 3]--;
            i++;
        }
        while (i < y) {
            show_img[i * x + j] = 255;
            show_img[i * x + j + 1] = 255;
            show_img[i * x + j + 2] = 255;
            i++;
        }
    }
    write(fh, ih, show_img, R"(..\resources\2.2\Equalization.bmp)", actual_size);
}

//平均处理
void AverageTreatment(char *filename, int windows_size) {
    int depth = windows_size / 2;
    IMGINFO imginfo = openImg(filename);
    int width = imginfo.imgSize / imginfo.infoHeader.biHeight;
    auto *show_img = new unsigned char[imginfo.imgSize];
    int step = imginfo.infoHeader.biBitCount / 8;
    //忽略式
    for (int i = 0; i < imginfo.infoHeader.biHeight; i++) {
        for (int j = 0; j < width; j++) {

            //边界处理
            if (i <= depth - 1 || i >= imginfo.infoHeader.biHeight - depth || j <= depth - 1 ||
                j >= (imginfo.infoHeader.biWidth) * (imginfo.infoHeader.biBitCount / 8) - depth) {
                show_img[i * width + j] = imginfo.img[i * width + j];
            }
                //中间区域
            else {
                int temp = 0;
                for (int width_pointer = -depth; width_pointer <= depth; width_pointer++) {
                    for (int height_pointer = -depth; height_pointer <= depth; height_pointer++) {
                        temp += imginfo.img[(i + width_pointer) * width + j + (step * height_pointer)];
                    }
                }
                show_img[i * width + j] = temp / (windows_size * windows_size);
            }
        }
    }
    if (imginfo.infoHeader.biBitCount == 8) {
        write(imginfo.fileHeader, imginfo.infoHeader, imginfo.pRGB, show_img,
              R"(..\resources\3.1\AverageTreatment1.bmp)", imginfo.imgSize);
    } else if (imginfo.infoHeader.biBitCount == 24) {
        write(imginfo.fileHeader, imginfo.infoHeader, show_img, R"(..\resources\3.1\AverageTreatment1.bmp)",
              imginfo.imgSize);
    }
    //扩充式
    for (int i = 0; i < imginfo.infoHeader.biHeight; i++) {
        for (int j = 0; j < width; j++) {
            if (i <= depth - 1 || i >= imginfo.infoHeader.biHeight - depth || j <= depth - 1 ||
                j >= (imginfo.infoHeader.biWidth) * (imginfo.infoHeader.biBitCount / 8) - depth) {

            } else {
                int temp = 0;
                for (int width_pointer = -depth; width_pointer <= depth; width_pointer++) {
                    for (int height_pointer = -depth; height_pointer <= depth; height_pointer++) {
                        temp += imginfo.img[(i + width_pointer) * width + j + (step * height_pointer)];
                    }
                }
                show_img[i * width + j] = temp / (windows_size * windows_size);
            }
        }
    }
    if (imginfo.infoHeader.biBitCount == 8) {
        write(imginfo.fileHeader, imginfo.infoHeader, imginfo.pRGB, show_img,
              R"(..\resources\3.1\AverageTreatment2.bmp)", imginfo.imgSize);
    } else if (imginfo.infoHeader.biBitCount == 24) {
        write(imginfo.fileHeader, imginfo.infoHeader, show_img, R"(..\resources\3.1\AverageTreatment2.bmp)",
              imginfo.imgSize);
    }
}

//中值滤波
void MedianFiltering(char *filename, int windows_size) {
    int depth = windows_size / 2;
    IMGINFO imginfo = openImg(filename);
    int width = imginfo.imgSize / imginfo.infoHeader.biHeight;
    auto *show_img = new unsigned char[imginfo.imgSize];
    int step = imginfo.infoHeader.biBitCount / 8;
    //忽略式
    for (int i = 0; i < imginfo.infoHeader.biHeight; i++) {
        int *temp = (int *) malloc(sizeof(int) * windows_size * windows_size);
        for (int j = 0; j < width; j++) {
            if (i <= depth - 1 || i >= imginfo.infoHeader.biHeight - depth || j <= depth - 1 ||
                j >= (imginfo.infoHeader.biWidth) * (imginfo.infoHeader.biBitCount / 8) - depth) {
                show_img[i * width + j] = imginfo.img[i * width + j];
            } else {
                int k = 0;
                for (int width_pointer = -depth; width_pointer <= depth; width_pointer++) {
                    for (int height_pointer = -depth; height_pointer <= depth; height_pointer++) {
                        temp[k] = imginfo.img[(i + width_pointer) * width + j + (step * height_pointer)];
                        k++;
                    }
                }
                show_img[i * width + j] = getMiddle(temp, windows_size * windows_size);
            }
        }
    }
    if (imginfo.infoHeader.biBitCount == 8) {
        write(imginfo.fileHeader, imginfo.infoHeader, imginfo.pRGB, show_img,
              R"(..\resources\3.2\MedianFiltering1.bmp)", imginfo.imgSize);
    } else if (imginfo.infoHeader.biBitCount == 24) {
        write(imginfo.fileHeader, imginfo.infoHeader, show_img, R"(..\resources\3.2\MedianFiltering1.bmp)",
              imginfo.imgSize);
    }
    //扩充式
    for (int i = 0; i < imginfo.infoHeader.biHeight; i++) {
        int *temp = (int *) malloc(sizeof(int) * 9);
        for (int j = 0; j < width; j++) {
            if (i == 0)//下界
            {
                if (j == 0)//左下
                {
                    temp[0] = imginfo.img[i * width + j];
                    temp[1] = imginfo.img[i * width + j + step];
                    temp[2] = imginfo.img[i * width + j + step];
                    temp[3] = imginfo.img[(i + 1) * width + j];
                    temp[4] = imginfo.img[(i + 1) * width + j];
                    temp[5] = imginfo.img[(i + 1) * width + j + step];
                    temp[6] = imginfo.img[(i + 1) * width + j + step];
                    temp[7] = imginfo.img[(i + 1) * width + j + step];
                    temp[8] = imginfo.img[(i + 1) * width + j + step];
                    show_img[i * width + j] = getMiddle(temp, 9);
                } else if (j == imginfo.infoHeader.biWidth - 1)//右下
                {
                    temp[0] = imginfo.img[i * width + j];
                    temp[1] = imginfo.img[i * width + j - step];
                    temp[2] = imginfo.img[i * width + j - step];
                    temp[3] = imginfo.img[(i + 1) * width + j];
                    temp[4] = imginfo.img[(i + 1) * width + j];
                    temp[5] = imginfo.img[(i + 1) * width + j - step];
                    temp[6] = imginfo.img[(i + 1) * width + j - step];
                    temp[7] = imginfo.img[(i + 1) * width + j - step];
                    temp[8] = imginfo.img[(i + 1) * width + j - step];
                    show_img[i * width + j] = getMiddle(temp, 9);
                } else {
                    temp[0] = imginfo.img[i * width + j];
                    temp[1] = imginfo.img[i * width + j - step];
                    temp[2] = imginfo.img[i * width + j + step];
                    temp[3] = imginfo.img[(i + 1) * width + j];
                    temp[4] = imginfo.img[(i + 1) * width + j];
                    temp[5] = imginfo.img[(i + 1) * width + j - step];
                    temp[6] = imginfo.img[(i + 1) * width + j - step];
                    temp[7] = imginfo.img[(i + 1) * width + j + step];
                    temp[8] = imginfo.img[(i + 1) * width + j + step];
                    show_img[i * width + j] = getMiddle(temp, 9);
                }

            } else if (i == imginfo.infoHeader.biHeight - 1)//上界
            {
                if (j == 0)//左上
                {
                    temp[0] = imginfo.img[i * width + j];
                    temp[1] = imginfo.img[i * width + j + step];
                    temp[2] = imginfo.img[i * width + j + step];
                    temp[3] = imginfo.img[(i - 1) * width + j];
                    temp[4] = imginfo.img[(i - 1) * width + j];
                    temp[5] = imginfo.img[(i - 1) * width + j + step];
                    temp[6] = imginfo.img[(i - 1) * width + j + step];
                    temp[7] = imginfo.img[(i - 1) * width + j + step];
                    temp[8] = imginfo.img[(i - 1) * width + j + step];
                    show_img[i * width + j] = getMiddle(temp, 9);
                } else if (j == imginfo.infoHeader.biWidth - 1)//右上
                {
                    temp[0] = imginfo.img[i * width + j];
                    temp[1] = imginfo.img[i * width + j - step];
                    temp[2] = imginfo.img[i * width + j - step];
                    temp[3] = imginfo.img[(i - 1) * width + j];
                    temp[4] = imginfo.img[(i - 1) * width + j];
                    temp[5] = imginfo.img[(i - 1) * width + j - step];
                    temp[6] = imginfo.img[(i - 1) * width + j - step];
                    temp[7] = imginfo.img[(i - 1) * width + j - step];
                    temp[8] = imginfo.img[(i - 1) * width + j - step];
                    show_img[i * width + j] = getMiddle(temp, 9);
                } else {
                    temp[0] = imginfo.img[i * width + j];
                    temp[1] = imginfo.img[i * width + j - step];
                    temp[2] = imginfo.img[i * width + j + step];
                    temp[3] = imginfo.img[(i - 1) * width + j];
                    temp[4] = imginfo.img[(i - 1) * width + j];
                    temp[5] = imginfo.img[(i - 1) * width + j - step];
                    temp[6] = imginfo.img[(i - 1) * width + j - step];
                    temp[7] = imginfo.img[(i - 1) * width + j + step];
                    temp[8] = imginfo.img[(i - 1) * width + j + step];
                    show_img[i * width + j] = getMiddle(temp, 9);
                }

            } else {//中间区域
                temp[0] = imginfo.img[i * width + j];
                temp[1] = imginfo.img[i * width + j - step];
                temp[2] = imginfo.img[i * width + j + step];
                temp[3] = imginfo.img[(i + 1) * width + j];
                temp[4] = imginfo.img[(i + 1) * width + j - step];
                temp[5] = imginfo.img[(i + 1) * width + j + step];
                temp[6] = imginfo.img[(i - 1) * width + j];
                temp[7] = imginfo.img[(i - 1) * width + j - step];
                temp[8] = imginfo.img[(i - 1) * width + j + step];
                show_img[i * width + j] = getMiddle(temp, 9);
            }
        }
    }
    if (imginfo.infoHeader.biBitCount == 8) {
        write(imginfo.fileHeader, imginfo.infoHeader, imginfo.pRGB, show_img,
              R"(..\resources\3.2\MedianFiltering2.bmp)", imginfo.imgSize);
    } else if (imginfo.infoHeader.biBitCount == 24) {
        write(imginfo.fileHeader, imginfo.infoHeader, show_img, R"(..\resources\3.2\MedianFiltering2.bmp)",
              imginfo.imgSize);
    }
}

#endif //DIP_HANDLE_H
