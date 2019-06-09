#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979

struct ComplexNum {
	double real;
	double imag;
};

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

unsigned char** _2dDFT(unsigned char** originaImg) {
	unsigned char** result = _2dAlloc(128, 128);
	int m = 0, n = 0;
	int u = 0, v = 0, mnSum = 1;
	double coef = 0, res = 0, totalPixel = 128 * 128; //DCvalue (0,0)의 값.
	double real = 0, imag = 0, valueCos = 0, valueSin = 0, muMul = 0, nvMul = 0;
	double savePixel[128][128] = { 0 };
	struct ComplexNum dftValues[128][128], idftValues[128][128];
    // step 1을 시행
	for (m = 0; m < 128; m++) {
		for (n = 0; n < 128; n++) {
			if ((m + n) % 2 == 0) {
				savePixel[m][n] = originaImg[m][n];
			}
			else {
				savePixel[m][n] = -1 * originaImg[m][n];
			}
			dftValues[m][n].real = 0;
			dftValues[m][n].imag = 0;
		}
	}
	printf("step 1 is complete!! \n");
	// step 2를 시행
	for (u = 0; u < 128; u++) {
		for (v = 0; v < 128; v++) {
			real = 0;
			imag = 0;
			for (m = 0; m < 128; m++) {
				for (n = 0; n < 128; n++) {
					valueCos = (cos(2.0 * PI * (((u * m) + (v * n)) / 128.0)));
					valueSin = (sin(2.0 * PI * (((u * m) + (v * n)) / 128.0)));
                    real = real + (savePixel[m][n] * valueCos);
                    imag = imag + (savePixel[m][n] * valueSin);
				}
			}
			dftValues[u][v].real = real / totalPixel; 
			dftValues[u][v].imag = imag / totalPixel; 
		}
		printf("%d complete!\n", u);
	}
	printf("step 2 is complete!! \n");

	// IDFT를 계산하고 그 값을 double형 배열인 values에 넣는다.
    for(m = 0; m < 128; m++) {
        for(n = 0; n < 128; n++) {
            real = 0;
            imag = 0;
            for(u = 0; u < 128; u++) {
                for(v = 0; v < 128; v++) {
                    muMul = m * u;
                    nvMul = n * v;
                    valueCos = cos(2 * PI * ((muMul + nvMul) / 128.0));
                    valueSin = sin(2 * PI * ((muMul + nvMul) / 128.0));
                    real = real + ((dftValues[u][v].real * valueCos) + (dftValues[u][v].imag * valueSin));
                    imag = imag + ((dftValues[u][v].real * valueSin) + (dftValues[u][v].imag * valueCos));
                }
            }
            idftValues[m][n].real = real;
            idftValues[m][n].imag = imag;
        }
        printf("%d complete\n", m);
    }
    printf("step 4 complete!\n");
    // real part만을 여기서 선택해 얻는다.
    for(m = 0; m < 128; m++) {
        for(n = 0; n < 128; n++) {
            savePixel[m][n] = idftValues[m][n].real;
        }
    }
    // (-1)^m+n을 곱한다.
    for(m = 0; m < 128; m++) {
        for(n = 0; n < 128; n++) {
            mnSum = m + n;
            if(mnSum % 2 == 0) {
                savePixel[m][n] *= 1;
            }
            else {
                savePixel[m][n] *= -1;
            }
        }
    }
    // result에 값을 넣어준다.
    for(m = 0; m < 128; m++) {
        for(n = 0; n < 128; n++) {
            coef = savePixel[m][n];
            if(coef < 0) {
                coef = 0;
                result[m][n] = coef;
            }
            else if(coef > 256) {
                coef = 255;
                result[m][n] = coef;
            }
            else {
                result[m][n] = coef;
            }
        }
    }

	return result;
}

int main() {
	int fileSize = 128;
	FILE* hLena = fopen("lena.img", "rb");
	unsigned char** ppLena = _2dAlloc(fileSize, fileSize);
	fread(ppLena[0], 1, fileSize * fileSize, hLena);
	fclose(hLena);

	unsigned char** ppOutputImg = _2dDFT(ppLena);
	printf("2dIDFT is complete!");

	FILE* hOutput = fopen("recover.img", "wb");
	fwrite(ppOutputImg[0], 1, fileSize * fileSize, hOutput);
	fclose(hOutput);
	delete[] ppLena[0];
	delete[] ppLena;
	delete[] ppOutputImg[0];
	delete[] ppOutputImg;
	return 0;
}