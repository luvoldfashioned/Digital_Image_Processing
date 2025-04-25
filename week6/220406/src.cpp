#pragma warning(disable : 4996) // ���� ���(C4996)�� ��Ȱ��ȭ
#include <stdio.h>				// ǥ�� ����� �Լ� ���
#include <stdlib.h>				// ���� �޸� �Ҵ� �� rand() �Լ� ���
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
	int ImgSize = W * H;			  // �̹��� ũ�� ���
	for (int i = 0; i < ImgSize; i++) // ��� �ȼ��� ���� �ݺ�
	{
		Histo[Img[i]]++; // �ش� �ȼ� ���� �ش��ϴ� ������׷� �� ����
	}
}

// ���� ������׷� ��� �Լ�
void ObtainAHistogram(int *Histo, int *AHisto)
{
	for (int i = 0; i < 256; i++) // ��� �ȼ� ��(0~255)�� ���� �ݺ�
	{
		for (int j = 0; j <= i; j++) // ���� �� ���
		{
			AHisto[i] += Histo[j];
		}
	}
}

// ������׷� ��Ȱȭ �Լ�
void HistogramEqualization(BYTE *Img, BYTE *Out, int *AHisto, int W, int H)
{
	int ImgSize = W * H;			  // �̹��� ũ�� ���
	int Nt = W * H, Gmax = 255;		  // �� �ȼ� ��(Nt)�� �ִ� �׷��̽����� ��(Gmax)
	double Ratio = Gmax / (double)Nt; // ����ȭ ���� ���
	BYTE NormSum[256];				  // ����ȭ�� ���� �� �迭
	for (int i = 0; i < 256; i++)	  // ����ȭ�� ���� �� ���
	{
		NormSum[i] = (BYTE)(Ratio * AHisto[i]);
	}
	for (int i = 0; i < ImgSize; i++) // ��� �ȼ��� ���� �ݺ�
	{
		Out[i] = NormSum[Img[i]]; // ����ȭ�� �� ����
	}
}

// �̵�� ���͸����� ����ϴ� �迭 ���� �Լ�
void swap(BYTE *a, BYTE *b)
{
	BYTE temp = *a; // �� ���� ��ȯ
	*a = *b;
	*b = temp;
}

// �̵�� ���͸����� �߾Ӱ� ��� �Լ�
BYTE Median(BYTE *arr, int size)
{
	// �迭�� ������������ ����
	for (int i = 0; i < size - 1; i++) // ���� �ε���
	{
		for (int j = i + 1; j < size; j++) // �� ��� �ε���
		{
			if (arr[i] > arr[j])		// ���� ���� �� ��󺸴� ũ��
				swap(&arr[i], &arr[j]); // �� ���� ��ȯ
		}
	}
	return arr[size / 2]; // ���ĵ� �迭�� �߾Ӱ� ��ȯ
}

// ���� �Լ�
int main()
{
	BITMAPFILEHEADER hf;	// BMP ���� ��� (14����Ʈ)
	BITMAPINFOHEADER hInfo; // BMP ���� ��� (40����Ʈ)
	RGBQUAD hRGB[256];		// ���� �ȷ�Ʈ (256�� ����, 1024����Ʈ)
	FILE *fp;

	// �Է� BMP ���� ����
	fp = fopen("lenna_gauss.bmp", "rb");
	if (fp == NULL) // ������ ������
	{
		printf("File not found!\n"); // ���� �޽��� ���
		return -1;					 // ���α׷� ����
	}

	// BMP ���� ��� �б�
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);

	// �̹��� ũ�� ���
	int ImgSize = hInfo.biWidth * hInfo.biHeight;

	// ���� �޸� �Ҵ�
	BYTE *Image = (BYTE *)malloc(ImgSize);	// ���� �̹���
	BYTE *Temp = (BYTE *)malloc(ImgSize);	// �ӽ� �迭
	BYTE *Output = (BYTE *)malloc(ImgSize); // ��� �̹���

	// BMP �̹��� ������ �б�
	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp); // ���� �ݱ�

	// �̵�� ���͸� ����
	int Length = 3;				 // ���� ũ�� (3x3)
	int Margin = Length / 2;	 // ������ �߽ɿ��� �����ڸ����� �Ÿ�
	int WSize = Length * Length; // ���� �� �ȼ� ���� (3x3 = 9)

	BYTE *temp = (BYTE *)malloc(sizeof(BYTE) * WSize); // ���� �� �ȼ� ���� ������ �迭

	int W = hInfo.biWidth, H = hInfo.biHeight; // �̹����� �ʺ�� ����

	// �̹��� ���� �ȼ��� ���� ���� ����
	for (int i = Margin; i < H - Margin; i++) // �� �������� ��ĵ
	{
		for (int j = Margin; j < W - Margin; j++) // �� �������� ��ĵ
		{
			// ���� ���� ��� �ȼ� ���� temp �迭�� ����
			for (int m = -Margin; m <= Margin; m++) // ������ �� Ž��
			{
				for (int n = -Margin; n <= Margin; n++) // ������ �� Ž��
				{
					temp[(m + Margin) * Length + (n + Margin)] = Image[(i + m) * W + (j + n)];
				}
			}

			// temp �迭���� �߾Ӱ� ��� �� ��� �̹����� ����
			Output[i * W + j] = Median(temp, WSize);
		}
	}

	free(temp); // ���� �޸� ����

	// ��� BMP ���� ����
	FILE *out_fp = fopen("median_filtered.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), out_fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), out_fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, out_fp);
	fwrite(Output, sizeof(BYTE), ImgSize, out_fp);
	fclose(out_fp);

	// ���� �޸� ����
	free(Image);
	free(Output);
	free(Temp);

	return 0; // ���α׷� ����
}