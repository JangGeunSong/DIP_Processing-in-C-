#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<math.h>

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

unsigned char** shrinkMapping(unsigned char** originaImg) {
	unsigned char** result = _2dAlloc(512, 512);
    int row = 0, col = 0, greyLevel = 0;
    double histogram[256] = {0}, PDF[256] = {0};
    double s_min = 0, s_max = 0, totalPixel = 512 * 512;
    // 히스토그램 && PDF 초기화
    for(greyLevel = 0; greyLevel < 256; greyLevel++) {
        histogram[greyLevel] = 0;
        PDF[greyLevel] = 0;
    }
    // 히스토그램 생성
    for(row = 0; row < 512; row++) {
        for(col = 0; col < 512; col++) {
            greyLevel = originaImg[row][col];
            histogram[greyLevel]++;
        }
    }
    // 히스토그램의 최대값과 최소값을 구함
    s_min = histogram[0];
    s_max = histogram[0];

    for(greyLevel = 0; greyLevel < 256; greyLevel++) {
        if(s_min > histogram[greyLevel]) {
            s_min = histogram[greyLevel];
        }
        if(s_max < histogram[greyLevel]) {
            s_max = histogram[greyLevel];
        }
        PDF[greyLevel] = histogram[greyLevel] / totalPixel;
    }
    // ??
    for(row = 0; row < 512; row++) {
        for(col = 0; col < 512; col++) {
            
        }
    }


	return result;
}

int main() {
	int fileSize = 512;
	FILE* hLena = fopen("lena.img", "rb");
	unsigned char** ppLena = _2dAlloc(fileSize, fileSize);
	fread(ppLena[0], 1, fileSize * fileSize, hLena);
	fclose(hLena);

	unsigned char** ppOutputImg = shrinkMapping(ppLena);
	printf("Mapping is complete!");

	FILE* hOutput = fopen("output.img", "wb");
	fwrite(ppOutputImg[0], 1, fileSize * fileSize, hOutput);
	fclose(hOutput);
	delete[] ppLena[0];
	delete[] ppLena;
	delete[] ppOutputImg[0];
	delete[] ppOutputImg;
	return 0;
}