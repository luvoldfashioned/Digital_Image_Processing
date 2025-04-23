#pragma warning(disable : 4996) // ���� ���(C4996)�� ��Ȱ��ȭ
#include <stdio.h>				// ǥ�� ����� �Լ� ���
#include <stdlib.h>				// �޸� �Ҵ� �� ��Ÿ ��ƿ��Ƽ �Լ� ���
#include <Windows.h>			// Windows API ��� (BITMAP ���� ����ü ����)

// �̹��� ���� �Լ�
void InverseImage(BYTE *Img, BYTE *Out, int W, int H)
{
	int ImgSize = W * H;			  // �̹��� ũ�� ��� (�ʺ� * ����)
	for (int i = 0; i < ImgSize; i++) // ��� �ȼ��� ���� �ݺ�
	{
		Out[i] = 255 - Img[i]; // �ȼ� ���� ���� (255���� ��)
	}
}

// ��� ���� �Լ�
void BrightnessAdj(BYTE *Img, BYTE *Out, int W, int H, int Val)
{
	int ImgSize = W * H;			  // �̹��� ũ�� ���
	for (int i = 0; i < ImgSize; i++) // ��� �ȼ��� ���� �ݺ�
	{
		if (Img[i] + Val > 255) // ��� ���� �� ���� 255�� �ʰ��ϸ�
		{
			Out[i] = 255; // �ִ밪���� ����
		}
		else if (Img[i] + Val < 0) // ��� ���� �� ���� 0���� ������
		{
			Out[i] = 0; // �ּҰ����� ����
		}
		else
			Out[i] = Img[i] + Val; // ��� ���� �� ����
	}
}

// ��� ���� �Լ�
void ContrastAdj(BYTE *Img, BYTE *Out, int W, int H, double Val)
{
	int ImgSize = W * H;			  // �̹��� ũ�� ���
	for (int i = 0; i < ImgSize; i++) // ��� �ȼ��� ���� �ݺ�
	{
		if (Img[i] * Val > 255.0) // ��� ���� �� ���� 255�� �ʰ��ϸ�
		{
			Out[i] = 255; // �ִ밪���� ����
		}
		else
			Out[i] = (BYTE)(Img[i] * Val); // ��� ���� �� ����
	}
}

// ������׷� ��� �Լ�
void ObtainHistogram(BYTE *Img, int *Histo, int W, int H)
{
	int ImgSize = W * H; // �̹��� ũ�� ���
	for (int i = 0; i < ImgSize; i++)
	{					 // ��� �ȼ��� ���� �ݺ�
		Histo[Img[i]]++; // �ش� �ȼ� ���� �ش��ϴ� ������׷� �� ����
	}
}

// ���� ������׷� ��� �Լ�
void ObtainAHistogram(int *Histo, int *AHisto)
{
	for (int i = 0; i < 256; i++)
	{ // ��� �ȼ� ��(0~255)�� ���� �ݺ�
		for (int j = 0; j <= i; j++)
		{ // ���� �� ���
			AHisto[i] += Histo[j];
		}
	}
}

// ������׷� ��Ʈ��Ī �Լ�
void HistogramStretching(BYTE *Img, BYTE *Out, int *Histo, int W, int H)
{
	int ImgSize = W * H; // �̹��� ũ�� ���
	BYTE Low, High;		 // �ּҰ�(Low)�� �ִ밪(High) ����
	for (int i = 0; i < 256; i++)
	{ // ������׷����� �ּҰ� ã��
		if (Histo[i] != 0)
		{
			Low = i;
			break;
		}
	}
	for (int i = 255; i >= 0; i--)
	{ // ������׷����� �ִ밪 ã��
		if (Histo[i] != 0)
		{
			High = i;
			break;
		}
	}
	for (int i = 0; i < ImgSize; i++)
	{																	// ��� �ȼ��� ���� �ݺ�
		Out[i] = (BYTE)((Img[i] - Low) / (double)(High - Low) * 255.0); // ��Ʈ��Ī ���
	}
}

