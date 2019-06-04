#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979

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
	unsigned char** result = _2dAlloc(128, 128);
	int m = 0, n = 0;
	int u = 0, v = 0;
    double coef = 2.0 / 129.0;
	double DCvalue = 1, res = 0, totalPixel = 128 * 128;
	double real = 0, imag = 0, valueSin_1 = 0, valueSin_2 = 0;
	double values[128][128] = { 0 }, IDST[128][128] = { 0 };
    // DCT 시행
    for(u = 0; u < 128; u++) {
        for(v = 0; v < 128; v++) {
            for(m = 0; m < 128; m++) {
                for(n = 0; n < 128; n++) {
                    valueSin_1 = sin((PI * (m + 1) * (u + 1)) / 129.0);
                    valueSin_2 = sin((PI * (n + 1) * (v + 1)) / 129.0);
                    values[u][v] = values[u][v] + (coef * originaImg[m][n] * valueSin_1 * valueSin_2); 
                }
            }
        }
        printf("%d complete!\n", u);
    }
    printf("Step1 complete!\n");
    // IDST를 시행
    for(m = 0; m < 128; m++) {
        for(n = 0; n < 128; n++) {
            for(u = 0; u < 128; u++) {
                for(v = 0; v < 128; v++) {
                    valueSin_1 = sin((PI * (m + 1) * (u + 1)) / 129.0);
                    valueSin_2 = sin((PI * (n + 1) * (v + 1)) / 129.0);
                    IDST[m][n] = IDST[m][n] + (coef * values[u][v] * valueSin_1 * valueSin_2); 
                }
            }
        }
        printf("%d complete!\n", m);
    }
    printf("Step2 complete!\n");
    for(m = 0; m < 128; m++) {
        for(n = 0; n < 128; n++) {
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
	int fileSize = 128;
	FILE* hLena = fopen("lena.img", "rb");
	unsigned char** ppLena = _2dAlloc(fileSize, fileSize);
	fread(ppLena[0], 1, fileSize * fileSize, hLena);
	fclose(hLena);

	unsigned char** ppOutputImg = _2dDST(ppLena);
	printf("2dDCT is complete!");

	FILE* hOutput = fopen("IDST.img", "wb");
	fwrite(ppOutputImg[0], 1, fileSize * fileSize, hOutput);
	fclose(hOutput);
	delete[] ppLena[0];
	delete[] ppLena;
	delete[] ppOutputImg[0];
	delete[] ppOutputImg;
	return 0;
}