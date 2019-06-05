#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>

#define PI 3.141592653589

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

unsigned char** _2dDST(unsigned char** originaImg) {
	unsigned char** result = _2dAlloc(512, 512);
	int m = 0, n = 0;
	int u = 0, v = 0;
    int x = 0, y = 0;
    double coef = 2.0 / 17.0;
	double DCvalue = 1, res = 0, totalPixel = 512 * 512;
	double real = 0, imag = 0, valueSin_1 = 0, valueSin_2 = 0;
	double values[512][512] = { 0 }, IDST[512][512] = { 0 };
    // DST 시행
    for(x = 0; x < 32; x++) {
        printf("%d complete!\n", x);
        for(y = 0; y < 32; y++) {
            for(u = 0; u < 16; u++) {
                for(v = 0; v < 16; v++) {
                    for(m = 0; m < 16; m++) {
                        for(n = 0; n < 16; n++) {
                            valueSin_1 = sin((PI * ((double)m + 1) * ((double)u + 1)) / 17.0);
                            valueSin_2 = sin((PI * ((double)n + 1) * ((double)v + 1)) / 17.0);
                            values[(x * 16) + u][(y * 16) + v] = values[(x * 16) + u][(y * 16) + v] + (coef * originaImg[(x * 16) + m][(y * 16) + n] * valueSin_1 * valueSin_2); 
                        }
                    }
                }
            }
        }
    }
    printf("Step1 complete!\n");
    // IDST를 시행
    for(x = 0; x < 32; x++) {
        printf("%d complete!\n", x);
        for(y = 0; y < 32; y++) {
            for(m = 0; m < 16; m++) {
                for(n = 0; n < 16; n++) {
                    for(u = 0; u < 16; u++) {
                        for(v = 0; v < 16; v++) {
                            valueSin_1 = sin((PI * ((double)m + 1) * ((double)u + 1)) / 17.0);
                            valueSin_2 = sin((PI * ((double)n + 1) * ((double)v + 1)) / 17.0);
                            IDST[(x * 16) + m][(y * 16) + n] = IDST[(x * 16) + m][(y * 16) + n] + (coef * values[(x * 16) + u][(y * 16) + v] * valueSin_1 * valueSin_2); 
                        }
                    }
                }
            }
        }
    }
    printf("Step2 complete!\n");
    for(m = 0; m < 512; m++) {
        for(n = 0; n < 512; n++) {
            if(IDST[m][n] < 0) {
                result[m][n] = 0;
            }
            else if(IDST[m][n] > 256) {
                result[m][n] = 255;
            }
            else {
                result[m][n] = IDST[m][n];
            }
        }
    }
    printf("Step3 complete!\n");
	
	return result;
}

int main() {
	int fileSize = 512;
	FILE* hLena = fopen("lena.img", "rb");
	unsigned char** ppLena = _2dAlloc(fileSize, fileSize);
	fread(ppLena[0], 1, fileSize * fileSize, hLena);
	fclose(hLena);

	unsigned char** ppOutputImg = _2dDST(ppLena);
	printf("2dDCT is complete!");

	FILE* hOutput = fopen("16_IDST.img", "wb");
	fwrite(ppOutputImg[0], 1, fileSize * fileSize, hOutput);
	fclose(hOutput);
	delete[] ppLena[0];
	delete[] ppLena;
	delete[] ppOutputImg[0];
	delete[] ppOutputImg;
	return 0;
}