#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EPSILON 3 // 종료 조건에 해당하는 입실론 값

int main()
{
    int width = 256, height = 256;       // 예제 이미지 크기
    unsigned char image[256][256];       // 입력 이미지 데이터 (예제)
    unsigned char thresholded[256][256]; // 이진화된 결과 이미지
    int T = 128;                         // 초기 경계값 (밝기의 중간값으로 설정)
    int newT;
    int sumG1, sumG2, countG1, countG2;

    // 예제 이미지 데이터 초기화 (랜덤 값으로 설정)
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = rand() % 256; // 0~255 사이의 랜덤 값
        }
    }

    while (1)
    {
        // G1과 G2 초기화
        sumG1 = sumG2 = countG1 = countG2 = 0;

        // 밝기값 T를 기준으로 G1과 G2로 나누기
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

        // G1과 G2의 평균 밝기 계산
        double mu1 = (countG1 == 0) ? 0 : (double)sumG1 / countG1;
        double mu2 = (countG2 == 0) ? 0 : (double)sumG2 / countG2;

        // 새로운 경계값 계산
        newT = (int)((mu1 + mu2) / 2);

        // 종료 조건 확인 (경계값 변화가 입실론 이하일 경우 종료)
        if (abs(newT - T) <= EPSILON)
        {
            break;
        }

        // 경계값 업데이트
        T = newT;
    }

    // 최종 경계값 T를 기준으로 이진화 수행
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            thresholded[i][j] = (image[i][j] > T) ? 255 : 0;
        }
    }

    // 결과 출력
    printf("Final Threshold Value: %d\n", T);

    return 0;
}