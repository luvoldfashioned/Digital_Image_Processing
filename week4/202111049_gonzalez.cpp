#include <stdio.h>  // 표준 입출력 함수 사용
#include <stdlib.h> // rand() 함수 사용을 위한 헤더
#include <math.h>   // abs() 함수 사용을 위한 헤더

#define EPSILON 3 // 종료 조건에 해당하는 입실론 값 (경계값 변화 허용 범위)

// 프로그램 시작
int main()
{
    int width = 256, height = 256;       // 예제 이미지의 가로(width)와 세로(height) 크기
    unsigned char image[256][256];       // 입력 이미지 데이터를 저장할 2차원 배열
    unsigned char thresholded[256][256]; // 이진화된 결과 이미지를 저장할 2차원 배열
    int T = 128;                         // 초기 경계값 (밝기의 중간값으로 설정)
    int newT;                            // 새로운 경계값을 저장할 변수
    int sumG1, sumG2, countG1, countG2;  // G1, G2의 합과 개수를 저장할 변수

    // 예제 이미지 데이터 초기화 (랜덤 값으로 설정)
    for (int i = 0; i < height; i++) // 이미지의 각 행에 대해 반복
    {
        for (int j = 0; j < width; j++) // 이미지의 각 열에 대해 반복
        {
            image[i][j] = rand() % 256; // 0~255 사이의 랜덤 밝기 값 생성
        }
    }

    // 반복문을 통해 경계값 T를 계산
    while (1) // 무한 루프 (종료 조건을 만족할 때까지 반복)
    {
        // G1과 G2 초기화
        sumG1 = sumG2 = countG1 = countG2 = 0; // 합과 개수를 0으로 초기화

        // 밝기값 T를 기준으로 G1과 G2로 나누기
        for (int i = 0; i < height; i++) // 이미지의 각 행에 대해 반복
        {
            for (int j = 0; j < width; j++) // 이미지의 각 열에 대해 반복
            {
                if (image[i][j] > T) // 현재 픽셀 값이 T보다 크면
                {
                    sumG1 += image[i][j]; // G1에 해당하는 픽셀 값의 합 계산
                    countG1++;            // G1에 속한 픽셀 개수 증가
                }
                else // 현재 픽셀 값이 T보다 작거나 같으면
                {
                    sumG2 += image[i][j]; // G2에 해당하는 픽셀 값의 합 계산
                    countG2++;            // G2에 속한 픽셀 개수 증가
                }
            }
        }

        // G1과 G2의 평균 밝기 계산
        double mu1 = (countG1 == 0) ? 0 : (double)sumG1 / countG1; // G1의 평균 밝기 계산
        double mu2 = (countG2 == 0) ? 0 : (double)sumG2 / countG2; // G2의 평균 밝기 계산

        // 새로운 경계값 계산
        newT = (int)((mu1 + mu2) / 2); // G1과 G2의 평균 밝기의 중간값을 새로운 경계값으로 설정

        // 종료 조건 확인 (경계값 변화가 입실론 이하일 경우 종료)
        if (abs(newT - T) <= EPSILON) // 새로운 경계값과 이전 경계값의 차이가 EPSILON 이하이면
        {
            break; // 반복문 종료
        }

        // 경계값 업데이트
        T = newT; // 새로운 경계값을 현재 경계값으로 업데이트
    }

    // 최종 경계값 T를 기준으로 이진화 수행
    for (int i = 0; i < height; i++) // 이미지의 각 행에 대해 반복
    {
        for (int j = 0; j < width; j++) // 이미지의 각 열에 대해 반복
        {
            thresholded[i][j] = (image[i][j] > T) ? 255 : 0; // T보다 크면 255(흰색), 작거나 같으면 0(검은색)
        }
    }

    // 결과 출력
    printf("Final Threshold Value: %d\n", T); // 최종 경계값 출력

    return 0; // 프로그램 종료
}