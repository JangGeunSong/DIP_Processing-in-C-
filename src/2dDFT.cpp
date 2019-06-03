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
	int u = 0, v = 0;
	double coef = 0, DCvalue = 1, res = 0, totalPixel = 128 * 128; //DCvalue (0,0)의 값.
	double real = 0, imag = 0, valueCos = 0, valueSin = 0;
	double savePixel[128][128] = { 0 };
	struct ComplexNum values[128][128];
	// step 1을 시행
	for (m = 0; m < 128; m++) {
		for (n = 0; n < 128; n++) {
			if ((m + n) % 2 == 0) {
				savePixel[m][n] = originaImg[m][n];
			}
			else {
				savePixel[m][n] = -1 * originaImg[m][n];
			}
			values[m][n].real = 0;
			values[m][n].imag = 0;
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
			values[u][v].real = real / totalPixel; 
			values[u][v].imag = imag / totalPixel; 
		}
		printf("%d complete!\n", u);
	}
	printf("step 2 is complete!! \n");

    DCvalue = sqrt(pow(values[64][64].real, 2) + pow(values[64][64].imag, 2));

	for (u = 0; u < 128; u++) {
		for (v = 0; v < 128; v++) {
			coef = sqrt(pow(values[u][v].real, 2) + pow(values[u][v].imag, 2));
			res = 255.0 * log10(coef + 1) / log10(DCvalue + 1);
			if(res < 0) {
				res = 0;
				result[u][v] = res;
			}
			else if(res > 256) {
				res = 255;
				result[u][v] = res;
			}
			else {
				result[u][v] = res;
			}
		}
	}
	printf("step 3 is complete!! \n");

	return result;
}

int main() {
	int fileSize = 128;
	FILE* hLena = fopen("lena.img", "rb");
	unsigned char** ppLena = _2dAlloc(fileSize, fileSize);
	fread(ppLena[0], 1, fileSize * fileSize, hLena);
	fclose(hLena);

	unsigned char** ppOutputImg = _2dDFT(ppLena);
	printf("2dDFT is complete!");

	FILE* hOutput = fopen("output.img", "wb");
	fwrite(ppOutputImg[0], 1, fileSize * fileSize, hOutput);
	fclose(hOutput);
	delete[] ppLena[0];
	delete[] ppLena;
	delete[] ppOutputImg[0];
	delete[] ppOutputImg;
	return 0;
}