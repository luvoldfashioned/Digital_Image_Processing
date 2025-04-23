#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
void InverseImage(BYTE *Img, BYTE *Out, int W, int H)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
	{
		Out[i] = 255 - Img[i];
	}
}
void BrightnessAdj(BYTE *Img, BYTE *Out, int W, int H, int Val)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
	{
		if (Img[i] + Val > 255)
		{
			Out[i] = 255;
		}
		else if (Img[i] + Val < 0)
		{
			Out[i] = 0;
		}
		else
			Out[i] = Img[i] + Val;
	}
}
void ContrastAdj(BYTE *Img, BYTE *Out, int W, int H, double Val)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
	{
		if (Img[i] * Val > 255.0)
		{
			Out[i] = 255;
		}
		else
			Out[i] = (BYTE)(Img[i] * Val);
	}
}

void ObtainHistogram(BYTE *Img, int *Histo, int W, int H)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
	{
		Histo[Img[i]]++;
	}
}

void ObtainAHistogram(int *Histo, int *AHisto)
{
	for (int i = 0; i < 256; i++)
	{
		for (int j = 0; j <= i; j++)
		{
			AHisto[i] += Histo[j];
		}
	}
}

void HistogramStretching(BYTE *Img, BYTE *Out, int *Histo, int W, int H)
{
	int ImgSize = W * H;
	BYTE Low, High;
	for (int i = 0; i < 256; i++)
	{
		if (Histo[i] != 0)
		{
			Low = i;
			break;
		}
	}
	for (int i = 255; i >= 0; i--)
	{
		if (Histo[i] != 0)
		{
			High = i;
			break;
		}
	}
	for (int i = 0; i < ImgSize; i++)
	{
		Out[i] = (BYTE)((Img[i] - Low) / (double)(High - Low) * 255.0);
	}
}
void HistogramEqualization(BYTE *Img, BYTE *Out, int *AHisto, int W, int H)
{
	int ImgSize = W * H;
	int Nt = W * H, Gmax = 255;
	double Ratio = Gmax / (double)Nt;
	BYTE NormSum[256];
	for (int i = 0; i < 256; i++)
	{
		NormSum[i] = (BYTE)(Ratio * AHisto[i]);
	}
	for (int i = 0; i < ImgSize; i++)
	{
		Out[i] = NormSum[Img[i]];
	}
}

void Binarization(BYTE *Img, BYTE *Out, int W, int H, BYTE Threshold)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
	{
		if (Img[i] < Threshold)
			Out[i] = 0;
		else
			Out[i] = 255;
	}
}

// int GozalezBinThresh()
//{
//
// }

void AverageConv(BYTE *Img, BYTE *Out, int W, int H) // 박스평활화
{
	double Kernel[3][3] = {0.11111, 0.11111, 0.11111,
						   0.11111, 0.11111, 0.11111,
						   0.11111, 0.11111, 0.11111};
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
			Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0.0;
		}
	}
}

void GaussAvrConv(BYTE *Img, BYTE *Out, int W, int H) // 가우시안평활화
{
	double Kernel[3][3] = {0.0625, 0.125, 0.0625,
						   0.125, 0.25, 0.125,
						   0.0625, 0.125, 0.0625};
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
			Out[i * W + j] = (BYTE)SumProduct;
			SumProduct = 0.0;
		}
	}
}

void Prewitt_X_Conv(BYTE *Img, BYTE *Out, int W, int H) // Prewitt 마스크 X
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

void Prewitt_Y_Conv(BYTE *Img, BYTE *Out, int W, int H) // Prewitt 마스크 X
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

void Sobel_X_Conv(BYTE *Img, BYTE *Out, int W, int H) // Prewitt 마스크 X
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

void Sobel_Y_Conv(BYTE *Img, BYTE *Out, int W, int H) // Prewitt 마스크 X
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

void Laplace_Conv(BYTE *Img, BYTE *Out, int W, int H) // Prewitt 마스크 X
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

void Laplace_Conv_DC(BYTE *Img, BYTE *Out, int W, int H) // Prewitt 마스크 X
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
			// Out[i * W + j] = abs((long)SumProduct) / 8;
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

