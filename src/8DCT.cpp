#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>

#define PI 3.1415926535

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
	unsigned char** result = _2dAlloc(512, 512);
	int m = 0, n = 0;
	int u = 0, v = 0;
    int x = 0, y = 0;
    int m_start = 0, n_start = 0, m_end = 0, n_end = 0;
    double alpha_u = 0, alpha_v = 0;
	double valueCos_1 = 0, valueCos_2 = 0;
	double values[512][512] = { 0 }, IDCT[512][512] = { 0 };
    // DCT 시행
    for(x = 0; x < 64; x++){
        printf("%d complete!\n", x);
        for(y = 0; y < 64; y++){
            for(u = 0; u < 8; u++) {
                if(u == 0) {
                    alpha_u = 1.0 / sqrt(8.0);
                }
                else {
                    alpha_u = sqrt(2.0) / sqrt(8.0);
                }
                for(v = 0; v < 8; v++) {
                    if(v == 0) {
                        alpha_v = 1.0 / sqrt(8.0);
                    }
                    else {
                        alpha_v = sqrt(2.0) / sqrt(8.0);
                    }
                    for(m = 0; m < 8; m++) {
                        for(n = 0; n < 8; n++) {
                            valueCos_1 = cos((PI * ((2 * (double)m) + 1) * (double)u) / 16.0);
                            valueCos_2 = cos((PI * ((2 * (double)n) + 1) * (double)v) / 16.0);
                            values[(x * 8) + u][(y * 8) + v] = values[(x * 8) + u][(y * 8) + v] + (alpha_u * alpha_v * originaImg[(x * 8) + m][(y * 8) + n] * valueCos_1 * valueCos_2);
                        }
                    }
                    // printf("%d, %d : %lf\n", u, v, values[u][v]);
                }
            }
        }
    }
    printf("Step1 complete!\n");
    // IDCT를 시행
    // for(x = 0; x < 64; x++) {
    //     printf("%d complete!\n", x);
    //     for(y = 0; y < 64; y++) {
    //         for(m = 0; m < 8; m++) {
    //             for(n = 0; n < 8; n++) {
    //                 for(u = 0; u < 8; u++) {
    //                     if(u == 0) {
    //                         alpha_u = 1.0 / sqrt(8);
    //                     }
    //                     else {
    //                         alpha_u = sqrt(2) / sqrt(8);
    //                     }
    //                     for(v = 0; v < 8; v++) {
    //                         if(v == 0) {
    //                             alpha_v = 1.0 / sqrt(8);
    //                         }
    //                         else {
    //                             alpha_v = sqrt(2) / sqrt(8);
    //                         }
    //                         valueCos_1 = cos((PI * ((2 * (double)m) + 1) * (double)u) / 16.0);
    //                         valueCos_2 = cos((PI * ((2 * (double)n) + 1) * (double)v) / 16.0);
    //                         IDCT[(x * 8) + m][(y * 8) + n] = IDCT[(x * 8) + m][(y * 8) + n] + (alpha_u * alpha_v * values[(x * 8) + u][(y * 8) + v] * valueCos_1 * valueCos_2); 
    //                     }
    //                 }
    //             }
    //         }
    //     }
    // }
    printf("Step2 complete!\n");
    for(m = 0; m < 512; m++) {
        for(n = 0; n < 512; n++) {
            if(values[m][n] < 0) {
                result[m][n] = 0;
            }
            else if(values[m][n] > 256) {
                result[m][n] = 255;
            }
            else {
                result[m][n] = values[m][n];
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

	unsigned char** ppOutputImg = _2dDCT(ppLena);
	printf("2dDCT is complete!");

	FILE* hOutput = fopen("8_DCT.img", "wb");
	fwrite(ppOutputImg[0], 1, fileSize * fileSize, hOutput);
	fclose(hOutput);
	delete[] ppLena[0];
	delete[] ppLena;
	delete[] ppOutputImg[0];
	delete[] ppOutputImg;
	return 0;
}