// ������׷� ��Ȱȭ �Լ�
void HistogramEqualization(BYTE *Img, BYTE *Out, int *AHisto, int W, int H)
{
	int ImgSize = W * H;			  // �̹��� ũ�� ���
	int Nt = W * H, Gmax = 255;		  // �� �ȼ� ��(Nt)�� �ִ� �׷��̽����� ��(Gmax)
	double Ratio = Gmax / (double)Nt; // ����ȭ ���� ���
	BYTE NormSum[256];				  // ����ȭ�� ���� �� �迭
	for (int i = 0; i < 256; i++)
	{ // ����ȭ�� ���� �� ���
		NormSum[i] = (BYTE)(Ratio * AHisto[i]);
	}
	for (int i = 0; i < ImgSize; i++) // ��� �ȼ��� ���� �ݺ�
	{
		Out[i] = NormSum[Img[i]]; // ����ȭ�� �� ����
	}
}

// ����ȭ �Լ�
void Binarization(BYTE *Img, BYTE *Out, int W, int H, BYTE Threshold)
{
	int ImgSize = W * H; // �̹��� ũ�� ���
	for (int i = 0; i < ImgSize; i++)
	{ // ��� �ȼ��� ���� �ݺ�
		if (Img[i] < Threshold)
			Out[i] = 0; // �Ӱ谪���� ������ 0
		else
			Out[i] = 255; // �Ӱ谪 �̻��̸� 255
	}
}

// ��� ������� �Լ� (�ڽ� ����)
void AverageConv(BYTE *Img, BYTE *Out, int W, int H)
{
	double Kernel[3][3] = {0.11111, 0.11111, 0.11111, // 3x3 ��� ���� Ŀ��
						   0.11111, 0.11111, 0.11111,
						   0.11111, 0.11111, 0.11111};
	double SumProduct = 0.0; // �ջ� ��� ����
	for (int i = 1; i < H - 1; i++)
	{ // Y��ǥ (��)
		for (int j = 1; j < W - 1; j++)
		{ // X��ǥ (��)
			for (int m = -1; m <= 1; m++)
			{ // Ŀ�� ��
				for (int n = -1; n <= 1; n++)
				{																	 // Ŀ�� ��
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1]; // Ŀ�� ����
				}
			}
			Out[i * W + j] = (BYTE)SumProduct; // ��� ����
			SumProduct = 0.0;				   // �ʱ�ȭ
		}
	}
}

// ����þ� ���� ������� �Լ�
void GaussAvrConv(BYTE *Img, BYTE *Out, int W, int H)
{
	double Kernel[3][3] = {0.0625, 0.125, 0.0625, // 3x3 ����þ� ���� Ŀ��
						   0.125, 0.25, 0.125,
						   0.0625, 0.125, 0.0625};
	double SumProduct = 0.0; // �ջ� ��� ����
	for (int i = 1; i < H - 1; i++)
	{ // Y��ǥ (��)
		for (int j = 1; j < W - 1; j++)
		{ // X��ǥ (��)
			for (int m = -1; m <= 1; m++)
			{ // Ŀ�� ��
				for (int n = -1; n <= 1; n++)
				{																	 // Ŀ�� ��
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1]; // Ŀ�� ����
				}
			}
			Out[i * W + j] = (BYTE)SumProduct; // ��� ����
			SumProduct = 0.0;				   // �ʱ�ȭ
		}
	}
}

