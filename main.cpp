#pragma pack(2)

#include "handle.h"

#define  PI  acos(-1.0)

void menu() {
    std::cout << "����ͼ����" << "\n" << "��������ѡ����" << "\n" << "1:24λ��ɫ" << "\n" << "2:8λ�ҶȻ�" << "\n" << "3:8λ��ɫ"
              << "\n" << "4:����ֱ��ͼ" << "\n" << "5:ֱ��ͼ���⻯" << "\n" << "6:ƽ���˲�" << "\n" << "7:��ֵ�˲�" << "\n" << "8:����"
              << "\n"
              << "9:ƽ��" << "\n" << "10:����" << "\n" << "11:��ת" << "\n" << "12:�̶���ֵ�ָ�" << "\n" << "13:������ֵ�ָ�" << "\n"
              << "14:���" << "\n" << "15:��������" << "\n" << "16:���Ѻϲ�" << "\n" << "17:Prewitt" << "\n" << "18:Sobel"
              << "\n" << "19:LOG" << "\n"
              << "20:ֱ�߼��" << "\n" << "21:��ͨ�����" << "\n" << "22:������ȡ" << "\n";
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
            case 1://��ɫ
                sprintf_s(filename, R"(..\resources\1.1\test%d%d.bmp)", choice / 10, choice % 10);
                ColorSeparation(filename);
                system("explorer ..\\resources\\1.1");
                break;
            case 2://�ҶȻ�
                sprintf_s(filename, R"(..\resources\1.2\test%d%d.bmp)", choice / 10, choice % 10);
                Grayscale(filename);
                system("explorer ..\\resources\\1.2");
                break;
            case 3://��ɫ
                sprintf_s(filename, R"(..\resources\1.3\test%d%d.bmp)", choice / 10, choice % 10);
                Invert(filename);
                system("explorer ..\\resources\\1.3");
                break;
            case 4://ֱ��ͼ
                sprintf_s(filename, R"(..\resources\2.1\test%d%d.bmp)", choice / 10, choice % 10);
                Histogram(filename);
                system("explorer ..\\resources\\2.1");
                break;
            case 5://���⻯
                sprintf_s(filename, R"(..\resources\2.2\test%d%d.bmp)", choice / 10, choice % 10);
                Equalization(filename);
                system("explorer ..\\resources\\2.2");
                break;
            case 6://ƽ������
                std::cout << "���봰�ڴ�С��������" << "\n";
                std::cin >> alpha;
                sprintf_s(filename, R"(..\resources\3.1\test%d%d.bmp)", choice / 10, choice % 10);
                AverageTreatment(filename, alpha);
                system("explorer ..\\resources\\3.1");
                break;
            case 7://��ֵ����
                std::cout << "���봰�ڴ�С��������" << "\n";
                std::cin >> alpha;
                sprintf_s(filename, R"(..\resources\3.2\test%d%d.bmp)", choice / 10, choice % 10);
                MedianFiltering(filename, alpha);
                system("explorer ..\\resources\\3.2");
                break;
            case 8://����
                std::cout << "����������ű�" << "\n";
                std::cin >> scaleX;
                std::cout << "�����������ű�" << "\n";
                std::cin >> scaleY;
                sprintf_s(filename, R"(..\resources\4.1\test%d%d.bmp)", choice / 10, choice % 10);
                Scale(filename, scaleX, scaleY);
                system("explorer ..\\resources\\4.1");
                break;
            case 9://ƽ��
                std::cout << "����ˮƽƫ����" << "\n";
                std::cin >> deltaX;
                std::cout << "���봹ֱƫ����" << "\n";
                std::cin >> deltaY;
                sprintf_s(filename, R"(..\resources\4.2\test%d%d.bmp)", choice / 10, choice % 10);
                Translation(filename, deltaX, deltaY);
                system("explorer ..\\resources\\4.2");
                break;
            case 10://����
                sprintf_s(filename, R"(..\resources\4.3\test%d%d.bmp)", choice / 10, choice % 10);
                Horizontal_Mirror(filename);
                Vertical_Mirror(filename);
                system("explorer ..\\resources\\4.3");
                break;
            case 11://��ת
                std::cout << "������ת�Ƕ�" << "\n";
                std::cin >> angle;
                sprintf_s(filename, R"(..\resources\4.4\test%d%d.bmp)", choice / 10, choice % 10);
                Rotate(filename, angle * PI / 180);
                system("explorer ..\\resources\\4.4");
                break;
            case 12://������ֵ�ָ�
                std::cout << "���������ֵ" << "\n";
                std::cin >> alpha;
                sprintf_s(filename, R"(..\resources\5.1\test%d%d.bmp)", choice / 10, choice % 10);
                FixedThresholdSegmentation(filename, alpha);
                system("explorer ..\\resources\\5.1");
                break;
            case 13://������ֵ��
                std::cout << "�����������ֵ" << "\n";
                std::cin >> alpha;
                sprintf_s(filename, R"(..\resources\5.2\test%d%d.bmp)", choice / 10, choice % 10);
                IterationThresholdSegmentation(filename, alpha);
                system("explorer ..\\resources\\5.2");
                break;
            case 14://���
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
                std::cout << "������ֵ" << "\n";
                std::cin >> alpha;
                sprintf_s(filename, R"(..\resources\7.1\test%d%d.bmp)", choice / 10, choice % 10);
                Prewitt(filename, alpha);
                system("explorer ..\\resources\\7.1");
                break;
            case 18:
                std::cout << "������ֵ" << "\n";
                std::cin >> alpha;
                sprintf_s(filename, R"(..\resources\7.2\test%d%d.bmp)", choice / 10, choice % 10);
                Sobel(filename, alpha);
                system("explorer ..\\resources\\7.2");
                break;
            case 19:
                std::cout << "������ֵ" << "\n";
                std::cin >> alpha;
                sprintf_s(filename, R"(..\resources\7.3\test%d%d.bmp)", choice / 10, choice % 10);
                Log(filename, alpha);
                system("explorer ..\\resources\\7.3");
                break;
            case 20:
                std::cout << "������ֵ" << "\n";
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
