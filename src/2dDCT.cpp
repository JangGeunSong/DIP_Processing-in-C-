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

unsigned char** _2dDCT(unsigned char** originaImg) {
	unsigned char** result = _2dAlloc(128, 128);
	int m = 0, n = 0;
	int u = 0, v = 0;
    double alpha_u = 0, alpha_v = 0, alpha_m = 0, alpha_n = 0;
	double coef = 0, DCvalue = 1, res = 0, totalPixel = 128 * 128;
	double real = 0, imag = 0, valueCos_1 = 0, valueCos_2 = 0;
	double values[128][128] = { 0 }, IDCT[128][128] = { 0 };
    // DCT 시행
    for(u = 0; u < 128; u++) {
        if(u == 0) {
            alpha_u = 1.0 / sqrt(128);
        }
        else {
            alpha_u = sqrt(2) / sqrt(128);
        }
        for(v = 0; v < 128; v++) {
            if(v == 0) {
                alpha_v = 1.0 / sqrt(128);
            }
            else {
                alpha_v = sqrt(2) / sqrt(128);
            }
            for(m = 0; m < 128; m++) {
                for(n = 0; n < 128; n++) {
                    valueCos_1 = cos((PI * ((2 * m) + 1) * u) / 256.0);
                    valueCos_2 = cos((PI * ((2 * n) + 1) * v) / 256.0);
                    values[u][v] = values[u][v] + (alpha_u * alpha_v * originaImg[m][n] * valueCos_1 * valueCos_2); 
                }
            }
        }
        printf("%d complete!\n", u);
    }
    printf("Step1 complete!\n");
    // IDCT를 시행
    for(m = 0; m < 128; m++) {
        for(n = 0; n < 128; n++) {
            for(u = 0; u < 128; u++) {
                if(u == 0) {
                    alpha_u = 1.0 / sqrt(128);
                }
                else {
                    alpha_u = sqrt(2) / sqrt(128);
                }
                for(v = 0; v < 128; v++) {
                    if(v == 0) {
                        alpha_v = 1.0 / sqrt(128);
                    }
                    else {
                        alpha_v = sqrt(2) / sqrt(128);
                    }
                    valueCos_1 = cos((PI * ((2 * m) + 1) * u) / 256.0);
                    valueCos_2 = cos((PI * ((2 * n) + 1) * v) / 256.0);
                    IDCT[m][n] = IDCT[m][n] + (alpha_u * alpha_v * values[u][v] * valueCos_1 * valueCos_2); 
                }
            }
        }
        printf("%d complete!\n", m);
    }
    printf("Step2 complete!\n");
    for(m = 0; m < 128; m++) {
        for(n = 0; n < 128; n++) {
            if(IDCT[m][n] < 0) {
                result[m][n] = 0;
            }
            else if(IDCT[m][n] > 256) {
                result[m][n] = 255;
            }
            else {
                result[m][n] = IDCT[m][n];
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

	unsigned char** ppOutputImg = _2dDCT(ppLena);
	printf("2dDCT is complete!");

	FILE* hOutput = fopen("DCT.img", "wb");
	fwrite(ppOutputImg[0], 1, fileSize * fileSize, hOutput);
	fclose(hOutput);
	delete[] ppLena[0];
	delete[] ppLena;
	delete[] ppOutputImg[0];
	delete[] ppOutputImg;
	return 0;
}