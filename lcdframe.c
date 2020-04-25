#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include <pthread.h>

#include "externals.h"
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "lcdframe.h"
#include "font_types.h"

#define round(x) (int)(x + 0.5)

unsigned char *parlcd_mem_base = NULL;
uint16_t frame[FRAME_H][FRAME_W]; // velikost displeje

void frameToLCD()

{
	// To begin drawing, move LCD pixel pointer to point 0,0 - top left corner 
	*(volatile uint16_t*)(parlcd_mem_base + PARLCD_REG_CMD_o) = 0x2c; // command
	
	// budu cist po 2 prvcich
	volatile uint32_t* ptr = (volatile uint32_t*) frame;
	volatile uint32_t* dst = (volatile uint32_t*) (parlcd_mem_base + PARLCD_REG_DATA_o);
	
	int i;
	// jeden prvek smycky bude trvat 200ns
	for (i = 0; i < FRAME_H*(FRAME_W/2); i++) {
		*dst = *ptr++;
	}
}

// fonts: opt/apo/lcd/fonts/
int charToFrame(char c, int yRow, int xColumn, uint16_t forecolor, uint16_t backcolor, int zoom) 
{
	// mezera je prvni znak ve fontu
	int cIdx = c - ' ';
	int w = font_winFreeSystem14x16.width[cIdx] * zoom + 4; // aby za tim fontem byla mezera na 4 pozice
	int y, x; 
	// vim ze velikost znaku je 16
	for (y = 0; y < 16*zoom; y+=zoom) {
		uint16_t mask = font_winFreeSystem14x16.bits[16*cIdx+y/zoom];
		// w je sirka znaku
		for (x = 0; x < w; x+=zoom) {
			frame[yRow+y][xColumn+x] = (mask & 0x8000) ? forecolor : backcolor;
			if (zoom == 2) {
				frame[yRow+y+1][xColumn+x] = (mask & 0x8000) ? forecolor : backcolor;
				frame[yRow+y][xColumn+x+1] = (mask & 0x8000) ? forecolor : backcolor;
				frame[yRow+y+1][xColumn+x+1] = (mask & 0x8000) ? forecolor : backcolor;
			}
			mask <<= 1; // mask = mask << 1; rotuju o 1
		}
	}
	return w; // sirka vytisteneho znaku	
}

int strToFrame(char *str, int yRow, int xColumn, uint16_t forecolor, uint16_t backcolor, int zoom) 
{
	char c;
	int w = 0;
	while((c=*str++) != 0) {
		w += charToFrame(c, yRow, xColumn+w, forecolor, backcolor, zoom);
	}
	return w; // pixelova sirka napsaneho textu, abychom vedeli, kde muzeme psat dalsi znak
}

void clearScreen() 
{
	for (int i = 0; i < FRAME_H ; i++) {
		for (int j = 0; j < FRAME_W ; j++) {
			frame[i][j] = 0x0;
		}
	}
	frameToLCD();
}

void drawSquare(int x, int y, int width, int height, uint32_t color) 
{
	for (int i = x; i < x+width ; i++) {
		for (int j = y; j < y+height ; j++) {
			frame[i][j] = color;
		}
	}
}

void clearPanel() 
{
	for (int i = 285; i < FRAME_H; i++) {
		for (int j = 0; j < FRAME_W ; j++) {
			frame[i][j] = 0x0;
		}
	}
	frameToLCD();
}


// the other functions
struct RGB HSVToRGB(struct HSV hsv) {

	double hue = hsv.H;
	double saturation = hsv.S / 100;
	double value = hsv.V / 100;

	double r = 0, g = 0, b = 0;

	if (saturation == 0)
	{
		r = value;
		g = value;
		b = value;
	}
	else
	{
		int i;
		double f, p, q, t;

		if (hue == 360)
			hue = 0;
		else
			hue = hue / 60;

		i = (int)trunc(hue);
		f = hue - i;

		p = value * (1.0 - saturation);
		q = value * (1.0 - (saturation * f));
		t = value * (1.0 - (saturation * (1.0 - f)));

		switch (i)
		{
		case 0:
			r = value;
			g = t;
			b = p;
			break;

		case 1:
			r = q;
			g = value;
			b = p;
			break;

		case 2:
			r = p;
			g = value;
			b = t;
			break;

		case 3:
			r = p;
			g = q;
			b = value;
			break;

		case 4:
			r = t;
			g = p;
			b = value;
			break;

		default:
			r = value;
			g = p;
			b = q;
			break;
		}

	}

	struct RGB rgb;
	rgb.R = (unsigned char)round(r * 255);
	rgb.G = (unsigned char)round(g * 255);
	rgb.B = (unsigned char)round(b * 255);

	return rgb;
}










