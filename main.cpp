#pragma pack(2)

#include "handle.h"

#define  PI  acos(-1.0)

void menu() {
    std::cout << "数字图像处理" << "\n" << "输入数字选择功能" << "\n" << "1:24位分色" << "\n" << "2:8位灰度化" << "\n" << "3:8位反色"
              << "\n" << "4:绘制直方图" << "\n" << "5:直方图均衡化" << "\n" << "6:平均滤波" << "\n" << "7:中值滤波" << "\n" << "8:缩放"
              << "\n"
              << "9:平移" << "\n" << "10:镜像" << "\n" << "11:旋转" << "\n" << "12:固定阈值分割" << "\n" << "13:迭代阈值分割" << "\n"
              << "14:大津法" << "\n" << "15:区域生长" << "\n" << "16:分裂合并" << "\n" << "17:Prewitt" << "\n" << "18:Sobel"
              << "\n" << "19:LOG" << "\n"
              << "20:直线检测" << "\n" << "21:连通域分析" << "\n" << "22:轮廓提取" << "\n";
}

int main() {

    int choice;
    int alpha;
    int deltaX, deltaY;
    float scaleX, scaleY;
    double angle;
    char filename[50];
    for (;;) {
        menu();
        std::cin >> choice;
        switch (choice) {
            case 1://分色
                sprintf_s(filename, R"(..\resources\1.1\test%d%d.bmp)", choice / 10, choice % 10);
                ColorSeparation(filename);
                system("explorer ..\\resources\\1.1");
                break;
            case 2://灰度化
                sprintf_s(filename, R"(..\resources\1.2\test%d%d.bmp)", choice / 10, choice % 10);
                Grayscale(filename);
                system("explorer ..\\resources\\1.2");
                break;
            case 3://反色
                sprintf_s(filename, R"(..\resources\1.3\test%d%d.bmp)", choice / 10, choice % 10);
                Invert(filename);
                system("explorer ..\\resources\\1.3");
                break;
            case 4://直方图
                sprintf_s(filename, R"(..\resources\2.1\test%d%d.bmp)", choice / 10, choice % 10);
                Histogram(filename);
                system("explorer ..\\resources\\2.1");
                break;
            case 5://均衡化
                sprintf_s(filename, R"(..\resources\2.2\test%d%d.bmp)", choice / 10, choice % 10);
                Equalization(filename);
                system("explorer ..\\resources\\2.2");
                break;
            case 6://平均降噪
                std::cout << "输入窗口大小（奇数）" << "\n";
                std::cin >> alpha;
                sprintf_s(filename, R"(..\resources\3.1\test%d%d.bmp)", choice / 10, choice % 10);
                AverageTreatment(filename, alpha);
                system("explorer ..\\resources\\3.1");
                break;
            case 7://中值降噪
                std::cout << "输入窗口大小（奇数）" << "\n";
                std::cin >> alpha;
                sprintf_s(filename, R"(..\resources\3.2\test%d%d.bmp)", choice / 10, choice % 10);
                MedianFiltering(filename, alpha);
                system("explorer ..\\resources\\3.2");
                break;
            case 8://缩放
                std::cout << "输入横向缩放比" << "\n";
                std::cin >> scaleX;
                std::cout << "输入纵向缩放比" << "\n";
                std::cin >> scaleY;
                sprintf_s(filename, R"(..\resources\4.1\test%d%d.bmp)", choice / 10, choice % 10);
                Scale(filename, scaleX, scaleY);
                system("explorer ..\\resources\\4.1");
                break;
            case 9://平移
                std::cout << "输入水平偏移量" << "\n";
                std::cin >> deltaX;
                std::cout << "输入垂直偏移量" << "\n";
                std::cin >> deltaY;
                sprintf_s(filename, R"(..\resources\4.2\test%d%d.bmp)", choice / 10, choice % 10);
                Translation(filename, deltaX, deltaY);
                system("explorer ..\\resources\\4.2");
                break;
            case 10://镜像
                sprintf_s(filename, R"(..\resources\4.3\test%d%d.bmp)", choice / 10, choice % 10);
                Horizontal_Mirror(filename);
                Vertical_Mirror(filename);
                system("explorer ..\\resources\\4.3");
                break;
            case 11://旋转
                std::cout << "输入旋转角度" << "\n";
                std::cin >> angle;
                sprintf_s(filename, R"(..\resources\4.4\test%d%d.bmp)", choice / 10, choice % 10);
                Rotate(filename, angle * PI / 180);
                system("explorer ..\\resources\\4.4");
                break;
            case 12://给定阈值分割
                std::cout << "输入给定阈值" << "\n";
                std::cin >> alpha;
                sprintf_s(filename, R"(..\resources\5.1\test%d%d.bmp)", choice / 10, choice % 10);
                FixedThresholdSegmentation(filename, alpha);
                system("explorer ..\\resources\\5.1");
                break;
            case 13://迭代阈值法
                std::cout << "输入迭代门限值" << "\n";
                std::cin >> alpha;
                sprintf_s(filename, R"(..\resources\5.2\test%d%d.bmp)", choice / 10, choice % 10);
                IterationThresholdSegmentation(filename, alpha);
                system("explorer ..\\resources\\5.2");
                break;
            case 14://大津法
                sprintf_s(filename, R"(..\resources\5.3\test%d%d.bmp)", choice / 10, choice % 10);
                otsu(filename);
                system("explorer ..\\resources\\5.3");
                break;
            case 15:
                RegionGrowth();
                system("explorer ..\\resources\\6.1");
                break;
            case 16:
                Merge();
                system("explorer ..\\resources\\6.2");
                break;
            case 17:
                std::cout << "输入阈值" << "\n";
                std::cin >> alpha;
                sprintf_s(filename, R"(..\resources\7.1\test%d%d.bmp)", choice / 10, choice % 10);
                Prewitt(filename, alpha);
                system("explorer ..\\resources\\7.1");
                break;
            case 18:
                std::cout << "输入阈值" << "\n";
                std::cin >> alpha;
                sprintf_s(filename, R"(..\resources\7.2\test%d%d.bmp)", choice / 10, choice % 10);
                Sobel(filename, alpha);
                system("explorer ..\\resources\\7.2");
                break;
            case 19:
                std::cout << "输入阈值" << "\n";
                std::cin >> alpha;
                sprintf_s(filename, R"(..\resources\7.3\test%d%d.bmp)", choice / 10, choice % 10);
                Log(filename, alpha);
                system("explorer ..\\resources\\7.3");
                break;
            case 20:
                std::cout << "输入阈值" << "\n";
                std::cin >> alpha;
                sprintf_s(filename, R"(..\resources\8.1\test%d%d.bmp)", choice / 10, choice % 10);
                Hough(filename, alpha);
                system("explorer ..\\resources\\8.1");
                break;
            case 21:
                sprintf_s(filename, R"(..\resources\9.1\test%d%d.bmp)", choice / 10, choice % 10);
                RegionMark(filename);
                system("explorer ..\\resources\\9.1");
                break;
            case 22:
                sprintf_s(filename, R"(..\resources\9.2\test%d%d.bmp)", choice / 10, choice % 10);
                ContourTrack(filename);
                system("explorer ..\\resources\\9.2");
                break;
            case -1:
                return 0;
            default:
                break;
        }
    }
}
