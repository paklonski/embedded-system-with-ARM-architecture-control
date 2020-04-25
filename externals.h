#ifndef EXTERNALS
#define EXTERNALS

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

typedef struct RGB {
	unsigned char R;
	unsigned char G;
	unsigned char B;
} RGB;

typedef struct HSV {
	double H;
	double S;
	double V;
} HSV;

pthread_t threadLED1;
pthread_t threadLED2;
pthread_t threadLEDs;
pthread_t threadLEDs_in_antiphase;
pthread_t flashingLED1;
pthread_t flashingLED2;
pthread_t flashingLEDs;
pthread_t flashing_shifted;

volatile int LED1_ON;
volatile int LED2_ON;

volatile int STATIC_MODE;

volatile int FLASHING_MODE;
volatile int LED1_FLASHING;
volatile int LED2_FLASHING;
volatile int LEDs_FLASHING;

volatile int CONTINUOUS_MODE;
volatile int LED1_CONTINUOUS;
volatile int LED2_CONTINUOUS;
volatile int LEDs_CONTINUOUS;

bool is_red_pressed;
bool is_green_pressed;
bool is_blue_pressed;

int zoomTextLCD;
int timeChange, timeOn, timeOff, timeShift;
uint32_t rgb_knobs_value;
int r_knob, g_knob, b_knob;
int r_button, g_button, b_button;

int redKnobLED1, greenKnobLED1, blueKnobLED1, redKnobLED2, greenKnobLED2, blueKnobLED2;

struct HSV LED1;
struct HSV LED2;
struct RGB RGB_LED1;
struct RGB RGB_LED2;

char H_String_LED1[5];
char S_String_LED1[5];
char V_String_LED1[5];
char H_String_LED2[5];
char S_String_LED2[5];
char V_String_LED2[5];

char timeString[5];
char timeOnString[5]; 
char timeOffString[5]; 
char timeShiftString[5]; 

#endif