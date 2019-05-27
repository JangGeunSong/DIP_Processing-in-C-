#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

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

unsigned char** zoomOut(unsigned char** originaImg, int zoomOutScope) {
	int imgSize = 512 / zoomOutScope, step = 0;
    int index = 0, count = 0;
    int row = 0, col = 0;
    int value = 0; 
    // value == ??? 이 값은 unsigned char의 자료형이 0 ~ 256 까지만 표현 가능하므로 음수나 256 이상의 결과가 나올떄 이를 처리할 방법이 없어 정수형으로 받은 후 연산을 처리하기 위해 선언하는 변수이다. 
	unsigned char** result = _2dAlloc(imgSize, imgSize);
    
	step = 512 / imgSize;

    for(int i = 0;i < 512;i = i + step) {
        for(int j = 0; j < 512; j = j + step){
            if(i == 0) {
                row = 0;
            }
            else {
                row = i / 2;
            }
            if(j == 0) {
                col = 0;
            }
            else {
                col = j / 2;
            }
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
                result[row][col] = 0;
            }
            else if (value > 255) {
                result[row][col] = 255;
            }
            else {
                result[row][col] = value;
            }
            index++;
            count = 0;
        }
    }

	return result;
}

int main() {
	int fileSize = 512, zoomNum = 2;
    int chnageSize = fileSize / zoomNum;
	FILE* hLena = fopen("lena.img", "rb");
	unsigned char** ppLena = _2dAlloc(fileSize, fileSize);
	fread(ppLena[0], 1, fileSize * fileSize, hLena);
	fclose(hLena);

	unsigned char** ppOutputImg = zoomOut(ppLena, zoomNum);
	printf("Zoom out is complete!");

	FILE* hOutput = fopen("output.img", "wb");
	fwrite(ppOutputImg[0], 1, chnageSize * chnageSize, hOutput);
	fclose(hOutput);
	delete[] ppLena[0];
	delete[] ppLena;
	delete[] ppOutputImg[0];
	delete[] ppOutputImg;
	return 0;
}