#pragma warning(disable : 4996) // 보안 경고(C4996)를 비활성화
#include <stdio.h>				// 표준 입출력 함수 사용
#include <stdlib.h>				// 동적 메모리 할당 및 rand() 함수 사용
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
	int ImgSize = W * H;			  // 이미지 크기 계산
	for (int i = 0; i < ImgSize; i++) // 모든 픽셀에 대해 반복
	{
		Histo[Img[i]]++; // 해당 픽셀 값에 해당하는 히스토그램 값 증가
	}
}

// 누적 히스토그램 계산 함수
void ObtainAHistogram(int *Histo, int *AHisto)
{
	for (int i = 0; i < 256; i++) // 모든 픽셀 값(0~255)에 대해 반복
	{
		for (int j = 0; j <= i; j++) // 누적 합 계산
		{
			AHisto[i] += Histo[j];
		}
	}
}

// 히스토그램 평활화 함수
void HistogramEqualization(BYTE *Img, BYTE *Out, int *AHisto, int W, int H)
{
	int ImgSize = W * H;			  // 이미지 크기 계산
	int Nt = W * H, Gmax = 255;		  // 총 픽셀 수(Nt)와 최대 그레이스케일 값(Gmax)
	double Ratio = Gmax / (double)Nt; // 정규화 비율 계산
	BYTE NormSum[256];				  // 정규화된 누적 합 배열
	for (int i = 0; i < 256; i++)	  // 정규화된 누적 합 계산
	{
		NormSum[i] = (BYTE)(Ratio * AHisto[i]);
	}
	for (int i = 0; i < ImgSize; i++) // 모든 픽셀에 대해 반복
	{
		Out[i] = NormSum[Img[i]]; // 정규화된 값 적용
	}
}

// 미디언 필터링에서 사용하는 배열 정렬 함수
void swap(BYTE *a, BYTE *b)
{
	BYTE temp = *a; // 두 값을 교환
	*a = *b;
	*b = temp;
}

// 미디언 필터링에서 중앙값 계산 함수
BYTE Median(BYTE *arr, int size)
{
	// 배열을 오름차순으로 정렬
	for (int i = 0; i < size - 1; i++) // 기준 인덱스
	{
		for (int j = i + 1; j < size; j++) // 비교 대상 인덱스
		{
			if (arr[i] > arr[j])		// 현재 값이 비교 대상보다 크면
				swap(&arr[i], &arr[j]); // 두 값을 교환
		}
	}
	return arr[size / 2]; // 정렬된 배열의 중앙값 반환
}

// 메인 함수
int main()
{
	BITMAPFILEHEADER hf;	// BMP 파일 헤더 (14바이트)
	BITMAPINFOHEADER hInfo; // BMP 정보 헤더 (40바이트)
	RGBQUAD hRGB[256];		// 색상 팔레트 (256개 색상, 1024바이트)
	FILE *fp;

	// 입력 BMP 파일 열기
	fp = fopen("lenna_gauss.bmp", "rb");
	if (fp == NULL) // 파일이 없으면
	{
		printf("File not found!\n"); // 오류 메시지 출력
		return -1;					 // 프로그램 종료
	}

	// BMP 파일 헤더 읽기
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);

	// 이미지 크기 계산
	int ImgSize = hInfo.biWidth * hInfo.biHeight;

	// 동적 메모리 할당
	BYTE *Image = (BYTE *)malloc(ImgSize);	// 원본 이미지
	BYTE *Temp = (BYTE *)malloc(ImgSize);	// 임시 배열
	BYTE *Output = (BYTE *)malloc(ImgSize); // 출력 이미지

	// BMP 이미지 데이터 읽기
	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp); // 파일 닫기

	// 미디언 필터링 수행
	int Length = 3;				 // 필터 크기 (3x3)
	int Margin = Length / 2;	 // 필터의 중심에서 가장자리까지 거리
	int WSize = Length * Length; // 필터 내 픽셀 개수 (3x3 = 9)

	BYTE *temp = (BYTE *)malloc(sizeof(BYTE) * WSize); // 필터 내 픽셀 값을 저장할 배열

	int W = hInfo.biWidth, H = hInfo.biHeight; // 이미지의 너비와 높이

	// 이미지 내부 픽셀에 대해 필터 적용
	for (int i = Margin; i < H - Margin; i++) // 행 방향으로 스캔
	{
		for (int j = Margin; j < W - Margin; j++) // 열 방향으로 스캔
		{
			// 필터 내의 모든 픽셀 값을 temp 배열에 저장
			for (int m = -Margin; m <= Margin; m++) // 필터의 행 탐색
			{
				for (int n = -Margin; n <= Margin; n++) // 필터의 열 탐색
				{
					temp[(m + Margin) * Length + (n + Margin)] = Image[(i + m) * W + (j + n)];
				}
			}

			// temp 배열에서 중앙값 계산 후 결과 이미지에 저장
			Output[i * W + j] = Median(temp, WSize);
		}
	}

	free(temp); // 동적 메모리 해제

	// 결과 BMP 파일 저장
	FILE *out_fp = fopen("median_filtered.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), out_fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), out_fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, out_fp);
	fwrite(Output, sizeof(BYTE), ImgSize, out_fp);
	fclose(out_fp);

	// 동적 메모리 해제
	free(Image);
	free(Output);
	free(Temp);

	return 0; // 프로그램 종료
}