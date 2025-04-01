#pragma warning(disable:4996) //vs 2019���� fopen ���� ����� ����
#include <stdio.h> // C����� ǥ�� ����� ���̺귯��
#include <stdlib.h> //�����Ҵ��� ���� ���̺귯��
#include <Windows.h> // bmp ������ �б� ���� ���̺귯��
int main()
{
	BITMAPFILEHEADER hf; // BMP ������� 14Bytes
	BITMAPINFOHEADER hInfo; // BMP ������� 40Bytes
	RGBQUAD hRGB[256]; // �ȷ�Ʈ (256 * 4Bytes)
	FILE *fp;
	fp = fopen("lenna.bmp", "rb");
	if(fp == NULL) {
		printf("File not found\n");
		return -1;
	}
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);

	int ImgSize = hInfo.biWidth * hInfo.biHeight;
	BYTE * Image = (BYTE *)malloc(ImgSize);
    BYTE *output1 = (BYTE *)malloc(ImgSize);
    BYTE *output2 = (BYTE *)malloc(ImgSize);
    BYTE *output3 = (BYTE *)malloc(ImgSize);

	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);
	/* ����ó�� */
	
	// output1.bmp: lenna.bmp ���ϰ� ������ ����
	for(int i=0; i<ImgSize; i++)
		output1[i] = Image[i];

    // output2.bmp: ���� ������ ��Ⱚ�� 50��ŭ ������Ų ����
    for (int i = 0; i < ImgSize; i++)
        output2[i] = Image[i] + 50;

    // output3.bmp: ���� ������ ���� ��� ����(negative image)
    for (int i = 0; i < ImgSize; i++)
        output3[i] = 255 - Image[i];

	fp = fopen("output1.bmp", "wb");
    fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
    fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
    fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
    fwrite(output1, sizeof(BYTE), ImgSize, fp);
    fclose(fp);

    fp = fopen("output2.bmp", "wb");
    fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
    fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
    fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
    fwrite(output2, sizeof(BYTE), ImgSize, fp);
    fclose(fp);

    fp = fopen("output3.bmp", "wb");
    fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
    fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
    fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
    fwrite(output3, sizeof(BYTE), ImgSize, fp);
    fclose(fp);

	free(Image);
    free(output1);
    free(output2);
    free(output3);
	return 0;
}