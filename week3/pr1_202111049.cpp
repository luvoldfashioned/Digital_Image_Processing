#pragma warning(disable : 4996) // Visual Studio에서 fopen 사용 시 발생하는 경고를 무시
#include <stdio.h>              // 표준 입출력 함수 사용을 위한 헤더 파일
#include <stdlib.h>             // 동적 메모리 할당 및 기타 유틸리티 함수 사용을 위한 헤더 파일
#include <Windows.h>            // BMP 파일 처리를 위한 Windows API 헤더 파일

int main()
{
    // BMP 파일의 헤더 정보를 저장하기 위한 구조체
    BITMAPFILEHEADER hf;    // BMP 파일 헤더 (파일의 기본 정보, 14바이트)
    BITMAPINFOHEADER hInfo; // BMP 정보 헤더 (이미지의 상세 정보, 40바이트)
    RGBQUAD hRGB[256];      // BMP 팔레트 (256개의 색상, 각 색상은 4바이트)

    FILE *fp; // 파일 포인터 선언

    // BMP 파일 열기 (읽기 모드)
    fp = fopen("lenna.bmp", "rb"); // "lenna.bmp" 파일을 바이너리 읽기 모드로 열기
    if (fp == NULL)                // 파일이 없거나 열 수 없는 경우
    {
        printf("File not found\n"); // 오류 메시지 출력
        return -1;                  // 프로그램 종료 (비정상 종료를 나타내는 -1 반환)
    }

    // BMP 파일의 헤더 정보 읽기
    fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);    // BMP 파일 헤더 읽기
    fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp); // BMP 정보 헤더 읽기
    fread(hRGB, sizeof(RGBQUAD), 256, fp);          // BMP 팔레트 읽기

    // 이미지 크기 계산 (너비 x 높이)
    int ImgSize = hInfo.biWidth * hInfo.biHeight;

    // 동적 메모리 할당 (이미지 데이터와 처리 결과를 저장할 공간)
    BYTE *Image = (BYTE *)malloc(ImgSize);   // 원본 이미지 데이터를 저장할 메모리
    BYTE *output1 = (BYTE *)malloc(ImgSize); // 원본과 동일한 이미지를 저장할 메모리
    BYTE *output2 = (BYTE *)malloc(ImgSize); // 밝기 증가 이미지를 저장할 메모리
    BYTE *output3 = (BYTE *)malloc(ImgSize); // 반전 이미지를 저장할 메모리

    // BMP 이미지 데이터 읽기
    fread(Image, sizeof(BYTE), ImgSize, fp); // 이미지 데이터를 읽어서 메모리에 저장
    fclose(fp);                              // 파일 닫기

    /* --- 영상 처리 --- */

    // output1.bmp: 원본 이미지와 동일한 데이터 복사
    for (int i = 0; i < ImgSize; i++)
        output1[i] = Image[i];

    // output2.bmp: 원본 이미지의 밝기를 50 증가
    for (int i = 0; i < ImgSize; i++)
        output2[i] = Image[i] + 50;

    // output3.bmp: 원본 이미지의 색상을 반전 (255에서 원본 값을 뺌)
    for (int i = 0; i < ImgSize; i++)
        output3[i] = 255 - Image[i];

    /* --- 처리된 이미지를 파일로 저장 --- */

    // output1.bmp 파일 생성 (원본과 동일한 이미지 저장)
    fp = fopen("output1.bmp", "wb");                            // 파일 열기 (쓰기 모드)
    fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);    // BMP 파일 헤더 쓰기
    fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp); // BMP 정보 헤더 쓰기
    fwrite(hRGB, sizeof(RGBQUAD), 256, fp);                     // BMP 팔레트 쓰기
    fwrite(output1, sizeof(BYTE), ImgSize, fp);                 // 이미지 데이터 쓰기
    fclose(fp);                                                 // 파일 닫기

    // output2.bmp 파일 생성 (밝기 증가 이미지 저장)
    fp = fopen("output2.bmp", "wb");
    fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
    fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
    fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
    fwrite(output2, sizeof(BYTE), ImgSize, fp);
    fclose(fp);

    // output3.bmp 파일 생성 (반전 이미지 저장)
    fp = fopen("output3.bmp", "wb");
    fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
    fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
    fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
    fwrite(output3, sizeof(BYTE), ImgSize, fp);
    fclose(fp);

    /* --- 동적 메모리 해제 --- */
    free(Image);   // 원본 이미지 데이터 메모리 해제
    free(output1); // output1 데이터 메모리 해제
    free(output2); // output2 데이터 메모리 해제
    free(output3); // output3 데이터 메모리 해제

    return 0; // 프로그램 정상 종료
}
// end of file