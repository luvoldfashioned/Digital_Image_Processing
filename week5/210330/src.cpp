#pragma warning(disable : 4996) // 보안 경고(C4996)를 비활성화
#include <stdio.h>				// 표준 입출력 함수 사용
#include <stdlib.h>				// 메모리 할당 및 기타 유틸리티 함수 사용
#include <Windows.h>			// Windows API 사용 (BITMAP 관련 구조체 포함)

// 이미지 반전 함수
void InverseImage(BYTE *Img, BYTE *Out, int W, int H)
{
	int ImgSize = W * H;			  // 이미지 크기 계산 (너비 * 높이)
	for (int i = 0; i < ImgSize; i++) // 모든 픽셀에 대해 반복
	{
		Out[i] = 255 - Img[i]; // 픽셀 값을 반전 (255에서 뺌)
	}
}

// 밝기 조정 함수
void BrightnessAdj(BYTE *Img, BYTE *Out, int W, int H, int Val)
{
	int ImgSize = W * H;			  // 이미지 크기 계산
	for (int i = 0; i < ImgSize; i++) // 모든 픽셀에 대해 반복
	{
		if (Img[i] + Val > 255) // 밝기 조정 후 값이 255를 초과하면
		{
			Out[i] = 255; // 최대값으로 설정
		}
		else if (Img[i] + Val < 0) // 밝기 조정 후 값이 0보다 작으면
		{
			Out[i] = 0; // 최소값으로 설정
		}
		else
			Out[i] = Img[i] + Val; // 밝기 조정 값 적용
	}
}

// 대비 조정 함수
void ContrastAdj(BYTE *Img, BYTE *Out, int W, int H, double Val)
{
	int ImgSize = W * H;			  // 이미지 크기 계산
	for (int i = 0; i < ImgSize; i++) // 모든 픽셀에 대해 반복
	{
		if (Img[i] * Val > 255.0) // 대비 조정 후 값이 255를 초과하면
		{
			Out[i] = 255; // 최대값으로 설정
		}
		else
			Out[i] = (BYTE)(Img[i] * Val); // 대비 조정 값 적용
	}
}

// 히스토그램 계산 함수
void ObtainHistogram(BYTE *Img, int *Histo, int W, int H)
{
	int ImgSize = W * H; // 이미지 크기 계산
	for (int i = 0; i < ImgSize; i++)
	{					 // 모든 픽셀에 대해 반복
		Histo[Img[i]]++; // 해당 픽셀 값에 해당하는 히스토그램 값 증가
	}
}

// 누적 히스토그램 계산 함수
void ObtainAHistogram(int *Histo, int *AHisto)
{
	for (int i = 0; i < 256; i++)
	{ // 모든 픽셀 값(0~255)에 대해 반복
		for (int j = 0; j <= i; j++)
		{ // 누적 합 계산
			AHisto[i] += Histo[j];
		}
	}
}

// 히스토그램 스트레칭 함수
void HistogramStretching(BYTE *Img, BYTE *Out, int *Histo, int W, int H)
{
	int ImgSize = W * H; // 이미지 크기 계산
	BYTE Low, High;		 // 최소값(Low)과 최대값(High) 선언
	for (int i = 0; i < 256; i++)
	{ // 히스토그램에서 최소값 찾기
		if (Histo[i] != 0)
		{
			Low = i;
			break;
		}
	}
	for (int i = 255; i >= 0; i--)
	{ // 히스토그램에서 최대값 찾기
		if (Histo[i] != 0)
		{
			High = i;
			break;
		}
	}
	for (int i = 0; i < ImgSize; i++)
	{																	// 모든 픽셀에 대해 반복
		Out[i] = (BYTE)((Img[i] - Low) / (double)(High - Low) * 255.0); // 스트레칭 계산
	}
}

