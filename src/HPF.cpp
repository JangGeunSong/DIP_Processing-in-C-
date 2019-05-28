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

int** _2FilterAlloc(int width, int height)
{
	int i;
	int** Filter = new int* [height];
	Filter[0] = new int[width * height];
	for (i = 1; i < height; i++) {
		Filter[i] = Filter[i - 1] + width;
	}
    // 수직
    Filter[0][0] = 1;
    Filter[0][1] = 0;
    Filter[0][2] = -1;
    Filter[1][0] = 2;
    Filter[1][1] = 0;
    Filter[1][2] = -2;
    Filter[2][0] = 1;
    Filter[2][1] = 0;
    Filter[2][2] = -1;
    
    // 수평
    // Filter[0][0] = 1;
    // Filter[0][1] = 2;
    // Filter[0][2] = 1;
    // Filter[1][0] = 0;
    // Filter[1][1] = 0;
    // Filter[1][2] = 0;
    // Filter[2][0] = -1;
    // Filter[2][1] = -2;
    // Filter[2][2] = -1;

	return Filter;
}

unsigned char** zoomOut(unsigned char** originaImg, int **Filter) {
	unsigned char** result = _2dAlloc(512, 512);
    int value = 0;
    int count = 0;

    for(int i = 0; i < 512; i++) {
        for(int j = 0; j < 512; j++) {
            if(i - 1 > 0) {
                if(j - 1 > 0) {
                    value = value + (originaImg[i - 1][j - 1] * Filter[0][0]);
                    count++;
                }
                if(j + 1 < 512) {
                    value = value + (originaImg[i - 1][j + 1] * Filter[0][2]);
                    count++;
                }
                value = value + (originaImg[i - 1][j] * Filter[0][1]);
                count++;
            }

            if(i + 1 < 512) {
                if(j - 1 > 0) {
                    value = value + (originaImg[i + 1][j - 1] * Filter[2][0]);
                    count++;
                }
                if(j + 1 < 512) {
                    value = value + (originaImg[i + 1][j + 1] * Filter[2][2]);
                    count++;
                }
                value = value + (originaImg[i + 1][j] * Filter[2][1]);
                count++;
            }

            value = value + (originaImg[i][j] * Filter[1][1]);
            count++;
            if(j - 1 > 0) {
                value = value + (originaImg[i][j - 1] * Filter[1][0]);
                count++;
            }
            if(j + 1 < 512) {
                value = value + (originaImg[i][j + 1]* Filter[1][2]);
                count++;
            }
            
            if (value < 0) {
                result[i][j] = 0;
            }
            else if (value > 255) {
                result[i][j] = 255;
            }
            else {
                result[i][j] = value;
            }
            count = 0;
            value = 0;
        }
    }

	return result;
}

int main() {
	int fileSize = 512;
	FILE* hLena = fopen("lena.img", "rb");
    int** Filter = _2FilterAlloc(512, 512);
	unsigned char** ppLena = _2dAlloc(fileSize, fileSize);
	fread(ppLena[0], 1, fileSize * fileSize, hLena);
	fclose(hLena);

	unsigned char** ppOutputImg = zoomOut(ppLena, Filter);
	printf("HPF is complete!");

	FILE* hOutput = fopen("output.img", "wb");
	fwrite(ppOutputImg[0], 1, 512 * 512, hOutput);
	fclose(hOutput);
	delete[] ppLena[0];
	delete[] ppLena;
	delete[] ppOutputImg[0];
	delete[] ppOutputImg;
    delete[] Filter[0];
    delete[] Filter;
	return 0;
}