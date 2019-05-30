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

unsigned char** histogramEQ(unsigned char** originaImg) {
	unsigned char** result = _2dAlloc(512, 512);
    int row = 0, col = 0, greyLevel = 0;
    double histogram[256] = {0}, PDF[256] = {0}, CDF[256] = {0};
    double s_min = 0, s_max = 0, value = 0, totalPixel = 512 * 512;
    // 히스토그램
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

    // PDF(Probability Density Function)을 구한다.
    for(row = 0; row < 256; row++) {
        PDF[row] = histogram[row] / totalPixel;
        printf("%d : %lf\n", row, PDF[row]);
    }

    // CDF(Cumulative Density Function)을 구한다.
    for(row = 0; row < 256; row++) {
        for(col = 0; col <= row; col++) {
            CDF[row] = CDF[row] + PDF[col];
        }
    }
    
    // Histogram Equalization을 하기 위해 s_max와 s_min을 구한다.
    s_min = CDF[0];
    s_max = CDF[0];

    for(row = 0; row < 256; row++) {
        if(s_min > CDF[row]) {
            s_min = CDF[row];
        }
        if(s_max < CDF[row]) {
            s_max = CDF[row];
        }
    }
    printf("min: %lf, max: %lf\n", s_min, s_max);
    // CDF를 사용하여 greyLevel을 구한 CDF의 인덱스로 활용하여 픽셀 값을 정한다. 이때 greyLevel - 1을 반드시 해준다. 
    for(row = 0; row < 512; row++) {
        for(col = 0; col < 512; col++) {
            value = 255 * ((CDF[originaImg[row][col]] - s_min) / (s_max - s_min));

            if(value < 0) {
                value = 0;
                result[row][col] = value;
            }
            if(value > 255) {
                value = 255;
                result[row][col] = value;
            }
            else {
                result[row][col] = (unsigned char)value;
            }
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

	unsigned char** ppOutputImg = histogramEQ(ppLena);
	printf("Histogram Equalization is complete!");

	FILE* hOutput = fopen("output.img", "wb");
	fwrite(ppOutputImg[0], 1, fileSize * fileSize, hOutput);
	fclose(hOutput);
	delete[] ppLena[0];
	delete[] ppLena;
	delete[] ppOutputImg[0];
	delete[] ppOutputImg;
	return 0;
}