// 히스토그램 평활화 함수
void HistogramEqualization(BYTE *Img, BYTE *Out, int *AHisto, int W, int H)
{
	int ImgSize = W * H;			  // 이미지 크기 계산
	int Nt = W * H, Gmax = 255;		  // 총 픽셀 수(Nt)와 최대 그레이스케일 값(Gmax)
	double Ratio = Gmax / (double)Nt; // 정규화 비율 계산
	BYTE NormSum[256];				  // 정규화된 누적 합 배열
	for (int i = 0; i < 256; i++)
	{ // 정규화된 누적 합 계산
		NormSum[i] = (BYTE)(Ratio * AHisto[i]);
	}
	for (int i = 0; i < ImgSize; i++) // 모든 픽셀에 대해 반복
	{
		Out[i] = NormSum[Img[i]]; // 정규화된 값 적용
	}
}

// 이진화 함수
void Binarization(BYTE *Img, BYTE *Out, int W, int H, BYTE Threshold)
{
	int ImgSize = W * H; // 이미지 크기 계산
	for (int i = 0; i < ImgSize; i++)
	{ // 모든 픽셀에 대해 반복
		if (Img[i] < Threshold)
			Out[i] = 0; // 임계값보다 작으면 0
		else
			Out[i] = 255; // 임계값 이상이면 255
	}
}

// 평균 컨볼루션 함수 (박스 필터)
void AverageConv(BYTE *Img, BYTE *Out, int W, int H)
{
	double Kernel[3][3] = {0.11111, 0.11111, 0.11111, // 3x3 평균 필터 커널
						   0.11111, 0.11111, 0.11111,
						   0.11111, 0.11111, 0.11111};
	double SumProduct = 0.0; // 합산 결과 저장
	for (int i = 1; i < H - 1; i++)
	{ // Y좌표 (행)
		for (int j = 1; j < W - 1; j++)
		{ // X좌표 (열)
			for (int m = -1; m <= 1; m++)
			{ // 커널 행
				for (int n = -1; n <= 1; n++)
				{																	 // 커널 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1]; // 커널 곱셈
				}
			}
			Out[i * W + j] = (BYTE)SumProduct; // 결과 저장
			SumProduct = 0.0;				   // 초기화
		}
	}
}

// 가우시안 필터 컨볼루션 함수
void GaussAvrConv(BYTE *Img, BYTE *Out, int W, int H)
{
	double Kernel[3][3] = {0.0625, 0.125, 0.0625, // 3x3 가우시안 필터 커널
						   0.125, 0.25, 0.125,
						   0.0625, 0.125, 0.0625};
	double SumProduct = 0.0; // 합산 결과 저장
	for (int i = 1; i < H - 1; i++)
	{ // Y좌표 (행)
		for (int j = 1; j < W - 1; j++)
		{ // X좌표 (열)
			for (int m = -1; m <= 1; m++)
			{ // 커널 행
				for (int n = -1; n <= 1; n++)
				{																	 // 커널 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1]; // 커널 곱셈
				}
			}
			Out[i * W + j] = (BYTE)SumProduct; // 결과 저장
			SumProduct = 0.0;				   // 초기화
		}
	}
}

