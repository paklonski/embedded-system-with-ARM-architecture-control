#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#include "externals.h"
#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "lcdframe.h"

void showColor1_Screen(struct RGB RGB_Screen) {
	uint32_t newColorScreen = ((RGB_Screen.R >> 3) << 11) | ((RGB_Screen.G >> 2) << 5) | (RGB_Screen.B >> 3);
	drawSquare(0, 0, 30, 60, newColorScreen);
}

void showColor2_Screen(struct RGB RGB_Screen) {
	uint32_t newColorScreen = ((RGB_Screen.R >> 3) << 11) | ((RGB_Screen.G >> 2) << 5) | (RGB_Screen.B >> 3);
	drawSquare(0, 60, 30, 60, newColorScreen);
}

void showValuesLCD_zoom1x(struct HSV HSV, char *H_String, char *S_String, char *V_String, int xRow)
{
	sprintf(H_String, "%.1f", HSV.H);
	sprintf(S_String, "%.1f", HSV.S);
	sprintf(V_String, "%.1f", HSV.V);

	strToFrame("       ", xRow, 210, 0x0, 0x0, zoomTextLCD);
	strToFrame("       ", xRow, 315, 0x0, 0x0, zoomTextLCD);
	strToFrame("       ", xRow, 405, 0x0, 0x0, zoomTextLCD);

	strToFrame(H_String, xRow, 210, 0xFFFF, 0x0, zoomTextLCD);
	strToFrame(S_String, xRow, 315, 0xFFFF, 0x0, zoomTextLCD);
	strToFrame(V_String, xRow, 405, 0xFFFF, 0x0, zoomTextLCD);

	frameToLCD();
}

void showValuesLCD_zoom2x(struct HSV HSV, char *H_String, char *S_String, char *V_String, int xRow)
{
	sprintf(H_String, "%.1f", HSV.H);
	sprintf(S_String, "%.1f", HSV.S);
	sprintf(V_String, "%.1f", HSV.V);

	strToFrame("          ", xRow, 170, 0x0, 0x0, zoomTextLCD);
	strToFrame("          ", xRow, 280, 0x0, 0x0, zoomTextLCD);
	strToFrame("          ", xRow, 380, 0x0, 0x0, zoomTextLCD);
	
	strToFrame(H_String, xRow, 170, 0xFFFF, 0x0, zoomTextLCD);
	strToFrame(S_String, xRow, 280, 0xFFFF, 0x0, zoomTextLCD);
	strToFrame(V_String, xRow, 380, 0xFFFF, 0x0, zoomTextLCD);

	frameToLCD();
}

bool check_pressed_red(int r_button) {

	if (is_red_pressed && r_button == 0) {
		is_red_pressed = false;
		return false;
	}
	if (!is_red_pressed && r_button == 1) {
		is_red_pressed = true;
		return true;
	}
	return false;
}

bool check_pressed_green(int g_button) {

	if (is_green_pressed && g_button == 0) {
		is_green_pressed = false;
		return false;
	}
	if (!is_green_pressed && g_button == 1) {
		is_green_pressed = true;
		return true;
	}
	return false;
}

bool check_pressed_blue(int b_button) {

	if (is_blue_pressed && b_button == 0) {
		is_blue_pressed = false;
		return false;
	}
	if (!is_blue_pressed && b_button == 1) {
		is_blue_pressed = true;
		return true;
	}
	return false;
}

void setFirst()
{
	zoomTextLCD = 1;

	timeChange = 0;
    timeOn = 0;
    timeOff = 0;
    timeShift = 0;

	LED1.H = 0.0;
	LED1.S = 0.0;
	LED1.V = 0.0;
	LED2.H = 0.0;
	LED2.S = 0.0;
	LED2.V = 0.0;

	LED1_ON = 1;
 	LED2_ON = 1;

	STATIC_MODE = 0;

	CONTINUOUS_MODE = 0;
  	LED1_CONTINUOUS = 0;
 	LED2_CONTINUOUS = 0;
	LEDs_CONTINUOUS = 0;

	FLASHING_MODE = 0;
	LED1_FLASHING = 0;
 	LED2_FLASHING = 0;
	LEDs_FLASHING = 0;

	sprintf(timeString, "%d", timeChange);
    sprintf(timeOnString, "%d", timeOn);
    sprintf(timeOffString, "%d", timeOff);
    sprintf(timeShiftString, "%d", timeShift);
	sprintf(H_String_LED1, "%.1f", LED1.H);
	sprintf(S_String_LED1, "%.1f", LED1.S);
	sprintf(V_String_LED1, "%.1f", LED1.V);
	sprintf(H_String_LED2, "%.1f", LED2.H);
	sprintf(S_String_LED2, "%.1f", LED2.S);
	sprintf(V_String_LED2, "%.1f", LED2.V);
}

void changeHSV(struct HSV *HSV, int r_knob, int g_knob, int b_knob, int redPos, int greenPos, int bluePos)
{
	if (r_knob > redPos || (redPos == 255 && r_knob == 0)) {
		HSV->H = (HSV->H > 359.9) ? 0.0 : HSV->H + 1.0;
	} else if (r_knob < redPos || (redPos == 0 && r_knob == 255)) {
		HSV->H = (HSV->H < 0.1) ? 360.0 : HSV->H - 1.0;
	}

	if (g_knob > greenPos || (greenPos == 255 && g_knob == 0)) {
		HSV->S = (HSV->S > 99.9) ? 0.0 : HSV->S + 0.1;
	} else if (g_knob < greenPos || (greenPos == 0 && g_knob == 255)) {
		HSV->S = (HSV->S < 0.1) ? 100.0 : HSV->S - 0.1;
	}

	if (b_knob > bluePos || (bluePos == 255 && b_knob == 0)) {
		HSV->V = (HSV->V > 99.9) ? 0.0 : HSV->V + 0.1;
	} else if (b_knob < bluePos || (bluePos == 0 && b_knob == 255)) {
		HSV->V = (HSV->V < 0.1) ? 100.0 : HSV->V - 0.1;
	}
}

void copyFromTo(struct HSV *src, struct HSV *dst)
{
	dst->H = src->H;
	dst->S = src->S;
	dst->V = src->V;
}