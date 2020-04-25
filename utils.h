#ifndef MENU_PAINTER_2X
#define MENU_PAINTER_2X

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

void showColor1_Screen(struct RGB RGB_Screen);
void showColor2_Screen(struct RGB RGB_Screen);
void showValuesLCD_zoom1x(struct HSV HSV, char *H_String, char *S_String, char *V_String, int xRow);
void showValuesLCD_zoom2x(struct HSV HSV, char *H_String, char *S_String, char *V_String, int xRow);
bool check_pressed_red(int r_button);
bool check_pressed_green(int g_button);
bool check_pressed_blue(int b_button);
void setFirst();
void changeHSV(struct HSV *HSV, int r_knob, int g_knob, int b_knob, int redPos, int greenPos, int bluePos);
void copyFromTo(struct HSV *src, struct HSV *dst);

#endif