// Prewitt 마스크 X
void Prewitt_X_Conv(BYTE *Img, BYTE *Out, int W, int H)
{
	double Kernel[3][3] = {-1.0, 0.0, 1.0,
						   -1.0, 0.0, 1.0,
						   -1.0, 0.0, 1.0};
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++)
	{ // Y좌표 (행)
		for (int j = 1; j < W - 1; j++)
		{ // X좌표 (열)
			for (int m = -1; m <= 1; m++)
			{ // Kernel 행
				for (int n = -1; n <= 1; n++)
				{ // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 765  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 3;
			SumProduct = 0.0;
		}
	}
}

// Prewitt 마스크 Y
void Prewitt_Y_Conv(BYTE *Img, BYTE *Out, int W, int H)
{
	double Kernel[3][3] = {-1.0, -1.0, -1.0,
						   0.0, 0.0, 0.0,
						   1.0, 1.0, 1.0};
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++)
	{ // Y좌표 (행)
		for (int j = 1; j < W - 1; j++)
		{ // X좌표 (열)
			for (int m = -1; m <= 1; m++)
			{ // Kernel 행
				for (int n = -1; n <= 1; n++)
				{ // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 765  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 3;
			SumProduct = 0.0;
		}
	}
}

// Sobel 마스크 X
void Sobel_X_Conv(BYTE *Img, BYTE *Out, int W, int H)
{
	double Kernel[3][3] = {-1.0, 0.0, 1.0,
						   -2.0, 0.0, 2.0,
						   -1.0, 0.0, 1.0};
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++)
	{ // Y좌표 (행)
		for (int j = 1; j < W - 1; j++)
		{ // X좌표 (열)
			for (int m = -1; m <= 1; m++)
			{ // Kernel 행
				for (int n = -1; n <= 1; n++)
				{ // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 1020  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 4;
			SumProduct = 0.0;
		}
	}
}

// Sobel 마스크 Y
void Sobel_Y_Conv(BYTE *Img, BYTE *Out, int W, int H)
{
	double Kernel[3][3] = {-1.0, -2.0, -1.0,
						   0.0, 0.0, 0.0,
						   1.0, 2.0, 1.0};
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++)
	{ // Y좌표 (행)
		for (int j = 1; j < W - 1; j++)
		{ // X좌표 (열)
			for (int m = -1; m <= 1; m++)
			{ // Kernel 행
				for (int n = -1; n <= 1; n++)
				{ // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 765  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 4;
			SumProduct = 0.0;
		}
	}
}

// 라플라시안 필터
void Laplace_Conv(BYTE *Img, BYTE *Out, int W, int H)
{
	double Kernel[3][3] = {-1.0, -1.0, -1.0,
						   -1.0, 8.0, -1.0,
						   -1.0, -1.0, -1.0};
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++)
	{ // Y좌표 (행)
		for (int j = 1; j < W - 1; j++)
		{ // X좌표 (열)
			for (int m = -1; m <= 1; m++)
			{ // Kernel 행
				for (int n = -1; n <= 1; n++)
				{ // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 2040  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 8;
			SumProduct = 0.0;
		}
	}
}

// 라플라시안 필터 (DC 보정)
void Laplace_Conv_DC(BYTE *Img, BYTE *Out, int W, int H)
{
	double Kernel[3][3] = {-1.0, -1.0, -1.0,
						   -1.0, 9.0, -1.0,
						   -1.0, -1.0, -1.0};
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++)
	{ // Y좌표 (행)
		for (int j = 1; j < W - 1; j++)
		{ // X좌표 (열)
			for (int m = -1; m <= 1; m++)
			{ // Kernel 행
				for (int n = -1; n <= 1; n++)
				{ // Kernel 열
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			if (SumProduct > 255.0)
				Out[i * W + j] = 255;
			else if (SumProduct < 0.0)
				Out[i * W + j] = 0;
			else
				Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0.0;
		}
	}
}

int main()
{
	BITMAPFILEHEADER hf;	// 14바이트
	BITMAPINFOHEADER hInfo; // 40바이트
	RGBQUAD hRGB[256];		// 1024바이트
	FILE *fp;
	fp = fopen("lenna.bmp", "rb");
	if (fp == NULL)
	{
		printf("File not found!\n");
		return -1;
	}
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);
	int ImgSize = hInfo.biWidth * hInfo.biHeight;
	BYTE *Image = (BYTE *)malloc(ImgSize);
	BYTE *Temp = (BYTE *)malloc(ImgSize); // 임시배열
	BYTE *Output = (BYTE *)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	int Histo[256] = {0};
	int AHisto[256] = {0};

	GaussAvrConv(Image, Temp, hInfo.biWidth, hInfo.biHeight);
	Laplace_Conv_DC(Temp, Output, hInfo.biWidth, hInfo.biHeight);

	fp = fopen("output.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);
	fclose(fp);
	free(Image);
	free(Output);
	free(Temp);
	return 0;
}