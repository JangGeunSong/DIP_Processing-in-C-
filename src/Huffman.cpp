#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <math.h>

using namespace std;

struct HuffmanTree {
    double p;
    int greyLevel;
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
	int index = 0, totalPixel = 512 * 512;
    int x = 0 , y = 0, level = 0;
    int save = 0;
    int min = 0, max = 0;
    int value[512][512] = { 0 }; // DPCM value
    int hist[256] = { 0 };
    HuffmanTree PDF[256] = { 0 }, dummy;
    double maxP = 0, Entropy = 0, enSave = 0;

    // step 1 execute DPCM
    for(x = 0; x < 512; x++) {
        for(y = 0; y < 512; y++) {
            if(x == 0 && y == 0) {
                value[x][y] = originaImg[x][y];
                save = originaImg[x][y];
            }
            else {
                value[x][y] = originaImg[x][y] - save;
                save = originaImg[x][y];
            }
        }
    }
    printf("DPCM encode complete!\n");
    min = value[0][0];
    max = value[0][1];
    // step 2 execute huffmancoding and calculate Entropy of the image
    for(x = 0; x < 512; x++) {
        for(y = 1; y < 512; y++) {
            if(min > value[x][y]) {
                min = value[x][y];
            }
            if(max < value[x][y]) {
                max = value[x][y];
            }
        }
    }
    printf("min %d, max %d\n", min, max);

    for(x = 0; x < 512; x++) {
        for(y = 0; y < 512; y++) {
            if(x == 0 && y == 0) {
                level = value[x][y];
                hist[level]++;
            }
            else {
                level = value[x][y] - min;
                hist[level]++;
            }
        }
    }

    // Initailize PDF value --> probability
    for(x = 0; x < 256; x++) {
        PDF[x].p = -1;
    }

    // step 1- 1 calculate Entropy
    for(x = 0; x < 256; x++) {
        PDF[x].greyLevel = x;
        PDF[x].p = (double)hist[x] / totalPixel;
    }
    
    for(x = 0; x < 256; x++) {
        if(PDF[x].p != -1) {
            enSave = -1 * log2(PDF[x].p) * PDF[x].p;
            Entropy = Entropy + enSave;
        }
    }

    printf("Entropy %lf\n", Entropy);

    for(x = 0; x < 256; x++) {
        maxP = PDF[x].p;
        for(y = x; y < 256; y++) {
            if(maxP < PDF[y].p) {
                maxP = PDF[y].p;
                dummy = PDF[y];
                PDF[y] = PDF[x];
                PDF[x] = dummy;
            }
        }
    }


    printf("Huffman encode complete!\n");

    // step 2-1 extract the file that huffman encoding result

    printf("Huffman encode result extract complete!\n");
    
    // step 3 decode the huffmancoding result

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