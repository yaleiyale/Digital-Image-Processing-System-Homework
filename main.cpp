#pragma pack(2)

#include "handle.h"

const double PI = acos(-1.0);

void menu() {
    std::cout << "数字图像处理" << "\n" << "输入数字选择功能" << "\n" << "1:24位分色" << "\n" << "2:8位灰度化" << "\n" << "3:8位反色" << "\n"
              << "4:绘制直方图" << "\n" << "5:直方图均衡化" << "\n" << "6:平均滤波" << "\n" << "7:中值滤波" << "\n" << "8:缩放" << "\n"
              << "9:平移" << "\n" << "10:镜像" << "\n" << "11:旋转" << "\n";
}

int main() {
    menu();
    int choice;
    int windows_size;
    int deltax,deltay;
    double angle;
    char filename[50];
    for (;;) {

        std::cin >> choice;
        switch (choice) {
            case 1:
                sprintf_s(filename, R"(..\resources\1.1\test%d%d.bmp)", choice / 10, choice % 10);
                ColorSeparation(filename);
                system("explorer ..\\resources\\1.1");
                break;
            case 2:
                sprintf_s(filename, R"(..\resources\1.2\test%d%d.bmp)", choice / 10, choice % 10);
                Grayscale(filename);
                system("explorer ..\\resources\\1.2");
                break;
            case 3:
                sprintf_s(filename, R"(..\resources\1.3\test%d%d.bmp)", choice / 10, choice % 10);
                Invert(filename);
                system("explorer ..\\resources\\1.3");
                break;
            case 4:
                sprintf_s(filename, R"(..\resources\2.1\test%d%d.bmp)", choice / 10, choice % 10);
                Histogram(filename);
                system("explorer ..\\resources\\2.1");
                break;
            case 5:
                sprintf_s(filename, R"(..\resources\2.2\test%d%d.bmp)", choice / 10, choice % 10);
                Equalization(filename);
                system("explorer ..\\resources\\2.2");
                break;
            case 6:
                std::cout << "输入窗口大小（奇数）" << "\n";
                std::cin >> windows_size;
                sprintf_s(filename, R"(..\resources\3.1\test%d%d.bmp)", choice / 10, choice % 10);
                AverageTreatment(filename, windows_size);
                system("explorer ..\\resources\\3.1");
                break;
            case 7:
                std::cout << "输入窗口大小（奇数）" << "\n";
                std::cin >> windows_size;
                sprintf_s(filename, R"(..\resources\3.2\test%d%d.bmp)", choice / 10, choice % 10);
                MedianFiltering(filename, windows_size);
                system("explorer ..\\resources\\3.2");
                break;
            case 8:
                sprintf_s(filename, R"(..\resources\4.1\test%d%d.bmp)", choice / 10, choice % 10);
                Zoom(filename, deltax);
                system("explorer ..\\resources\\4.1");
                break;
            case 9:
                std::cout << "输入水平偏移量";
                std::cin >> deltax;
                std::cout << "输入垂直偏移量";
                std::cin >> deltay;
                sprintf_s(filename, R"(..\resources\4.2\test%d%d.bmp)", choice / 10, choice % 10);
                Translation(filename, deltax,deltay);
                system("explorer ..\\resources\\4.2");
                break;
            case 10:
                sprintf_s(filename, R"(..\resources\4.3\test%d%d.bmp)", choice / 10, choice % 10);
                Horizontal_Mirror(filename);
                Vertical_Mirror(filename);
                system("explorer ..\\resources\\4.3");
                break;
            case 11:
                std::cout << "输入旋转角度";
                std::cin >> angle;
                sprintf_s(filename, R"(..\resources\4.4\test%d%d.bmp)", choice / 10, choice % 10);
                Rotate(filename, angle * PI / 180);
                system("explorer ..\\resources\\4.4");
                break;
            case -1:
                return 0;
            default:
                break;
        }
    }
}
