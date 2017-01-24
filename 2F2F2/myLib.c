// Name: Ryan Murphy
// Game: 2FAT2FURIOUS2

#include "myLib.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

void setPixel(int row, int col, volatile u16 color) {
	videoBuffer[OFFSET(row,col, 240)] = color;
}

void drawRect(int row, int col, int height, int width, volatile u16 color) {
	for(int r = 0; r < height; r++) {
		REG_DMA3SAD = (u32)&color;
		REG_DMA3DAD = (u32)(&videoBuffer[OFFSET(row + r, col, 240)]);
		REG_DMA3CNT = width | DMA_ON | DMA_SOURCE_FIXED;
	}
}

void drawImage3(int row, int col, int width, int height, const u16 *image) {

	for (int r = 0; r < height; r++) {
		REG_DMA3SAD = (u32)&image[OFFSET(r, 0, width)];
		REG_DMA3DAD = (u32)(&videoBuffer[OFFSET(row + r, col, 240)]);
		REG_DMA3CNT = width | DMA_ON;
	}
}

void waitForVblank() {
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

void drawChar(int row, int col, char ch, volatile u16 color) {
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 6; c++) {
			if (fontdata_6x8[OFFSET(r, c, 6) + ch * 48]) {
				setPixel(row + r, col + c, color);
			}
		}
	}
}

void drawString(int row, int col, char *str, volatile u16 color) {
	while (*str) {
		drawChar(row, col, *str++, color);
		col += 6;
		
	}
}