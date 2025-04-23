#pragma warning(disable : 4996) // Visual Studio���� fopen ��� �� �߻��ϴ� ��� ����
#include <stdio.h>              // ǥ�� ����� �Լ� ����� ���� ��� ����
#include <stdlib.h>             // ���� �޸� �Ҵ� �� ��Ÿ ��ƿ��Ƽ �Լ� ����� ���� ��� ����
#include <Windows.h>            // BMP ���� ó���� ���� Windows API ��� ����

int main()
{
    // BMP ������ ��� ������ �����ϱ� ���� ����ü
    BITMAPFILEHEADER hf;    // BMP ���� ��� (������ �⺻ ����, 14����Ʈ)
    BITMAPINFOHEADER hInfo; // BMP ���� ��� (�̹����� �� ����, 40����Ʈ)
    RGBQUAD hRGB[256];      // BMP �ȷ�Ʈ (256���� ����, �� ������ 4����Ʈ)

    FILE *fp; // ���� ������ ����

    // BMP ���� ���� (�б� ���)
    fp = fopen("lenna.bmp", "rb"); // "lenna.bmp" ������ ���̳ʸ� �б� ���� ����
    if (fp == NULL)                // ������ ���ų� �� �� ���� ���
    {
        printf("File not found\n"); // ���� �޽��� ���
        return -1;                  // ���α׷� ���� (������ ���Ḧ ��Ÿ���� -1 ��ȯ)
    }

    // BMP ������ ��� ���� �б�
    fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);    // BMP ���� ��� �б�
    fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp); // BMP ���� ��� �б�
    fread(hRGB, sizeof(RGBQUAD), 256, fp);          // BMP �ȷ�Ʈ �б�

    // �̹��� ũ�� ��� (�ʺ� x ����)
    int ImgSize = hInfo.biWidth * hInfo.biHeight;

    // ���� �޸� �Ҵ� (�̹��� �����Ϳ� ó�� ����� ������ ����)
    BYTE *Image = (BYTE *)malloc(ImgSize);   // ���� �̹��� �����͸� ������ �޸�
    BYTE *output1 = (BYTE *)malloc(ImgSize); // ������ ������ �̹����� ������ �޸�
    BYTE *output2 = (BYTE *)malloc(ImgSize); // ��� ���� �̹����� ������ �޸�
    BYTE *output3 = (BYTE *)malloc(ImgSize); // ���� �̹����� ������ �޸�

    // BMP �̹��� ������ �б�
    fread(Image, sizeof(BYTE), ImgSize, fp); // �̹��� �����͸� �о �޸𸮿� ����
    fclose(fp);                              // ���� �ݱ�

    /* --- ���� ó�� --- */

    // output1.bmp: ���� �̹����� ������ ������ ����
    for (int i = 0; i < ImgSize; i++)
        output1[i] = Image[i];

    // output2.bmp: ���� �̹����� ��⸦ 50 ����
    for (int i = 0; i < ImgSize; i++)
        output2[i] = Image[i] + 50;

    // output3.bmp: ���� �̹����� ������ ���� (255���� ���� ���� ��)
    for (int i = 0; i < ImgSize; i++)
        output3[i] = 255 - Image[i];

    /* --- ó���� �̹����� ���Ϸ� ���� --- */

    // output1.bmp ���� ���� (������ ������ �̹��� ����)
    fp = fopen("output1.bmp", "wb");                            // ���� ���� (���� ���)
    fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);    // BMP ���� ��� ����
    fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp); // BMP ���� ��� ����
    fwrite(hRGB, sizeof(RGBQUAD), 256, fp);                     // BMP �ȷ�Ʈ ����
    fwrite(output1, sizeof(BYTE), ImgSize, fp);                 // �̹��� ������ ����
    fclose(fp);                                                 // ���� �ݱ�

    // output2.bmp ���� ���� (��� ���� �̹��� ����)
    fp = fopen("output2.bmp", "wb");
    fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
    fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
    fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
    fwrite(output2, sizeof(BYTE), ImgSize, fp);
    fclose(fp);

    // output3.bmp ���� ���� (���� �̹��� ����)
    fp = fopen("output3.bmp", "wb");
    fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
    fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
    fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
    fwrite(output3, sizeof(BYTE), ImgSize, fp);
    fclose(fp);

    /* --- ���� �޸� ���� --- */
    free(Image);   // ���� �̹��� ������ �޸� ����
    free(output1); // output1 ������ �޸� ����
    free(output2); // output2 ������ �޸� ����
    free(output3); // output3 ������ �޸� ����

    return 0; // ���α׷� ���� ����
}
// end of file