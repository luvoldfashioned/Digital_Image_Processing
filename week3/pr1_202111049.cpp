#pragma warning(disable:4996) //vs 2019에서 fopen 사용시 경고문구 제거
#include <stdio.h> // C언어의 표준 입출력 라이브러리
#include <stdlib.h> //동적할당을 위한 라이브러리
#include <Windows.h> // bmp 파일을 읽기 위한 라이브러리
int main()
{
	BITMAPFILEHEADER hf; // BMP 파일헤더 14Bytes
	BITMAPINFOHEADER hInfo; // BMP 인포헤더 40Bytes
	RGBQUAD hRGB[256]; // 팔레트 (256 * 4Bytes)
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
	/* 영상처리 */
	
	// output1.bmp: lenna.bmp 파일과 동일한 영상
	for(int i=0; i<ImgSize; i++)
		output1[i] = Image[i];

    // output2.bmp: 원본 영상의 밝기값을 50만큼 증가시킨 영상
    for (int i = 0; i < ImgSize; i++)
        output2[i] = Image[i] + 50;

    // output3.bmp: 원본 영상의 반전 결과 영상(negative image)
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