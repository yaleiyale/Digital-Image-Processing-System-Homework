#pragma pack(2)

#include "handle.h"

const double PI = acos(-1.0);

void menu() {
    std::cout << "����ͼ����" << "\n" << "��������ѡ����" << "\n" << "1:24λ��ɫ" << "\n" << "2:8λ�ҶȻ�" << "\n" << "3:8λ��ɫ" << "\n"
              << "4:����ֱ��ͼ" << "\n" << "5:ֱ��ͼ���⻯" << "\n" << "6:ƽ���˲�" << "\n" << "7:��ֵ�˲�" << "\n" << "8:����" << "\n"
              << "9:ƽ��" << "\n" << "10:����" << "\n" << "11:��ת" << "\n";
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
                std::cout << "���봰�ڴ�С��������" << "\n";
                std::cin >> windows_size;
                sprintf_s(filename, R"(..\resources\3.1\test%d%d.bmp)", choice / 10, choice % 10);
                AverageTreatment(filename, windows_size);
                system("explorer ..\\resources\\3.1");
                break;
            case 7:
                std::cout << "���봰�ڴ�С��������" << "\n";
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
                std::cout << "����ˮƽƫ����";
                std::cin >> deltax;
                std::cout << "���봹ֱƫ����";
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
                std::cout << "������ת�Ƕ�";
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
