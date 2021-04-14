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

void quickSort(int a[], int l, int r) {
    if (l >= r)
        return;
    int i = l;
    int j = r;
    int key = a[l];//选择第一个数为key
    while (i < j) {
        while (i < j && a[j] >= key)//从右向左找第一个小于key的值
            j--;
        if (i < j) {
            a[i] = a[j];
            i++;
        }
        while (i < j && a[i] < key)//从左向右找第一个大于key的值
            i++;
        if (i < j) {
            a[j] = a[i];
            j--;
        }
    }
    a[i] = key;
    quickSort(a, l, i - 1);//继续排左部分，递归调用
    quickSort(a, i + 1, r);//继续排右部分，递归调用
}

//奇数个数据求中间值
int getMiddle(const int *arr, int count) {
    int *copy = (int *) malloc(sizeof(int) * count);
    for (int i = 0; i < count; i++) {
        copy[i] = arr[i];
    }
    quickSort(copy, 0, count - 1);
    return copy[count / 2 + 1];
}

//24位分色
void ColorSeparation(char *filename) {
    IMGINFO imgInfo = openImg(filename);
    //分色图像区
    auto *img_r = new unsigned char[imgInfo.infoHeader.biSizeImage];
    auto *img_g = new unsigned char[imgInfo.infoHeader.biSizeImage];
    auto *img_b = new unsigned char[imgInfo.infoHeader.biSizeImage];
    //分离
    for (int i = 0; i < imgInfo.infoHeader.biHeight; i++) {
        for (int j = 0; j < imgInfo.actual_width; j++) {
            //按BGR取存到内存中
            switch (j % 3) {
                //B区
                case 0:
                    img_r[i * imgInfo.actual_width + j] = 0;
                    img_g[i * imgInfo.actual_width + j] = 0;
                    img_b[i * imgInfo.actual_width + j] = imgInfo.img[i * imgInfo.actual_width + j];
                    break;
                    //G区
                case 1:
                    img_r[i * imgInfo.actual_width + j] = 0;
                    img_g[i * imgInfo.actual_width + j] = imgInfo.img[i * imgInfo.actual_width + j];
                    img_b[i * imgInfo.actual_width + j] = 0;
                    break;
                    //R区
                case 2:
                    img_r[i * imgInfo.actual_width + j] = imgInfo.img[i * imgInfo.actual_width + j];
                    img_g[i * imgInfo.actual_width + j] = 0;
                    img_b[i * imgInfo.actual_width + j] = 0;
                    break;
            }
        }
    }
    write(imgInfo.fileHeader, imgInfo.infoHeader, img_r, R"(..\resources\1.1\r.bmp)", imgInfo.infoHeader.biSizeImage);
    write(imgInfo.fileHeader, imgInfo.infoHeader, img_g, R"(..\resources\1.1\g.bmp)", imgInfo.infoHeader.biSizeImage);
    write(imgInfo.fileHeader, imgInfo.infoHeader, img_b, R"(..\resources\1.1\b.bmp)", imgInfo.infoHeader.biSizeImage);
    delete[](img_r);
    delete[](img_g);
    delete[](img_b);
}

//24位灰度化
void Grayscale(char *filename) {
    IMGINFO imgInfo = openImg(filename);
    int new_width = (imgInfo.infoHeader.biWidth + 3) / 4 * 4;
    int new_size = imgInfo.infoHeader.biHeight * new_width;
    auto *img_grey = new unsigned char[new_size];
    for (int i = 0; i < imgInfo.infoHeader.biHeight; i++) {
        for (int j = 1; j <= imgInfo.infoHeader.biWidth * (imgInfo.infoHeader.biBitCount / 8); j++) {
            if (j % 3 == 0) {
                double temp = imgInfo.img[i * imgInfo.actual_width + j - 3] * 0.114 +
                              imgInfo.img[i * imgInfo.actual_width + j - 2] * 0.587 +
                              imgInfo.img[i * imgInfo.actual_width + j - 1] * 0.299;
                img_grey[i * new_width + j / 3 - 1] = (unsigned char) temp;
            }
        }
    }
    RGBQUAD pRGB[256];
    //写入调色板数据
    for (int i = 0; i < 256; i++) {
        pRGB[i].rgbBlue = pRGB[i].rgbGreen = pRGB[i].rgbRed = i;
        pRGB[i].rgbReserved = 0;
    }
    imgInfo.fileHeader.bfOffBits += sizeof(RGBQUAD) * 256;
    imgInfo.infoHeader.biBitCount = 8;
    imgInfo.infoHeader.biSizeImage = new_size;
    imgInfo.fileHeader.bfSize = 54 + imgInfo.infoHeader.biSizeImage;
    write(imgInfo.fileHeader, imgInfo.infoHeader, pRGB, img_grey, R"(..\resources\1.2\grey.bmp)", new_size);
    delete[](img_grey);
}

