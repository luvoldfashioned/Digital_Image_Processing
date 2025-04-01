#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPSILON 3 // ���� ���ǿ� �ش��ϴ� �ԽǷ� ��

int main()
{
    int width = 256, height = 256;       // ���� �̹��� ũ��
    unsigned char image[256][256];       // �Է� �̹��� ������ (����)
    unsigned char thresholded[256][256]; // ����ȭ�� ��� �̹���
    int T = 128;                         // �ʱ� ��谪 (����� �߰������� ����)
    int newT;
    int sumG1, sumG2, countG1, countG2;

    // ���� �̹��� ������ �ʱ�ȭ (���� ������ ����)
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = rand() % 256; // 0~255 ������ ���� ��
        }
    }

    while (1)
    {
        // G1�� G2 �ʱ�ȭ
        sumG1 = sumG2 = countG1 = countG2 = 0;

        // ��Ⱚ T�� �������� G1�� G2�� ������
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if (image[i][j] > T)
                {
                    sumG1 += image[i][j];
                    countG1++;
                }
                else
                {
                    sumG2 += image[i][j];
                    countG2++;
                }
            }
        }

        // G1�� G2�� ��� ��� ���
        double mu1 = (countG1 == 0) ? 0 : (double)sumG1 / countG1;
        double mu2 = (countG2 == 0) ? 0 : (double)sumG2 / countG2;

        // ���ο� ��谪 ���
        newT = (int)((mu1 + mu2) / 2);

        // ���� ���� Ȯ�� (��谪 ��ȭ�� �ԽǷ� ������ ��� ����)
        if (abs(newT - T) <= EPSILON)
        {
            break;
        }

        // ��谪 ������Ʈ
        T = newT;
    }

    // ���� ��谪 T�� �������� ����ȭ ����
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            thresholded[i][j] = (image[i][j] > T) ? 255 : 0;
        }
    }

    // ��� ���
    printf("Final Threshold Value: %d\n", T);

    return 0;
}