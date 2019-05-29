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

unsigned char** zoomIn(unsigned char** originaImg, int zoomInScope) {
    // It needs to use the nearest neiborhood interpolation
    int imgSize = 512 * zoomInScope;
    int step = zoomInScope;
    int upRow = -1, downRow = -1, leftCol = -1, rightCol = -1;
    int row = 0, col = 0;
    double reValue = 0;
    unsigned char** result = _2dAlloc(imgSize, imgSize);
    
    // 만들게 될 값은 double이 될 수 있으므로 우선적으로 double 의 타입을 갖는 value 를 만든다
    double** value = new double* [imgSize];
    value[0] = new double[imgSize * imgSize];
    for(row = 1; row < imgSize; row++) {
        value[row] = value[row - 1] + imgSize;
    }
    // 원본 이미지 픽셀의 값을 확대할 이미지에 집어 넣는다.
    for(row = 0; row < imgSize; row = row + step) {
        for(col = 0; col < imgSize; col = col + step) {
            value[row][col] = originaImg[row / step][col / step];
        }
    }
    // 집어넣은 원본 이미지의 픽셀값을 이용하여 interpolation 을 시행한다.
    for(row = 0; row < imgSize; row++) {
        if(row % step == 0) {
            // 보간에 사용할 점의 위치 지정
            if(row == 0) {
                upRow = row;
                downRow = row + step;
            }
            else if(row == imgSize - step) {
                upRow = row - step;
                downRow = row;
            }
            else {
                upRow = row - step;
                downRow = row + step;
            }
        }
        for(col = 0; col < imgSize; col++) {
            // 보간에 사용할 점의 위치 지정
            if(col == 0) {
                leftCol = col;
                rightCol = col + step;
            }
            else if(col == imgSize - step) {
                leftCol = col - step;
                rightCol = col;
            }
            else {
                leftCol = col - step;
                rightCol = col + step;
            }
            // 보간 계산
            if(row % step == 0) {
                reValue = (value[row][leftCol] / (rightCol - col)) + (value[row][rightCol] / (col - leftCol));
            }
            else if(col % step == 0) {
                reValue = (value[upRow][col] / (downRow - row)) + (value[downRow][col] / (row - upRow));
            }
            else {
                reValue = (((value[upRow][leftCol] / (rightCol - col)) + (value[upRow][rightCol] / (col - leftCol))) / (downRow - row)) + (((value[downRow][leftCol] / (rightCol - col)) + (value[downRow][rightCol] / (col - leftCol))) / (row - upRow));
            }
            // unsigned char 에 맞춰서 값을 조정할 필요가 있다면 조정
            if(reValue < 0) {
                reValue = 0;
                result[row][col] = reValue;
            }
            else if(reValue > 256) {
                reValue = 255;
                result[row][col] = reValue;
            }
            else{
                result[row][col] = reValue;
            }
        }
    }

    // 사용이 끝난 value 배열은 메모리 할당 해제
    delete[] value[0];
    delete[] value;

	return result;
}

int main() {
	int fileSize = 512, zoomNum = 2;
    int chnageSize = fileSize * zoomNum;
	FILE* hLena = fopen("lena.img", "rb");
	unsigned char** ppLena = _2dAlloc(fileSize, fileSize);
	fread(ppLena[0], 1, fileSize * fileSize, hLena);
	fclose(hLena);

	unsigned char** ppOutputImg = zoomIn(ppLena, zoomNum);
	printf("Zoom in is complete!");

	FILE* hOutput = fopen("output.img", "wb");
	fwrite(ppOutputImg[0], 1, chnageSize * chnageSize, hOutput);
	fclose(hOutput);
	delete[] ppLena[0];
	delete[] ppLena;
	delete[] ppOutputImg[0];
	delete[] ppOutputImg;
	return 0;
}