//8位反色
void Invert(char *filename) {
    IMGINFO imgInfo = openImg(filename);
    auto *invert_img = new unsigned char[imgInfo.infoHeader.biSizeImage];
    for (int i = 0; i < imgInfo.infoHeader.biHeight; i++) {
        for (int j = 0; j < imgInfo.infoHeader.biWidth; j++) {
            invert_img[i * imgInfo.actual_width + j] = 255 - imgInfo.img[i * imgInfo.actual_width + j];
        }
    }
    write(imgInfo.fileHeader, imgInfo.infoHeader, imgInfo.pRGB, invert_img, R"(..\resources\1.3\invert.bmp)",
          imgInfo.infoHeader.biSizeImage);
    delete[](invert_img);
}

//直方图
void Histogram(char *filename) {
    int count[256] = {0};
    IMGINFO imgInfo = openImg(filename);

    for (int i = 0; i < imgInfo.infoHeader.biHeight; i++) {
        for (int j = 0; j < imgInfo.infoHeader.biWidth * (imgInfo.infoHeader.biBitCount / 8); j++) {
            int now = imgInfo.img[i * imgInfo.actual_width + j];
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
    ih.biSizeImage = ih.biWidth * ih.biHeight * 3;
    ih.biXPelsPerMeter = 4724;
    ih.biYPelsPerMeter = 4724;
    ih.biClrUsed = clrUsed;
    ih.biClrImportant = 0;
    auto *show_img = new unsigned char[ih.biSizeImage];
    //绘制直方图
    for (int j = 0; j < ih.biWidth * 3; j += 3) {
        int i = 0;
        while (count[j / 3] > 0) {
            show_img[i * ih.biWidth * 3 + j] = 0;
            show_img[i * ih.biWidth * 3 + j + 1] = 0;
            show_img[i * ih.biWidth * 3 + j + 2] = 0;
            count[j / 3]--;
            i++;
        }
        while (i < ih.biHeight) {
            show_img[i * ih.biWidth * 3 + j] = 255;
            show_img[i * ih.biWidth * 3 + j + 1] = 255;
            show_img[i * ih.biWidth * 3 + j + 2] = 255;
            i++;
        }
    }
    write(fh, ih, show_img, R"(..\resources\2.1\Histogram.bmp)", ih.biSizeImage);
    delete[](show_img);
}

//均衡化
void Equalization(char *filename) {
    char origin[50] = R"(..\resources\2.2\origin.bmp)";
    IMGINFO origin_img = openImg(origin);
    IMGINFO imgInfo = openImg(filename);
    //当前直方图解析
    int count[256] = {0};
    for (int i = 0; i < imgInfo.infoHeader.biWidth * 3; i += 3) {
        for (int j = 0; j < imgInfo.infoHeader.biHeight; j++) {
            if ((imgInfo.img[j * imgInfo.infoHeader.biWidth * 3 + i] == 0) &
                (imgInfo.img[j * imgInfo.infoHeader.biWidth * 3 + i + 1] == 0) &&
                (imgInfo.img[j * imgInfo.infoHeader.biWidth * 3 + i + 2] == 0)) {
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
        temp[k] = (int) (255 * pi[k] + 1 - 0.5);
    }

    for (int k = 0; k < origin_img.infoHeader.biSizeImage; k++) {
        int colour = origin_img.img[k];
        origin_img.img[k] = temp[colour];
    }

    int new_count[256] = {0};//新直方图计数
    for (int k = 0; k < 256; k++) {
        new_count[temp[k]] += (int) (sum * p[k]);
    }
    if (origin_img.infoHeader.biBitCount == 8) {
        write(origin_img.fileHeader, origin_img.infoHeader, origin_img.pRGB, origin_img.img,
              R"(..\resources\2.2\new_img.bmp)", origin_img.infoHeader.biSizeImage);
    } else if (origin_img.infoHeader.biBitCount == 24) {
        write(origin_img.fileHeader, origin_img.infoHeader, origin_img.img, R"(..\resources\2.2\new_img.bmp)",
              origin_img.infoHeader.biSizeImage);
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
    ih.biSizeImage = ih.biWidth * ih.biHeight * 3;
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
    delete[](show_img);
}

//平均处理
void AverageTreatment(char *filename, int windows_size) {
    int depth = windows_size / 2;
    IMGINFO imgInfo = openImg(filename);
    auto *show_img = new unsigned char[imgInfo.infoHeader.biSizeImage];
    int step = imgInfo.infoHeader.biBitCount / 8;
    //忽略式
    for (int i = 0; i < imgInfo.infoHeader.biHeight; i++) {
        for (int j = 0; j < imgInfo.actual_width; j++) {

            //边界处理
            if (i <= depth - 1 || i >= imgInfo.infoHeader.biHeight - depth || j <= depth - 1 ||
                j >= (imgInfo.infoHeader.biWidth - 1) * (imgInfo.infoHeader.biBitCount / 8)) {
                show_img[i * imgInfo.actual_width + j] = imgInfo.img[i * imgInfo.actual_width + j];
            }
                //中间区域
            else {
                int temp = 0;
                for (int width_pointer = -depth; width_pointer <= depth; width_pointer++) {
                    for (int height_pointer = -depth; height_pointer <= depth; height_pointer++) {
                        temp += imgInfo.img[(i + width_pointer) * imgInfo.actual_width + j + (step * height_pointer)];
                    }
                }
                show_img[i * imgInfo.actual_width + j] = temp / (windows_size * windows_size);
            }
        }
    }
    if (imgInfo.infoHeader.biBitCount == 8) {
        write(imgInfo.fileHeader, imgInfo.infoHeader, imgInfo.pRGB, show_img,
              R"(..\resources\3.1\AverageTreatment1.bmp)", imgInfo.infoHeader.biSizeImage);
    } else if (imgInfo.infoHeader.biBitCount == 24) {
        write(imgInfo.fileHeader, imgInfo.infoHeader, show_img, R"(..\resources\3.1\AverageTreatment1.bmp)",
              imgInfo.infoHeader.biSizeImage);
    }
    //扩充式
    for (int i = 0; i < imgInfo.infoHeader.biHeight; i++) {
        for (int j = 0; j < imgInfo.actual_width; j++) {
            int temp = 0;
            int x, y;
            if (i <= depth - 1 || i >= imgInfo.infoHeader.biHeight - depth || j <= depth - 1 ||
                j >= (imgInfo.infoHeader.biWidth - 1) * (imgInfo.infoHeader.biBitCount / 8)) {
                for (int width_pointer = -depth; width_pointer <= depth; width_pointer++) {
                    for (int height_pointer = -depth; height_pointer <= depth; height_pointer++) {
                        x = j + (step * height_pointer);
                        y = i + width_pointer;
                        if (y < 0) {
                            y = 0;
                        }
                        if (y > imgInfo.infoHeader.biHeight - 1) {
                            y = imgInfo.infoHeader.biHeight - 1;
                        }
                        if (x < 0) {
                            x = 0;
                        }

                        if (x > ((imgInfo.infoHeader.biWidth * imgInfo.infoHeader.biBitCount / 8) - 1)) {
                            x = ((imgInfo.infoHeader.biWidth * imgInfo.infoHeader.biBitCount / 8) - 1);
                        }
                        temp += imgInfo.img[y * imgInfo.actual_width + x];
                    }
                }
                show_img[i * imgInfo.actual_width + j] = temp / (windows_size * windows_size);
            } else {
                for (int width_pointer = -depth; width_pointer <= depth; width_pointer++) {
                    for (int height_pointer = -depth; height_pointer <= depth; height_pointer++) {
                        temp += imgInfo.img[(i + width_pointer) * imgInfo.actual_width + j + (step * height_pointer)];
                    }
                }
                show_img[i * imgInfo.actual_width + j] = temp / (windows_size * windows_size);
            }
        }
    }
    if (imgInfo.infoHeader.biBitCount == 8) {
        write(imgInfo.fileHeader, imgInfo.infoHeader, imgInfo.pRGB, show_img,
              R"(..\resources\3.1\AverageTreatment2.bmp)", imgInfo.infoHeader.biSizeImage);
    } else if (imgInfo.infoHeader.biBitCount == 24) {
        write(imgInfo.fileHeader, imgInfo.infoHeader, show_img, R"(..\resources\3.1\AverageTreatment2.bmp)",
              imgInfo.infoHeader.biSizeImage);
    }
    delete[](show_img);
}

//中值滤波
void MedianFiltering(char *filename, int windows_size) {
    int depth = windows_size / 2;
    IMGINFO imgInfo = openImg(filename);
    auto *show_img = new unsigned char[imgInfo.infoHeader.biSizeImage];
    int step = imgInfo.infoHeader.biBitCount / 8;
    int *temp = (int *) malloc(sizeof(int) * windows_size * windows_size);
    //忽略式
    for (int i = 0; i < imgInfo.infoHeader.biHeight; i++) {
        for (int j = 0; j < imgInfo.actual_width; j++) {
            if (i <= depth - 1 || i >= imgInfo.infoHeader.biHeight - depth || j <= depth - 1 ||
                j >= (imgInfo.infoHeader.biWidth - 1) * (imgInfo.infoHeader.biBitCount / 8)) {
                show_img[i * imgInfo.actual_width + j] = imgInfo.img[i * imgInfo.actual_width + j];
            } else {
                int k = 0;
                for (int width_pointer = -depth; width_pointer <= depth; width_pointer++) {
                    for (int height_pointer = -depth; height_pointer <= depth; height_pointer++) {
                        temp[k] = imgInfo.img[(i + width_pointer) * imgInfo.actual_width + j + (step * height_pointer)];
                        k++;
                    }
                }
                show_img[i * imgInfo.actual_width + j] = getMiddle(temp, windows_size * windows_size);
            }
        }
    }
    if (imgInfo.infoHeader.biBitCount == 8) {
        write(imgInfo.fileHeader, imgInfo.infoHeader, imgInfo.pRGB, show_img,
              R"(..\resources\3.2\MedianFiltering1.bmp)", imgInfo.infoHeader.biSizeImage);
    } else if (imgInfo.infoHeader.biBitCount == 24) {
        write(imgInfo.fileHeader, imgInfo.infoHeader, show_img, R"(..\resources\3.2\MedianFiltering1.bmp)",
              imgInfo.infoHeader.biSizeImage);
    }
    //扩充式
    for (int i = 0; i < imgInfo.infoHeader.biHeight; i++) {
        for (int j = 0; j < imgInfo.actual_width; j++) {
            if (i <= depth - 1 || i >= imgInfo.infoHeader.biHeight - depth || j <= depth - 1 ||
                j >= (imgInfo.infoHeader.biWidth - 1) * (imgInfo.infoHeader.biBitCount / 8)) {
                int x, y;
                int k = 0;
                for (int width_pointer = -depth; width_pointer <= depth; width_pointer++) {
                    for (int height_pointer = -depth; height_pointer <= depth; height_pointer++) {

                        x = j + (step * height_pointer);
                        y = i + width_pointer;
                        if (y < 0) {
                            y = 0;
                        }
                        if (y > imgInfo.infoHeader.biHeight - 1) {
                            y = imgInfo.infoHeader.biHeight - 1;
                        }
                        if (x < 0) {
                            x = 0;
                        }

                        if (x > ((imgInfo.infoHeader.biWidth * imgInfo.infoHeader.biBitCount / 8) - 1)) {
                            x = ((imgInfo.infoHeader.biWidth * imgInfo.infoHeader.biBitCount / 8) - 1);
                        }
                        temp[k] = imgInfo.img[y * imgInfo.actual_width + x];
                        k++;
                    }
                }
                show_img[i * imgInfo.actual_width + j] = getMiddle(temp, windows_size * windows_size);
            } else {
                int k = 0;
                for (int width_pointer = -depth; width_pointer <= depth; width_pointer++) {
                    for (int height_pointer = -depth; height_pointer <= depth; height_pointer++) {
                        temp[k] = imgInfo.img[(i + width_pointer) * imgInfo.actual_width + j + (step * height_pointer)];
                        k++;
                    }
                }
                show_img[i * imgInfo.actual_width + j] = getMiddle(temp, windows_size * windows_size);
            }
        }
    }
    if (imgInfo.infoHeader.biBitCount == 8) {
        write(imgInfo.fileHeader, imgInfo.infoHeader, imgInfo.pRGB, show_img,
              R"(..\resources\3.2\MedianFiltering2.bmp)", imgInfo.infoHeader.biSizeImage);
    } else if (imgInfo.infoHeader.biBitCount == 24) {
        write(imgInfo.fileHeader, imgInfo.infoHeader, show_img, R"(..\resources\3.2\MedianFiltering2.bmp)",
              imgInfo.infoHeader.biSizeImage);
    }
    delete[](show_img);
}

//缩放
void Scale(char *filename, float scaleX, float scaleY) {

    IMGINFO imgInfo = openImg((filename));

    int zoom_width = (int) (scaleX * (float) imgInfo.infoHeader.biWidth);
    int zoom_height = (int) (scaleY * (float) imgInfo.infoHeader.biHeight);
    int new_width = (zoom_width * imgInfo.infoHeader.biBitCount / 8 + 3) / 4 * 4;
    int new_size = new_width * zoom_height;
    auto *zoom_img = new unsigned char[new_size];
    int x, y, new_x, new_y;
    for (int i = 0; i < zoom_height; i++) {
        for (int j = 0; j < new_width; j++) {
            new_y = i;
            new_x = j;
            y = (int) ((float) new_y / scaleY);
            x = (int) ((float) new_x / scaleX);
            if (y >= 0 && y < imgInfo.infoHeader.biHeight && x >= 0 &&
                x < imgInfo.actual_width)
                zoom_img[i * new_width + j] = imgInfo.img[y * imgInfo.actual_width +
                                                          x];

        }
    }
    imgInfo.fileHeader.bfSize = imgInfo.fileHeader.bfSize - imgInfo.infoHeader.biSizeImage + new_width * zoom_height;
    imgInfo.infoHeader.biHeight = zoom_height;
    imgInfo.infoHeader.biWidth = zoom_width;
    imgInfo.infoHeader.biSizeImage = new_width * zoom_height;

    if (imgInfo.infoHeader.biBitCount == 8) {
        write(imgInfo.fileHeader, imgInfo.infoHeader, imgInfo.pRGB, zoom_img, R"(..\resources\4.1\Zoom.bmp)",
              new_size);
    } else if (imgInfo.infoHeader.biBitCount == 24) {
        write(imgInfo.fileHeader, imgInfo.infoHeader, zoom_img, R"(..\resources\4.1\Zoom.bmp)",
              new_size);
    }
    delete[](zoom_img);
}

//平移
void Translation(char *filename, int deltaX, int deltaY) {
    IMGINFO imgInfo = openImg(filename);
    auto *translation_img = new unsigned char[imgInfo.infoHeader.biSizeImage];
    int time = imgInfo.infoHeader.biBitCount / 8;
    for (int i = 0; i < imgInfo.infoHeader.biHeight; i++) {
        for (int j = 0; j < imgInfo.actual_width; j++) {
            translation_img[i * imgInfo.actual_width + j] = 255;
        }
    }
    for (int i = 0; i < imgInfo.infoHeader.biHeight; i++) {
        for (int j = 0; j < imgInfo.actual_width; j++) {
            if (deltaX >= 0) {
                if (deltaY >= 0) {
                    if (i < imgInfo.infoHeader.biHeight - deltaY && j <= imgInfo.actual_width - deltaX * time)
                        translation_img[(i + deltaY) * imgInfo.actual_width + j + deltaX * time] = imgInfo.img[
                                i * imgInfo.actual_width + j];
                } else if (deltaY < 0) {
                    if (i >= -deltaY && j <= imgInfo.actual_width - deltaX * time)
                        translation_img[(i + deltaY) * imgInfo.actual_width + j + deltaX * time] = imgInfo.img[
                                i * imgInfo.actual_width + j];
                }
            } else if (deltaX < 0) {
                if (deltaY >= 0) {
                    if (i < imgInfo.infoHeader.biHeight - deltaY && j >= -deltaX * time)
                        translation_img[(i + deltaY) * imgInfo.actual_width + j + deltaX * time] = imgInfo.img[
                                i * imgInfo.actual_width + j];
                } else if (deltaY < 0) {
                    if (i >= -deltaY && j >= -deltaX * time)
                        translation_img[(i + deltaY) * imgInfo.actual_width + j + deltaX * time] = imgInfo.img[
                                i * imgInfo.actual_width + j];
                }
            }
        }
    }
    if (imgInfo.infoHeader.biBitCount == 8) {
        write(imgInfo.fileHeader, imgInfo.infoHeader, imgInfo.pRGB, translation_img,
              R"(..\resources\4.2\Translation.bmp)", imgInfo.infoHeader.biSizeImage);
    } else if (imgInfo.infoHeader.biBitCount == 24) {
        write(imgInfo.fileHeader, imgInfo.infoHeader, translation_img, R"(..\resources\4.2\Translation.bmp)",
              imgInfo.infoHeader.biSizeImage);
    }
    delete[](translation_img);
}

//水平镜像
void Horizontal_Mirror(char *filename) {
    IMGINFO imgInfo = openImg(filename);
    auto *mirror_img = new unsigned char[imgInfo.infoHeader.biSizeImage];
    int time = imgInfo.infoHeader.biBitCount / 8;
    for (int i = 0; i < imgInfo.infoHeader.biHeight; i++) {
        for (int j = 0; j < imgInfo.actual_width; j++) {
            mirror_img[i * imgInfo.actual_width + j] = 0;
        }
    }
    if (time == 3) {
        for (int i = 0; i < imgInfo.infoHeader.biHeight; i++) {
            for (int j = 0; j < imgInfo.infoHeader.biWidth * time; j++) {
                if (j % 3 == 1)
                    mirror_img[i * imgInfo.actual_width + j] = imgInfo.img[i * imgInfo.actual_width +
                                                                           imgInfo.infoHeader.biWidth * time - j - 1];
                else if (j % 3 == 0)
                    mirror_img[i * imgInfo.actual_width + j] = imgInfo.img[i * imgInfo.actual_width +
                                                                           imgInfo.infoHeader.biWidth * time - j - 3];
                else
                    mirror_img[i * imgInfo.actual_width + j] = imgInfo.img[i * imgInfo.actual_width +
                                                                           imgInfo.infoHeader.biWidth * time - j + 1];
            }
        }
    } else if (time == 1) {
        for (int i = 0; i < imgInfo.infoHeader.biHeight; i++) {
            for (int j = 0; j < imgInfo.infoHeader.biWidth * time; j++) {
                mirror_img[i * imgInfo.actual_width + j] = imgInfo.img[i * imgInfo.actual_width +
                                                                       imgInfo.infoHeader.biWidth * time - j - time];
            }
        }
    }
    if (imgInfo.infoHeader.biBitCount == 8) {
        write(imgInfo.fileHeader, imgInfo.infoHeader, imgInfo.pRGB, mirror_img,
              R"(..\resources\4.3\Mirror1.bmp)", imgInfo.infoHeader.biSizeImage);
    } else if (imgInfo.infoHeader.biBitCount == 24) {
        write(imgInfo.fileHeader, imgInfo.infoHeader, mirror_img, R"(..\resources\4.3\Mirror1.bmp)",
              imgInfo.infoHeader.biSizeImage);
    }
    delete[](mirror_img);
}

//垂直镜像
void Vertical_Mirror(char *filename) {
    IMGINFO imgInfo = openImg(filename);
    auto *mirror_img = new unsigned char[imgInfo.infoHeader.biSizeImage];
    int time = imgInfo.infoHeader.biBitCount / 8;
    for (int i = 0; i < imgInfo.infoHeader.biHeight; i++) {
        for (int j = 0; j < imgInfo.infoHeader.biWidth * time; j++) {
            mirror_img[i * imgInfo.actual_width + j] = imgInfo.img[
                    (imgInfo.infoHeader.biHeight - i - 1) * imgInfo.actual_width + j];
        }
    }
    if (imgInfo.infoHeader.biBitCount == 8) {
        write(imgInfo.fileHeader, imgInfo.infoHeader, imgInfo.pRGB, mirror_img,
              R"(..\resources\4.3\Mirror2.bmp)", imgInfo.infoHeader.biSizeImage);
    } else if (imgInfo.infoHeader.biBitCount == 24) {
        write(imgInfo.fileHeader, imgInfo.infoHeader, mirror_img, R"(..\resources\4.3\Mirror2.bmp)",
              imgInfo.infoHeader.biSizeImage);
    }
    delete[](mirror_img);
}

//旋转
void Rotate(char *filename, double angle) {
    IMGINFO imgInfo = openImg(filename);
    int img_width = ceil(abs(imgInfo.infoHeader.biHeight * sin(angle)) + abs(imgInfo.actual_width * cos(angle)));//新图像宽
    int img_height = ceil(abs(imgInfo.actual_width * sin(angle)) + abs(imgInfo.infoHeader.biHeight * cos(angle)));//新图像高
    int new_size = img_width * img_height;
    auto *rotate_img = new unsigned char[new_size];
    for (int i = 0; i < img_height; i++) {
        for (int j = 0; j < img_width; j++) {
            rotate_img[i * img_width + j] = 255;
        }
    }
    for (int i = 0; i < imgInfo.infoHeader.biHeight; i++) {
        for (int j = 0; j < imgInfo.actual_width; j++) {
            if ((i * cos(angle) - j * sin(angle)) >= 0 && (i * sin(angle) + j * cos(angle)) >= 0 &&
                imgInfo.infoHeader.biHeight >= (i * cos(angle) - j * sin(angle)) &&
                imgInfo.actual_width >= (i * sin(angle) + j * cos(angle)))
                rotate_img[(int) (i * cos(angle) - j * sin(angle)) * imgInfo.actual_width +
                           (int) (i * sin(angle) + j * cos(angle))] = imgInfo.img[
                        i * imgInfo.actual_width + j];
        }
    }
    write(imgInfo.fileHeader, imgInfo.infoHeader, imgInfo.pRGB, rotate_img,
          R"(..\resources\4.4\Rotate.bmp)", imgInfo.infoHeader.biSizeImage);
    delete[](rotate_img);
}

//绘制带标记的直方图
void LabeledHistogram(char *filename, const char *target, int alpha) {
    IMGINFO imgInfo = openImg(filename);
    int count[256] = {0};
    for (int i = 0; i < imgInfo.infoHeader.biHeight; i++) {
        for (int j = 0; j < imgInfo.infoHeader.biWidth * (imgInfo.infoHeader.biBitCount / 8); j++) {
            int now = imgInfo.img[i * imgInfo.actual_width + j];
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
    ih.biSizeImage = ih.biWidth * ih.biHeight * 3;
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
            if (j / 3 == alpha) {
                show_img[i * x + j] = 0;
                show_img[i * x + j + 1] = 0;
                show_img[i * x + j + 2] = 255;
            } else {
                show_img[i * x + j] = 0;
                show_img[i * x + j + 1] = 0;
                show_img[i * x + j + 2] = 0;
            }
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
    write(fh, ih, show_img, target, actual_size);
    delete[](show_img);
}

//图像二值化
void Binary(const unsigned char *img, unsigned char *temp_img, unsigned long size, int threshold) {
    for (int i = 0; i < size; i++)
        temp_img[i] = img[i] >= threshold ? 255 : 0;
}

//给定阈值分割
void FixedThresholdSegmentation(char *filename, int alpha) {
    IMGINFO imgInfo = openImg(filename);
    auto *temp_img = new unsigned char[imgInfo.infoHeader.biSizeImage];
    Binary(imgInfo.img, temp_img, imgInfo.infoHeader.biSizeImage, alpha);
    if (imgInfo.infoHeader.biBitCount == 8) {
        write(imgInfo.fileHeader, imgInfo.infoHeader, imgInfo.pRGB, temp_img,
              R"(..\resources\5.1\FixedThresholdSegmentation.bmp)", imgInfo.infoHeader.biSizeImage);
    } else if (imgInfo.infoHeader.biBitCount == 24) {
        write(imgInfo.fileHeader, imgInfo.infoHeader, temp_img,
              R"(..\resources\5.1\FixedThresholdSegmentation.bmp)",
              imgInfo.infoHeader.biSizeImage);
    }
    delete[](temp_img);
    LabeledHistogram(filename, R"(..\resources\5.1\Histogram.bmp)", alpha);
}

//迭代阈值分割
void IterationThresholdSegmentation(char *filename, int alpha) {
    IMGINFO imgInfo = openImg(filename);
    int count[256] = {0};
    int sum = imgInfo.infoHeader.biHeight * imgInfo.infoHeader.biWidth * (imgInfo.infoHeader.biBitCount / 8);
    int *copy = (int *) malloc(sizeof(int) * sum);
    for (int i = 0; i < imgInfo.infoHeader.biHeight; i++) {
        for (int j = 0; j < imgInfo.infoHeader.biWidth * (imgInfo.infoHeader.biBitCount / 8); j++)
            copy[i * imgInfo.infoHeader.biWidth + j] = imgInfo.img[i * imgInfo.infoHeader.biWidth + j];
    }
    //初始阈值
    int t1 = getMiddle(copy, sum);
    free(copy);
    //统计灰度计数
    for (int i = 0; i < imgInfo.infoHeader.biHeight; i++) {
        for (int j = 0; j < imgInfo.infoHeader.biWidth * (imgInfo.infoHeader.biBitCount / 8); j++) {
            int now = imgInfo.img[i * imgInfo.actual_width + j];
            count[now]++;
        }
    }
    int t2;
    bool bingo = false;
    //迭代计算阈值
    while (!bingo) {
        double pre_sum = 0, next_sum = 0, pre_count = 0, next_count = 0;
        for (int i = 0; i < 256; i++) {
            if (i <= t1) {
                pre_sum += i * count[i];
                pre_count += count[i];
            } else {
                next_sum += i * count[i];
                next_count += count[i];
            }
        }
        double u1 = pre_sum / pre_count;
        double u2 = next_sum / next_count;
        t2 = (int) (u1 + u2) / 2;
        if (abs(t2 - t1) > alpha) {
            t1 = t2;
        } else {
            bingo = true;
        }
    }
    auto *temp_img = new unsigned char[imgInfo.infoHeader.biSizeImage];
    Binary(imgInfo.img, temp_img, imgInfo.infoHeader.biSizeImage, t2);
    if (imgInfo.infoHeader.biBitCount == 8) {
        write(imgInfo.fileHeader, imgInfo.infoHeader, imgInfo.pRGB, temp_img,
              R"(..\resources\5.2\IterationThresholdSegmentation.bmp)", imgInfo.infoHeader.biSizeImage);
    } else if (imgInfo.infoHeader.biBitCount == 24) {
        write(imgInfo.fileHeader, imgInfo.infoHeader, temp_img,
              R"(..\resources\5.2\IterationThresholdSegmentation.bmp)",
              imgInfo.infoHeader.biSizeImage);
    }
    delete[](temp_img);
    LabeledHistogram(filename, R"(..\resources\5.2\Histogram.bmp)", t2);

}

//otsu
void otsu(char *filename) {
    IMGINFO imgInfo = openImg(filename);
    const int GrayScale = 256;
    int bmpWidth = imgInfo.infoHeader.biWidth * (imgInfo.infoHeader.biBitCount / 8);
    int bmpHeight = imgInfo.infoHeader.biHeight;
    int count[GrayScale] = {0};
    double p[GrayScale] = {0};
    int i, j, threshold = 0;
    float sum = (float) imgInfo.infoHeader.biWidth * (float) imgInfo.infoHeader.biHeight *
                ((float) imgInfo.infoHeader.biBitCount / 8);
    //统计灰度计数
    for (i = 0; i < bmpHeight; i++) {
        for (j = 0; j < bmpWidth; j++) {
            int now = imgInfo.img[i * imgInfo.actual_width + j];
            count[now]++;
        }
    }

    //计算每个像素在整幅图像中的比例
    double max_p = 0.0;
    for (i = 0; i < GrayScale; i++) {
        p[i] = (float) count[i] / sum;
        if (p[i] > max_p) {
            max_p = p[i];
        }
    }
    //遍历寻找最大类间方差
    double back_p, front_p, back_sum, front_sum, back_aver, front_aver, average, g, max_g = 0;
    for (i = 0; i < GrayScale; i++) {
        back_p = front_p = back_sum = front_sum = back_aver = front_aver = average = g = 0;
        for (j = 0; j < GrayScale; j++) {
            if (j <= i)   //背景部分
            {
                back_p += p[j];
                back_sum += j * p[j];
            } else   //前景部分
            {
                front_p += p[j];
                front_sum += j * p[j];
            }
        }
        back_aver = back_sum / back_p;
        front_aver = front_sum / front_p;
        average = back_sum + front_sum;
        g = back_p * pow((back_aver - average), 2) + front_p * pow((front_aver - average), 2);
        if (g > max_g) {
            max_g = g;
            threshold = i;
        }
    }
    auto *temp_img = new unsigned char[imgInfo.infoHeader.biSizeImage];
    Binary(imgInfo.img, temp_img, imgInfo.infoHeader.biSizeImage, threshold);
    if (imgInfo.infoHeader.biBitCount == 8) {
        write(imgInfo.fileHeader, imgInfo.infoHeader, imgInfo.pRGB, temp_img,
              R"(..\resources\5.3\otsu.bmp)", imgInfo.infoHeader.biSizeImage);
    } else if (imgInfo.infoHeader.biBitCount == 24) {
        write(imgInfo.fileHeader, imgInfo.infoHeader, temp_img,
              R"(..\resources\5.3\otsu.bmp)",
              imgInfo.infoHeader.biSizeImage);
    }
    delete[](temp_img);
    LabeledHistogram(filename, R"(..\resources\5.3\Histogram.bmp)", threshold);

}

#endif //DIP_HANDLE_H