void SaveBMPFile(BITMAPFILEHEADER hf, BITMAPINFOHEADER hInfo,
				 RGBQUAD *hRGB, BYTE *Output, int W, int H, const char *FileName)
{
	FILE *fp = fopen(FileName, "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), W * H, fp);
	fclose(fp);
}

void swap(BYTE *a, BYTE *b)
{
	BYTE temp = *a;
	*a = *b;
	*b = temp;
}

BYTE Median(BYTE *arr, int size)
{
	// 오름차순 정렬
	const int S = size;
	for (int i = 0; i < size - 1; i++) // pivot index
	{
		for (int j = i + 1; j < size; j++) // 비교대상 index
		{
			if (arr[i] > arr[j])
				swap(&arr[i], &arr[j]);
		}
	}
	return arr[S / 2];
}

BYTE MaxPooling(BYTE *arr, int size)
{
	// 오름차순 정렬
	const int S = size;
	for (int i = 0; i < size - 1; i++) // pivot index
	{
		for (int j = i + 1; j < size; j++) // 비교대상 index
		{
			if (arr[i] > arr[j])
				swap(&arr[i], &arr[j]);
		}
	}
	return arr[S - 1];
}

BYTE MinPooling(BYTE *arr, int size)
{
	// 오름차순 정렬
	const int S = size;
	for (int i = 0; i < size - 1; i++) // pivot index
	{
		for (int j = i + 1; j < size; j++) // 비교대상 index
		{
			if (arr[i] > arr[j])
				swap(&arr[i], &arr[j]);
		}
	}
	return arr[0];
}

int main()
{
	BITMAPFILEHEADER hf;	// 14바이트
	BITMAPINFOHEADER hInfo; // 40바이트
	RGBQUAD hRGB[256];		// 1024바이트
	FILE *fp;
	fp = fopen("lenna_gauss.bmp", "rb");
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

	/*ObtainHistogram(Image, Histo, hInfo.biWidth, hInfo.biHeight);
	ObtainAHistogram(Histo, AHisto);
	HistogramEqualization(Image, Output, AHisto, hInfo.biWidth, hInfo.biHeight);*/
	/*int Thres = GozalezBinThresh();
	Binarization(Image, Output, hInfo.biWidth, hInfo.biHeight, Thres);*/

	// GaussAvrConv(Image, Output, hInfo.biWidth, hInfo.biHeight);

	/*Sobel_X_Conv(Image, Temp, hInfo.biWidth, hInfo.biHeight);
	Sobel_Y_Conv(Image, Output, hInfo.biWidth, hInfo.biHeight);
	for (int i = 0; i < ImgSize; i++) {
		if (Temp[i] > Output[i]) 	Output[i] = Temp[i];
	}
	Binarization(Output, Output, hInfo.biWidth, hInfo.biHeight, 40);*/
	/*GaussAvrConv(Image, Temp, hInfo.biWidth, hInfo.biHeight);
	Laplace_Conv_DC(Temp, Output, hInfo.biWidth, hInfo.biHeight);*/

	// HistogramStretching(Image, Output, Histo, hInfo.biWidth, hInfo.biHeight);
	// InverseImage(Image, Output, hInfo.biWidth, hInfo.biHeight);
	// BrightnessAdj(Image, Output, hInfo.biWidth, hInfo.biHeight, 70);
	// ContrastAdj(Image, Output, hInfo.biWidth, hInfo.biHeight, 0.5);

	/* Median filtering */

	int Length = 9;			 // 마스크(필터)의 한 변의 길이. 여기서는 3x3 마스크를 사용.
	int Margin = Length / 2; // 필터의 중심에서의 거리. 예: 3x3이면 Margin = 1

	int WSize = Length * Length; // 필터 내 전체 픽셀 수 (예: 3x3 = 9개)

	// 마스크 안의 픽셀 값을 담을 임시 배열 (정렬 후 중앙값 구하기 위함)
	BYTE *temp = (BYTE *)malloc(sizeof(BYTE) * WSize);

	int W = hInfo.biWidth, H = hInfo.biHeight; // 이미지의 너비(W), 높이(H)

	int i, j, m, n; // 반복문에서 사용할 변수 선언

	// 이미지 내부 픽셀들에 대해 필터 적용 (테두리는 제외해야 함)
	for (i = Margin; i < H - Margin; i++)
	{ // 행 방향으로 스캔
		for (j = Margin; j < W - Margin; j++)
		{ // 열 방향으로 스캔

			// 마스크 내의 모든 픽셀 값을 temp 배열에 저장
			for (m = -Margin; m <= Margin; m++)
			{ // 마스크의 행 탐색
				for (n = -Margin; n <= Margin; n++)
				{ // 마스크의 열 탐색
					temp[(m + Margin) * Length + (n + Margin)] = Image[(i + m) * W + (j + n)];
					// 마스크 내의 픽셀을 temp 배열에 1차원으로 저장
				}
			}

			// temp 배열에서 중앙값 계산 후 결과 이미지에 저장
			Output[i * W + j] = Median(temp, WSize);
		}
	}

	free(temp); // 동적 할당 해제

	/* Median filtering */

	AverageConv(Image, Output, hInfo.biWidth, hInfo.biHeight);

	SaveBMPFile(hf, hInfo, hRGB, Output, hInfo.biWidth, hInfo.biHeight, "median_9.bmp");

	free(Image);
	free(Output);
	free(Temp);
	return 0;
}