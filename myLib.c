#include <stdlib.h>
#include "myLib.h"

u16* videoBuffer = (u16*) 0x6000000;

void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

void setPixel(int x, int y, u16 color)
{
    *(videoBuffer + OFFSET2(x, y)) = color;
}

void drawRectangle(int r, int c, int width, int height, u16 color)
{
	for (int i = 0; i < height; i++) {
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET(r + i, c, 240)];
		DMA[3].cnt = DMA_ON | DMA_SOURCE_FIXED | width;
	}
}

void drawImage3(int r, int c, int width, int height, const u16* image)
{
	/*
	for(int i = 0; i < height; i++) { //row
        for(int j = 0; j < width; j++) { //col
            videoBuffer[(x + i)*240 + (y + j)] = *image++; //every x in every row, every y in every column
        }
    }
    */
	for (int i = 0; i < height; i++)
	{
		DMA[3].src = &image[OFFSET(i, 0, width)];
		DMA[3].dst = videoBuffer + OFFSET(r+i, c, 240);
		DMA[3].cnt = width | DMA_ON;
	}
}


