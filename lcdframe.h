#ifndef _LCDFRAME_H_
#define _LCDFRAME_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#include "externals.h"

#define FRAME_H 320
#define FRAME_W 480

#ifdef __cplusplus
extern "C" {
#endif

unsigned char *mem_base;
extern unsigned char *parlcd_mem_base;

extern uint16_t frame [FRAME_H][FRAME_W]; // velikost displeje

void frameToLCD();
int charToFrame(char c, int yRow, int xColumn, uint16_t forecolor, uint16_t backcolor, int zoom);
int strToFrame(char *str, int yRow, int xColumn, uint16_t forecolor, uint16_t backcolor, int zoom);
void clearScreen();
void clearPanel();
void drawSquare(int x, int y, int width, int height, uint32_t color);

struct RGB HSVToRGB(struct HSV hsv);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
