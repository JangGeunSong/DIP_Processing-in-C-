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
    // To calculate the zoonIn Image pixel value, It need to interpolation calculation (Linear).
    /*  --------------------------> col
        1        A         B
        1            P
        v        C         D
        row
        p = [(col_2 - col) * (row - row_1) / (col_2 - col_1) * (row_2 - row_1)] * B
            + [(col - col_1) * (row - row_1) / (col_2 - col_1) * (row_2 - row_1)] * A
            + [(col_2 - col) * (row_2 - row) / (col_2 - col_1) * (row_2 - row_1)] * D
            + [(col - col_1) * (row_2 - row) / (col_2 - col_1) * (row_2 - row_1)] * C

        의 식을 계산하는 코드를 만든다.
    */
	int imgSize = 512 * zoomInScope;
    int step = 0;
    int row = 0, rowUp = 0, rowBottom = 0;
    int col = 0, colLetf = 0, colRight = 0;
    int A = 0, B = 0, C = 0 , D = 0;
    int upperRowFlag = 0, lowerRowFlag = 0, leftColFlag = 0, rightColFlag = 0;
	int value = 0;
    unsigned char** result = _2dAlloc(imgSize, imgSize);

    int count = 0;

    int** values = new int* [imgSize];
    values[0] = new int [imgSize * imgSize];

    for(row = 1; row < imgSize; row++) {
        values[row] = values[row - 1] + imgSize;
    }
    
    for(row = 0; row < imgSize; row++) {
        for(col = 0; col < imgSize; col++) {
            values[row][col] = -1;
        }
    }
    
    step = zoomInScope;
    
    // 원본 이미지의 Pixel value 를 삽입
    for(row = 0; row < imgSize; row++) {
        for(col = 0; col < imgSize; col++) {
            if(row % step == 0 && col % step == 0) {
                result[row][col] = originaImg[row / step][col / step];
            }
            else {
                values[row][col] = -1;
            }
        }
    }

    // 빈 공간에 pixel 값을 계산해서 삽입
    for(row = 0; row < imgSize; row++) {
        rowUp = row - 1;
        rowBottom = row + 1;
        for(col = 0; col < imgSize; col++) {
            colLetf = col - 1;
            colRight = col + 1;
            if(values[row][col] == -1) {
                if(rowUp >= 0 && values[rowUp][col] != -1) {
                    value = value + values[rowUp][col];
                    count++;
                }
                if(rowBottom < imgSize && values[rowBottom][col] != -1) {
                    value = value + values[rowBottom][col];
                    count++;
                }
                if(colLetf >= 0 && values[row][colLetf] != -1) {
                    value = value + values[row][colLetf];
                    count++;
                }
                if(colLetf < imgSize && values[row][colRight] != -1) {
                    value = value + values[row][colRight];
                    count++;
                }

                value = value / count;

                // 값을 unsigned char 의 범위에 맞게 재 조정.
                if(value < 0) {
                    value = 0;
                    result[row][col] = value;
                }
                else if(value > 256) {
                    value = 255;
                    result[row][col] = value;
                }
                else {
                    result[row][col] = value;
                }
                count = 0;
            }
        }
    }

    delete[] values[0];
    delete[] values; 


    // for(row = 0; row < imgSize; row++) {
    //     A = 1;
    //     B = 1;
    //     C = 1;
    //     D = 1;
    //     // 이미지를 확대 시킬때 비어있는 픽셀을 채우기 위한 보간용 원레 이미지 픽셀 위치를 정하는 연산
    //     // 행을 정할때는 (채우게 될 픽셀의 위치 - 확대 하면서 원레 이미지의 픽셀이 벌어진 거리) 만큼을 계산하여 지정한다.
    //     /*
    //        (픽셀위치 - 벌어진거리) / 벌어진거리    지정픽셀위치    (픽셀위치 + 벌어진거리) / 벌어진거리
    //     */
    //     row_1 = (row - step) / step;
    //     row_2 = (row + step) / step;
    //     // 지정 픽셀 보다 위쪽 행에 있는 픽셀이 정의되지 않았을때
    //     if(row_1 < 0) {
    //         A = -1;
    //         B = -1;
    //         upperRowFlag = 1;
    //     }
    //     else {
    //         upperRowFlag = 0;
    //     }
    //     // 지정 픽셀보다 아래쪽 행에 있는 픽셀이 정의되지 않았을때
    //     if(row_2 >= 512) {
    //         C = -1;
    //         D = -1;
    //         lowerRowFlag = 1;
    //     }
    //     else {
    //         lowerRowFlag = 0;
    //     }
    //     for(col = 0; col< imgSize; col++) {
    //         value = 0;
    //         col_1 = (col - step) / step;
    //         col_2 = (col + step) / step;

    //         if(col_1 < 0){
    //             A = -1;
    //             C = -1;
    //             leftColFlag = 1;
    //         }
    //         else {
    //             leftColFlag = 0;
    //         }
    //         if(col_2 >= 512) {
    //             B = -1;
    //             D = -1;
    //             rightColFlag = 1;
    //         }
    //         else {
    //             rightColFlag = 0;
    //         }

    //         if(A != -1) {
    //             A = originaImg[row_1][col_1];
    //             value = value + ((col - col_1) * (row - row_1) / (col_2 - col_1) * (row_2 - row_1)) * A;
    //         }
    //         if(B != -1) {
    //             B = originaImg[row_1][col_2];
    //             value = value + ((col_2 - col) * (row - row_1) / (col_2 - col_1) * (row_2 - row_1)) * B;
    //         }
    //         if(C != -1) {
    //             C = originaImg[row_2][col_1];
    //             value = value + ((col - col_1) * (row_2 - row) / (col_2 - col_1) * (row_2 - row_1)) * C;
    //         }
    //         if(D != -1) {
    //             D = originaImg[row_2][col_2];
    //             value = value + ((col_2 - col) * (row_2 - row) / (col_2 - col_1) * (row_2 - row_1)) * D;
    //         }
            
    //         A = 1;
    //         B = 1;
    //         C = 1;
    //         D = 1;

    //         if(upperRowFlag) {
    //             A = -1;
    //             B = -1;
    //         }
    //         else {
    //             A = 1;
    //             B = 1;
    //         }
    //         if(lowerRowFlag) {
    //             C = -1;
    //             D = -1;
    //         }
    //         else {
    //             C = 1;
    //             D = 1;
    //         }

    //         if(value < 0) {
    //             value = 0;
    //             result[row][col] = value;
    //         }
    //         else if(value > 256) {
    //             value = 255;
    //             result[row][col] = value;
    //         }
    //         else {
    //             result[row][col] = value;
    //         }
    //     }
    // }

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