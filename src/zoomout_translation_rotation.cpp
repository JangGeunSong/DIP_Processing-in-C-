#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>

unsigned char** _2dAlloc(int width, int height)
{
	int i;
	unsigned char** ppA = new unsigned char* [height];
	ppA[0] = new unsigned char[width * height];
	for (i = 1; i < height; i++) {
		ppA[i] = ppA[i - 1] + width;
	}

	return ppA;
}

unsigned char** zoomOut(unsigned char** originaImg, int zoomOutScope, int dx, int dy, double radian) {
	int imgSize = 512 / zoomOutScope, step = 0;
    int index = 0, count = 0;
    int i = 0, j = 0;
    int x = 0, y = 0;
    int x_prime = 0, y_prime = 0;
    int row = 0, col = 0;
    int value = 0; 
    // value == ??? 이 값은 unsigned char의 자료형이 0 ~ 256 까지만 표현 가능하므로 음수나 256 이상의 결과가 나올떄 이를 처리할 방법이 없어 정수형으로 받은 후 연산을 처리하기 위해 선언하는 변수이다. 
	unsigned char** result = _2dAlloc(512, 512);
    unsigned char** reSizeImg = _2dAlloc(imgSize, imgSize);
    unsigned char** rotationImg = _2dAlloc(512, 512);
    
	step = 512 / imgSize;

    // 결과값을 리턴할 공간 초기화
    for(i = 0; i < 512; i++){
        for(j = 0; j < 512; j++){
            result[i][j] = 0;
        }
    }

    // 회전 변환 이미지를 저장할 공간 초기화
    for(i = 0; i < 512; i++){
        for(j = 0; j < 512; j++){
            rotationImg[i][j] = 0;
        }
    }

    for(i = 0;i < 512;i = i + step) {
        for(j = 0; j < 512; j = j + step){
            if(i == 0) {
                row = 0;
            }
            else {
                row = i / step;
            }
            if(j == 0) {
                col = 0;
            }
            else {
                col = j / step;
            }
            // Row, Col 값의 경우 step 값으로 나누어 줘야 정확한 사이즈가 얻어진다.
            // 0 0 0
            // X X X    0 부분의 연산을 시행
            // X X X
            if(row - 1 >= 0) {
                // 0 X X
                // X X X    0 부분의 연산을 시행
                // X X X
                if(col - 1 >= 0) {
                    value = value + originaImg[i-1][j-1];
                    count++;
                }
                // X X 0
                // X X X    0 부분의 연산을 시행
                // X X X
                if (col + 1 < 512) {
                    value = value + originaImg[i-1][j+1];
                    count++;
                }
                // X 0 X
                // X X X    0 부분의 연산을 시행
                // X X X
                value = value + originaImg[i-1][j];
                count++;
            }
            // X X X
            // X X X    0 부분의 연산을 시행
            // 0 0 0 
            if(row + 1 < 512) {
                // X X X
                // X X X    0 부분의 연산을 시행
                // 0 X X 
                if(col - 1 >= 0) {
                    value = value + originaImg[i+1][j-1];
                    count++;
                }
                // X X X
                // X X X    0 부분의 연산을 시행
                // X X 0 
                if (col + 1 < 512) {
                    value = value + originaImg[i+1][j+1];
                    count++;
                }
                // X X X
                // X X X    0 부분의 연산을 시행
                // X 0 X 
                value = value + originaImg[i+1][j];
                count++;
            }
            // X X X
            // 0 X X    0 부분의 연산을 시행
            // X X X 
            if(col - 1 >= 0) {
                value = value + originaImg[i][j-1];
                count++;
            }
            // X X X
            // X X 0    0 부분의 연산을 시행
            // X X X 
            if(col + 1 < 512) {
                value = value + originaImg[i][j+1];
                count++;
            }
            // X X X
            // X 0 X    0 부분의 연산을 시행
            // X X X 
            value = value + originaImg[i][j];
            count++;
            // 구한 값에 대한 비트 평균값을 계산
            value = value / count;
            if (value < 0) {
                reSizeImg[row][col] = 0;
            }
            else if (value > 255) {
                reSizeImg[row][col] = 255;
            }
            else {
                reSizeImg[row][col] = value;
            }
            index++;
            count = 0;
        }
    }

    // 위치 이동을 위한 이미지의 시작점을 정의
    x = x + dx;
    y = y + dy;
    // 결과 이미지 바탕에 사이즈가 줄여진 이미지를 입힌다.
    for(i = x; i < x + imgSize; i++) {
        for(j = y; j < y + imgSize; j++) {
            result[i][j] = result[i][j] + reSizeImg[i - x][j - y];
        }
    }
    
    // scaling 한 이미지는 필요 없으므로 메모리 해제
    delete[] reSizeImg[0];
	delete[] reSizeImg;

    // 이미지를 회전
    for(i = x; i < x + imgSize; i++) {
        for(j = y; j < y + imgSize; j++) {
            x_prime = (int)((i * cos(radian)) - (j * sin(radian)));
            y_prime = (int)((i * sin(radian)) + (j * cos(radian)));
            rotationImg[x_prime][y_prime] = result[i][j];
            result[i][j] = 0;
            result[x_prime][y_prime] = rotationImg[x_prime][y_prime];
        }
    }

    // 회전에 쓰인 이미지는 필요 없으므로 메모리 해제
    delete[] rotationImg[0];
    delete[] rotationImg;

    // Hole 채우기
    for(i = 0; i < 512; i++) {
        for(j = 0; j < 512; j++) {
            value = 0;
            if(result[i][j] == 0) {
                if(i - 1 > 0 && result[i - 1][j] > 0) {
                    value = value + result[i - 1][j];
                }
                if(i + 1 < 512 && result[i + 1][j] > 0) {
                    value = value + result[i + 1][j];
                }
                if(j - 1 > 0 && result[i][j - 1] > 0) {
                    value = value + result[i][j - 1];
                }
                if(j + 1 < 512 && result[i][j + 1] > 0) {
                    value = value + result[i][j + 1];
                }
                value = value / 4;
                result[i][j] = value;
            }
        }
    }

	return result;
}

int main() {
	int fileSize = 512, zoomNum = 2;
	FILE* hLena = fopen("lena.img", "rb");
	unsigned char** ppLena = _2dAlloc(fileSize, fileSize);
	fread(ppLena[0], 1, fileSize * fileSize, hLena);
	fclose(hLena);
    // 0.2 rad case
	unsigned char** ppOutputImg = zoomOut(ppLena, zoomNum, 200, 20, 0.2);
    // 0.5 rad case
	// unsigned char** ppOutputImg = zoomOut(ppLena, zoomNum, 200, 20, 0.5);   
	printf("Zoomout, translation and rotation are complete!\n");

	FILE* hOutput = fopen("ZTR.img", "wb");
	fwrite(ppOutputImg[0], 1, fileSize * fileSize, hOutput);
	fclose(hOutput);
	delete[] ppLena[0];
	delete[] ppLena;
	delete[] ppOutputImg[0];
	delete[] ppOutputImg;
	return 0;
}