#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>

#define PI 3.14159

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

double** _2dDoubleAlloc(int width, int height)
{
	int i;
	double** ppA = new double* [height];
	ppA[0] = new double[width * height];
	for (i = 1; i < height; i++) {
		ppA[i] = ppA[i - 1] + width;
	}
	return ppA;
}

unsigned char** _2dDCT(unsigned char** originaImg) {
	unsigned char** result = _2dAlloc(512, 512);
	int m = 0, n = 0;
	int u = 0, v = 0;
    int m_start = 0, n_start = 0, m_end = 0, n_end = 0;
    double alpha_u = 0, alpha_v = 0, alpha_m = 0, alpha_n = 0;
	double coef = 0, DCvalue = 1, res = 0, totalPixel = 512 * 512;
	double real = 0, imag = 0, valueCos_1 = 0, valueCos_2 = 0;
	double values[512][512] = { 0 }, IDCT[512][512] = { 0 };
    // DCT 시행
    for(u = 0; u < 512; u++) {
        if((u % 8) == 0) {
            alpha_u = 1.0 / sqrt(8.0);
            m_start = u;
            m_end = u + 8;
            printf("%d complete! start: %d end: %d \n", u, m_start, m_end);
        }
        else {
            alpha_u = sqrt(2.0) / sqrt(8.0);
        }
        for(v = 0; v < 512; v++) {
            if((v % 8) == 0) {
                alpha_v = 1.0 / sqrt(8.0);
                n_start = v;
                n_end = v + 8;
            }
            else {
                alpha_v = sqrt(2.0) / sqrt(8.0);
            }
            for(m = m_start; m < m_end; m++) {
                for(n = n_start; n < n_end; n++) {
                    valueCos_1 = cos((PI * ((2 * m) + 1) * u) / 16.0);
                    valueCos_2 = cos((PI * ((2 * n) + 1) * v) / 16.0);
                    values[u][v] = values[u][v] + (alpha_u * alpha_v * originaImg[m][n] * valueCos_1 * valueCos_2);
                }
            }
            // printf("%d, %d : %lf\n", u, v, values[u][v]);
        }
    }
    printf("Step1 complete!\n");
    // for(m = m_start; m < m_start + 8; m++) {
    //     for(n = n_start; n < n_start + 8; n++) {
    //         if(values[m][n] < 0) {
    //             values[m][n] = values[m][n] * (-1);
    //         }
    //         values[m][n] = round(values[m][n]);
    //     }
    // }
    // IDCT를 시행
    for(m = 0; m < 512; m++) {
        if((m % 8) == 0) {
            m_start = m;
            printf("%d complete!\n", m);
        }
        for(n = 0; n < 512; n++) {
            if((n % 8) == 0) {
                n_start = n;
            }
            for(u = m_start; u < m_start + 8; u++) {
                if((u % 8) == 0) {
                    alpha_u = 1.0 / sqrt(8);
                }
                else {
                    alpha_u = sqrt(2) / sqrt(8);
                }
                for(v = n_start; v < n_start + 8; v++) {
                    if((v % 8) == 0) {
                        alpha_v = 1.0 / sqrt(8);
                    }
                    else {
                        alpha_v = sqrt(2) / sqrt(8);
                    }
                    valueCos_1 = cos((PI * ((2 * m) + 1) * u) / 16.0);
                    valueCos_2 = cos((PI * ((2 * n) + 1) * v) / 16.0);
                    IDCT[m][n] = IDCT[m][n] + (alpha_u * alpha_v * values[u][v] * valueCos_1 * valueCos_2); 
                }
            }
        }
    }
    printf("Step2 complete!\n");
    for(m = 0; m < 512; m++) {
        for(n = 0; n < 512; n++) {
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
	int fileSize = 512;
	FILE* hLena = fopen("lena.img", "rb");
	unsigned char** ppLena = _2dAlloc(fileSize, fileSize);
	fread(ppLena[0], 1, fileSize * fileSize, hLena);
	fclose(hLena);

	unsigned char** ppOutputImg = _2dDCT(ppLena);
	printf("2dDCT is complete!");

	FILE* hOutput = fopen("8_IDCT.img", "wb");
	fwrite(ppOutputImg[0], 1, fileSize * fileSize, hOutput);
	fclose(hOutput);
	delete[] ppLena[0];
	delete[] ppLena;
	delete[] ppOutputImg[0];
	delete[] ppOutputImg;
	return 0;
}