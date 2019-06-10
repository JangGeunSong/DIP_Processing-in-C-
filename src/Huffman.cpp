#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <math.h>

using namespace std;

struct HuffmanValueBundle {
    int value;
    double probDensity;
    string binary;
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

unsigned char** _2dHuffman(unsigned char** originaImg) {
	unsigned char** result = _2dAlloc(512, 512);
	int m = 0, n = 0, u = 0 , v = 0;
    int x = 0 , y = 0;
    int save = 0;
    int value[512][512] = { 0 };
	
    // step 1 execute DPCM
    for(x = 0; x < 512; x++) {
        for(y = 0; y < 512; y++) {
            if(x == 0 && y == 0) {
                value[x][y] = originaImg[x][y];
            }
            else {
                value[x][y] = originaImg[x][y] - save;
                save = originaImg[x][y];
            }
        }
    }
    printf("DPCM encode complete!\n");

    // step 2 execute huffmancoding and calculate Entropy of the image

    printf("Huffman encode complete!\n");

    // step 2-1 extract the file that huffman encoding result

    printf("Huffman encode result extract complete!\n");
    
    // step 3 decode the huffmancoding

    printf("Huffman encode complete!\n");
    
    // step 4 decode DPCM
    for(x = 0; x < 512; x++) {
        for(y = 0; y < 512; y++) {
            if(x == 0 && y == 0) {
                save = value[x][y];
            }
            else {
                value[x][y] = value[x][y] + save;
                save = value[x][y];
            }
        }
    }
    printf("DPCM decode complete!\n");


    // step 5 attach the decode DPCM value at the result array that return final image
    for(x = 0; x < 512; x++) {
        for(y = 0; y < 512; y++) {
            if(value[x][y] < 0) {
                result[x][y] = 0;
            }
            else if(value[x][y] > 256) {
                result[x][y] = 255;
            }
            else {
                result[x][y] = value[x][y];
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

	unsigned char** ppOutputImg = _2dHuffman(ppLena);
	printf("Huffman coding is complete!");

	FILE* hOutput = fopen("HuffmanResult.img", "wb");
	fwrite(ppOutputImg[0], 1, fileSize * fileSize, hOutput);
	fclose(hOutput);
	delete[] ppLena[0];
	delete[] ppLena;
	delete[] ppOutputImg[0];
	delete[] ppOutputImg;
	return 0;
}