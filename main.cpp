#pragma pack(2)

#include "handle.h"

int main() {

    int choice;
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
                sprintf_s(filename, R"(..\resources\3.1\test%d%d.bmp)", choice / 10, choice % 10);
                AverageTreatment(filename, 3);
                system("explorer ..\\resources\\3.1");
                break;
            case 7:
                sprintf_s(filename, R"(..\resources\3.2\test%d%d.bmp)", choice / 10, choice % 10);
                MedianFiltering(filename, 3);
                system("explorer ..\\resources\\3.2");
                break;
            case -1:
                return 0;
            default:
                break;
        }
    }
}