// Prewitt ����ũ X
void Prewitt_X_Conv(BYTE *Img, BYTE *Out, int W, int H)
{
	double Kernel[3][3] = {-1.0, 0.0, 1.0,
						   -1.0, 0.0, 1.0,
						   -1.0, 0.0, 1.0};
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++)
	{ // Y��ǥ (��)
		for (int j = 1; j < W - 1; j++)
		{ // X��ǥ (��)
			for (int m = -1; m <= 1; m++)
			{ // Kernel ��
				for (int n = -1; n <= 1; n++)
				{ // Kernel ��
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 765  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 3;
			SumProduct = 0.0;
		}
	}
}

// Prewitt ����ũ Y
void Prewitt_Y_Conv(BYTE *Img, BYTE *Out, int W, int H)
{
	double Kernel[3][3] = {-1.0, -1.0, -1.0,
						   0.0, 0.0, 0.0,
						   1.0, 1.0, 1.0};
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++)
	{ // Y��ǥ (��)
		for (int j = 1; j < W - 1; j++)
		{ // X��ǥ (��)
			for (int m = -1; m <= 1; m++)
			{ // Kernel ��
				for (int n = -1; n <= 1; n++)
				{ // Kernel ��
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 765  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 3;
			SumProduct = 0.0;
		}
	}
}

// Sobel ����ũ X
void Sobel_X_Conv(BYTE *Img, BYTE *Out, int W, int H)
{
	double Kernel[3][3] = {-1.0, 0.0, 1.0,
						   -2.0, 0.0, 2.0,
						   -1.0, 0.0, 1.0};
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++)
	{ // Y��ǥ (��)
		for (int j = 1; j < W - 1; j++)
		{ // X��ǥ (��)
			for (int m = -1; m <= 1; m++)
			{ // Kernel ��
				for (int n = -1; n <= 1; n++)
				{ // Kernel ��
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 1020  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 4;
			SumProduct = 0.0;
		}
	}
}

// Sobel ����ũ Y
void Sobel_Y_Conv(BYTE *Img, BYTE *Out, int W, int H)
{
	double Kernel[3][3] = {-1.0, -2.0, -1.0,
						   0.0, 0.0, 0.0,
						   1.0, 2.0, 1.0};
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++)
	{ // Y��ǥ (��)
		for (int j = 1; j < W - 1; j++)
		{ // X��ǥ (��)
			for (int m = -1; m <= 1; m++)
			{ // Kernel ��
				for (int n = -1; n <= 1; n++)
				{ // Kernel ��
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 765  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 4;
			SumProduct = 0.0;
		}
	}
}

// ���ö�þ� ����
void Laplace_Conv(BYTE *Img, BYTE *Out, int W, int H)
{
	double Kernel[3][3] = {-1.0, -1.0, -1.0,
						   -1.0, 8.0, -1.0,
						   -1.0, -1.0, -1.0};
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++)
	{ // Y��ǥ (��)
		for (int j = 1; j < W - 1; j++)
		{ // X��ǥ (��)
			for (int m = -1; m <= 1; m++)
			{ // Kernel ��
				for (int n = -1; n <= 1; n++)
				{ // Kernel ��
					SumProduct += Img[(i + m) * W + (j + n)] * Kernel[m + 1][n + 1];
				}
			}
			// 0 ~ 2040  =====> 0 ~ 255
			Out[i * W + j] = abs((long)SumProduct) / 8;
			SumProduct = 0.0;
		}
	}
}

// ���ö�þ� ���� (DC ����)
void Laplace_Conv_DC(BYTE *Img, BYTE *Out, int W, int H)
{
	double Kernel[3][3] = {-1.0, -1.0, -1.0,
						   -1.0, 9.0, -1.0,
						   -1.0, -1.0, -1.0};
	double SumProduct = 0.0;
	for (int i = 1; i < H - 1; i++)
	{ // Y��ǥ (��)
		for (int j = 1; j < W - 1; j++)
		{ // X��ǥ (��)
			for (int m = -1; m <= 1; m++)
			{ // Kernel ��
				for (int n = -1; n <= 1; n++)
				{ // Kernel ��
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
	BITMAPFILEHEADER hf;	// 14����Ʈ
	BITMAPINFOHEADER hInfo; // 40����Ʈ
	RGBQUAD hRGB[256];		// 1024����Ʈ
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
	BYTE *Temp = (BYTE *)malloc(ImgSize); // �ӽù迭
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