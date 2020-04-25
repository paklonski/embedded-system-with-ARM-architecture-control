#include "menu_creator.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#include "mzapo_parlcd.h"
#include "mzapo_phys.h"
#include "mzapo_regs.h"
#include "externals.h"
#include "utils.h"
#include "lcdframe.h"
#include "set_static_zoom1x.h"
#include "set_static_zoom2x.h"
#include "set_time.h"
#include "set_continuous_change.h"
#include "set_flashing.h"

int createFlashing_zoom2x()
{
	int new_r_knob;
	int choice = 0;

	rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
	b_knob = rgb_knobs_value & 0xFF;		 // blue knob position
	g_knob = (rgb_knobs_value >> 8) & 0xFF;  // green knob position
	r_knob = (rgb_knobs_value >> 16) & 0xFF; // red knob position

	while (1)
	{
		rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
		new_r_knob = (rgb_knobs_value >> 16) & 0xFF; // new red knob position
		r_button = (rgb_knobs_value >> 26) & 1;		 // red button
		g_button = (rgb_knobs_value >> 25) & 1;		 // green button
		b_button = (rgb_knobs_value >> 24) & 1;		 // blue button

		// TO MOVE THE POINTER
		if (new_r_knob > r_knob + 3) {
			choice++;
			choice = (choice == 8) ? 0 : choice;
			r_knob = new_r_knob;
		} else if (new_r_knob < r_knob - 3) {
			choice--;
			choice = (choice == -1) ? 7 : choice;
			r_knob = new_r_knob;
		}

		strToFrame("TIME 'ON'", 60, 20, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("TIME 'OFF'", 105, 20, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("SHIFT TIME", 150, 20, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("LED1", 195, 30, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("LED2", 195, 150, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("LED1&LED2", 195, 270, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("LEDs+OFFSET", 240, 35, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("STOP ALL", 240, 285, 0xFFFF, 0x0, zoomTextLCD);

		switch (choice) {
			case 0:
				strToFrame("TIME 'ON'", 60, 20, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
					clearPanel();
				    setTime2x(2);
					clearPanel();
				}
				break;
			case 1:
				strToFrame("TIME 'OFF'", 105, 20, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
					clearPanel();
				    setTime2x(3);
					clearPanel();
				}
				break;
			case 2:
				strToFrame("SHIFT TIME", 150, 20, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
					clearPanel();
				    setTime2x(4);
					clearPanel();
				}
				break;
			case 3:
				strToFrame("LED1", 195, 30, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button) && timeOn > 0 && timeOff > 0) {
					FLASHING_MODE = 1;
					int NUMBER_OF_LED = 1;
					pthread_create(&flashingLED1, NULL, manage_flashing, (void *)&NUMBER_OF_LED);
				}
				break;
			case 4:
				strToFrame("LED2", 195, 150, 0xFFFF, 0xDACB, zoomTextLCD);
			 	if (check_pressed_red(r_button) && timeOn > 0 && timeOff > 0) {
			 		FLASHING_MODE = 1;
					int NUMBER_OF_LED = 2;
					pthread_create(&flashingLED2, NULL, manage_flashing, (void *)&NUMBER_OF_LED);
			 	}
				break;
			case 5:
				strToFrame("LED1&LED2", 195, 270, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button) && timeOn > 0 && timeOff > 0) {
					FLASHING_MODE = 1;
					int NUMBER_OF_LED = 3;
					pthread_create(&flashingLEDs, NULL, manage_flashing, (void *)&NUMBER_OF_LED);
				}
				break;
			case 6:
				strToFrame("LEDs+OFFSET", 240, 35, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button) && timeOn > 0 && timeOff > 0 && timeShift > 0) {

					FLASHING_MODE = 1;
					int NUMBER_OF_LED = 1;
					pthread_create(&flashingLEDs, NULL, manage_flashing, (void *)&NUMBER_OF_LED);

					struct timespec start, current;
    				clock_gettime(CLOCK_REALTIME, &start);
					LED2_ON = 0;
					while (1) {
						clock_gettime(CLOCK_REALTIME, &current);
						long int elapsed = (long int)((current.tv_sec - start.tv_sec) * 1000000L + (current.tv_nsec - start.tv_nsec) / 1000);
						if (elapsed >= timeShift*1000000) {
							LED2_ON = 1;
							int NUMBER_OF_LED = 2;
							pthread_create(&flashingLEDs, NULL, manage_flashing, (void *)&NUMBER_OF_LED);
							break;
						}
					}
				}
				break;
			case 7:
				strToFrame("STOP ALL", 240, 285, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button) && timeOn > 0 && timeOff > 0) {
                	FLASHING_MODE = 0;
                }
				break;
			default:
				break;
		}

		strToFrame("Enter time (s)", 15, 240, 0xDACB, 0x0, zoomTextLCD);
        strToFrame(timeOnString, 60, 320, 0xFFFF, 0x0, zoomTextLCD);
        strToFrame(timeOffString, 105, 320, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame(timeShiftString, 150, 320, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("choose", 285, 70, 0xF800, 0x0, zoomTextLCD);
		strToFrame("back", 285, 250, 0x07E0, 0x0, zoomTextLCD);
		frameToLCD();

		// TO EXIT
		if (check_pressed_green(g_button)) {
			clearScreen();
			return 2;
		}
	}
}

int createFlashing_zoom1x()
{
	int new_r_knob;
	int choice = 0;

	rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
	b_knob = rgb_knobs_value & 0xFF;		 // blue knob position
	g_knob = (rgb_knobs_value >> 8) & 0xFF;  // green knob position
	r_knob = (rgb_knobs_value >> 16) & 0xFF; // red knob position

	while (1)
	{
		rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
		new_r_knob = (rgb_knobs_value >> 16) & 0xFF; // new red knob position
		r_button = (rgb_knobs_value >> 26) & 1;		 // red button
		g_button = (rgb_knobs_value >> 25) & 1;		 // green button
		b_button = (rgb_knobs_value >> 24) & 1;		 // blue button

		// TO MOVE THE POINTER
		if (new_r_knob > r_knob + 3) {
			choice++;
			choice = (choice == 8) ? 0 : choice;
			r_knob = new_r_knob;
		} else if (new_r_knob < r_knob - 3) {
			choice--;
			choice = (choice == -1) ? 7 : choice;
			r_knob = new_r_knob;
		}

		strToFrame("TIME 'ON'", 60, 50, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("TIME 'OFF'", 105, 50, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("SHIFT TIME", 150, 50, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("LED1", 205, 80, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("LED2", 205, 200, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("LED1&LED2", 205, 320, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("LEDs+OFFSET", 250, 110, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("STOP ALL", 250, 280, 0xFFFF, 0x0, zoomTextLCD);

		switch (choice) {
			case 0:
				strToFrame("TIME 'ON'", 60, 50, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
				    setTime1x(2);
				}
				break;
			case 1:
				strToFrame("TIME 'OFF'", 105, 50, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
				    setTime1x(3);
				}
				break;
			case 2:
				strToFrame("SHIFT TIME", 150, 50, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
				    setTime1x(4);
				}
				break;
			case 3:
				strToFrame("LED1", 205, 80, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button) && timeOn > 0 && timeOff > 0) {
					FLASHING_MODE = 1;
					int NUMBER_OF_LED = 1;
					pthread_create(&flashingLED1, NULL, manage_flashing, (void *)&NUMBER_OF_LED);
				}
				break;
			case 4:
				strToFrame("LED2", 205, 200, 0xFFFF, 0xDACB, zoomTextLCD);
			 	if (check_pressed_red(r_button) && timeOn > 0 && timeOff > 0) {
			 		FLASHING_MODE = 1;
					int NUMBER_OF_LED = 2;
					pthread_create(&flashingLED2, NULL, manage_flashing, (void *)&NUMBER_OF_LED);
			 	}
				break;
			case 5:
				strToFrame("LED1&LED2", 205, 320, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button) && timeOn > 0 && timeOff > 0) {
					FLASHING_MODE = 1;
					int NUMBER_OF_LED = 3;
					pthread_create(&flashingLEDs, NULL, manage_flashing, (void *)&NUMBER_OF_LED);
				}
				break;
			case 6:
				strToFrame("LEDs+OFFSET", 250, 110, 0xFFFF, 0xDACB, zoomTextLCD);
			 	if (check_pressed_red(r_button) && timeOn > 0 && timeOff > 0 && timeShift > 0) {

					FLASHING_MODE = 1;
					int NUMBER_OF_LED = 1;
					pthread_create(&flashingLEDs, NULL, manage_flashing, (void *)&NUMBER_OF_LED);

					struct timespec start, current;
    				clock_gettime(CLOCK_REALTIME, &start);
					LED2_ON = 0;
					while (1) {
						clock_gettime(CLOCK_REALTIME, &current);
						long int elapsed = (long int)((current.tv_sec - start.tv_sec) * 1000000L + (current.tv_nsec - start.tv_nsec) / 1000);
						if (elapsed >= timeShift*1000000) {
							LED2_ON = 1;
							int NUMBER_OF_LED = 2;
							pthread_create(&flashingLEDs, NULL, manage_flashing, (void *)&NUMBER_OF_LED);
							break;
						}
					}
				}
				break;
			case 7:
				strToFrame("STOP ALL", 250, 280, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button) && timeOn > 0 && timeOff > 0) {
                	FLASHING_MODE = 0;
                }
				break;
			default:
				break;
		}

		strToFrame("Enter the time in seconds", 15, 200, 0xFFFF, 0x0, zoomTextLCD);
        strToFrame(timeOnString, 60, 320, 0xFFFF, 0x0, zoomTextLCD);
        strToFrame(timeOffString, 105, 320, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame(timeShiftString, 150, 320, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("- - - - - - - - - - - - - - - - - - - - - - - - -", 175, 50, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("                                                         ", 300, 50, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("Press GREEN to open Menu...", 300, 160, 0xFFFF, 0x0, zoomTextLCD);
		frameToLCD();

		// TO EXIT
		if (check_pressed_green(g_button))
		{
			clearScreen();
			return 1;
		}
	}
}

int createContinuousChange_zoom1x()
{
	int new_r_knob;
	int choice = 0;

	rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
	b_knob = rgb_knobs_value & 0xFF;		 // blue knob position
	g_knob = (rgb_knobs_value >> 8) & 0xFF;  // green knob position
	r_knob = (rgb_knobs_value >> 16) & 0xFF; // red knob position

	while (1)
	{
		rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
		new_r_knob = (rgb_knobs_value >> 16) & 0xFF; // new red knob position
		r_button = (rgb_knobs_value >> 26) & 1;		 // red button
		g_button = (rgb_knobs_value >> 25) & 1;		 // green button
		b_button = (rgb_knobs_value >> 24) & 1;		 // blue button

		// TO MOVE THE POINTER
		if (new_r_knob > r_knob + 3) {
			choice++;
			choice = (choice == 8) ? 0 : choice;
			r_knob = new_r_knob;
		} else if (new_r_knob < r_knob - 3) {
			choice--;
			choice = (choice == -1) ? 7 : choice;
			r_knob = new_r_knob;
		}

		strToFrame("COLOUR 1", 60, 50, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("COLOUR 2", 105, 50, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("TIME", 150, 50, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("s", 150, 355, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("LED1", 205, 80, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("LED2", 205, 200, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("LED1&LED2", 205, 320, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("ANTI-PHASE", 250, 110, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("STOP ALL", 250, 280, 0xFFFF, 0x0, zoomTextLCD);

		switch (choice) {
			case 0:
				strToFrame("COLOUR 1", 60, 50, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
				    setStatic_zoom1x_LED1();
					STATIC_MODE = ((LED1.H == 0.0 && LED1.S == 0.0 && LED1.V == 0.0) && (LED2.H == 0.0 && LED2.S == 0.0 && LED2.V == 0.0)) ? 0 : 1;
				}
				break;
			case 1:
				strToFrame("COLOUR 2", 105, 50, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
				    setStatic_zoom1x_LED2();
					STATIC_MODE = ((LED1.H == 0.0 && LED1.S == 0.0 && LED1.V == 0.0) && (LED2.H == 0.0 && LED2.S == 0.0 && LED2.V == 0.0)) ? 0 : 1;
				}
				break;
			case 2:
				strToFrame("TIME", 150, 50, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
				    setTime1x(1);
				}
				break;
			case 3:
				strToFrame("LED1", 205, 80, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button) && timeChange > 0) {
					STATIC_MODE = 0;
					CONTINUOUS_MODE = 1;
					LED1_CONTINUOUS = 1;
					pthread_create(&threadLED1, NULL, playLED, NULL);
				}
				break;
			case 4:
				strToFrame("LED2", 205, 200, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button) && timeChange > 0) {
					STATIC_MODE = 0;
					CONTINUOUS_MODE = 1;
					LED2_CONTINUOUS = 1;
					pthread_create(&threadLED2, NULL, playLED, NULL);
				}
				break;
			case 5:
				strToFrame("LED1&LED2", 205, 320, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button) && timeChange > 0) {
					STATIC_MODE = 0;
					CONTINUOUS_MODE = 1;
					LEDs_CONTINUOUS = 1;
					pthread_create(&threadLEDs, NULL, playLED, NULL);
				}
				break;
			case 6:
				strToFrame("ANTI-PHASE", 250, 110, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button) && timeChange > 0) {
					STATIC_MODE = 0;
					CONTINUOUS_MODE = 1;
                	pthread_create(&threadLEDs_in_antiphase, NULL, play_LED_in_antiphase, NULL);
				}
				break;
			case 7:
				strToFrame("STOP ALL", 250, 280, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button) && timeChange > 0) {
					STATIC_MODE = 1;
					CONTINUOUS_MODE = 0;
					LED1_CONTINUOUS = 0;
					LED2_CONTINUOUS = 0;
					LEDs_CONTINUOUS = 0;
                }
				break;
			default:
				break;
		}

		showColor1_Screen(RGB_LED1);
		showColor2_Screen(RGB_LED2);

		strToFrame("HUE", 15, 210, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("SATUR", 15, 300, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("VALUE", 15, 400, 0xFFFF, 0x0, zoomTextLCD);

		strToFrame(H_String_LED1, 60, 210, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame(S_String_LED1, 60, 315, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame(V_String_LED1, 60, 405, 0xFFFF, 0x0, zoomTextLCD);

		strToFrame(H_String_LED2, 105, 210, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame(S_String_LED2, 105, 315, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame(V_String_LED2, 105, 405, 0xFFFF, 0x0, zoomTextLCD);

		strToFrame(timeString, 150, 320, 0xFFFF, 0x0, zoomTextLCD);

		strToFrame("- - - - - - - - - - - - - - - - - - - - - - - - -", 175, 50, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("                                                         ", 300, 50, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("Press GREEN to open Menu...", 300, 160, 0xFFFF, 0x0, zoomTextLCD);

		frameToLCD();

		// TO EXIT
		if (check_pressed_green(g_button))
		{
			clearScreen();
			return 1;
		}
	}
}

int createContinuousChange_zoom2x()
{
	int new_r_knob;
	int choice = 0;

	rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
	b_knob = rgb_knobs_value & 0xFF;		 // blue knob position
	g_knob = (rgb_knobs_value >> 8) & 0xFF;  // green knob position
	r_knob = (rgb_knobs_value >> 16) & 0xFF; // red knob position

	while (1)
	{
		rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
		new_r_knob = (rgb_knobs_value >> 16) & 0xFF; // new red knob position
		r_button = (rgb_knobs_value >> 26) & 1;		 // red button
		g_button = (rgb_knobs_value >> 25) & 1;		 // green button
		b_button = (rgb_knobs_value >> 24) & 1;		 // blue button

		// TO MOVE THE POINTER
		if (new_r_knob > r_knob + 3) {
			choice++;
			choice = (choice == 8) ? 0 : choice;
			r_knob = new_r_knob;
		} else if (new_r_knob < r_knob - 3) {
			choice--;
			choice = (choice == -1) ? 7 : choice;
			r_knob = new_r_knob;
		}

		showColor1_Screen(RGB_LED1);
		showColor2_Screen(RGB_LED2);

		strToFrame("COL 1", 60, 30, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("COL 2", 105, 30, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("TIME", 150, 30, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("LED1", 195, 30, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("LED2", 195, 145, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("LED1&LED2", 195, 260, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("ANTI-PHASE", 240, 30, 0xFFFF, 0x0, zoomTextLCD);
        strToFrame("STOP ALL", 240, 285, 0xFFFF, 0x0, zoomTextLCD);

		switch (choice) {
			case 0:
				strToFrame("COL 1", 60, 30, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
					clearPanel();
					setStatic_zoom2x_LED1();
					STATIC_MODE = ((LED1.H == 0.0 && LED1.S == 0.0 && LED1.V == 0.0) && (LED2.H == 0.0 && LED2.S == 0.0 && LED2.V == 0.0)) ? 0 : 1;
					clearPanel();
				}
				break;
			case 1:
				strToFrame("COL 2", 105, 30, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
					clearPanel();
					setStatic_zoom2x_LED2();
					STATIC_MODE = ((LED1.H == 0.0 && LED1.S == 0.0 && LED1.V == 0.0) && (LED2.H == 0.0 && LED2.S == 0.0 && LED2.V == 0.0)) ? 0 : 1;
					clearPanel();
				}
				break;
			case 2:
				strToFrame("TIME", 150, 30, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
					clearPanel();
					setTime2x(1);
					clearPanel();
				}
				break;
			case 3:
                strToFrame("LED1", 195, 30, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button) && timeChange > 0) {
					STATIC_MODE = 0;
					CONTINUOUS_MODE = 1;
					LED1_CONTINUOUS = 1;
					pthread_create(&threadLED1, NULL, playLED, NULL);
				}
				break;
			case 4:
		        strToFrame("LED2", 195, 145, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button) && timeChange > 0) {
					STATIC_MODE = 0;
					CONTINUOUS_MODE = 1;
					LED2_CONTINUOUS = 1;
					pthread_create(&threadLED2, NULL, playLED, NULL);
				}
				break;
			case 5:
		        strToFrame("LED1&LED2", 195, 260, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button) && timeChange > 0) {
					STATIC_MODE = 0;
					CONTINUOUS_MODE = 1;
					LEDs_CONTINUOUS = 1;
					pthread_create(&threadLEDs, NULL, playLED, NULL);
				}
				break;
			case 6:
		        strToFrame("ANTI-PHASE", 240, 30, 0xFFFF, 0xDACB, zoomTextLCD);
                if (check_pressed_red(r_button) && timeChange > 0) {
					STATIC_MODE = 0;
					CONTINUOUS_MODE = 1;
                	pthread_create(&threadLEDs_in_antiphase, NULL, play_LED_in_antiphase, NULL);
                }
				break;
            case 7:
		        strToFrame("STOP ALL", 240, 285, 0xFFFF, 0xDACB, zoomTextLCD);
                if (check_pressed_red(r_button) && timeChange > 0) {
					STATIC_MODE = 1;
					CONTINUOUS_MODE = 0;
					LED1_CONTINUOUS = 0;
					LED2_CONTINUOUS = 0;
					LEDs_CONTINUOUS = 0;
                }
				break;
			default:
				break;
		}

		strToFrame("HUE", 10, 170, 0xDACB, 0x0, zoomTextLCD);
		strToFrame("SAT", 10, 280, 0xDACB, 0x0, zoomTextLCD);
		strToFrame("VAL", 10, 380, 0xDACB, 0x0, zoomTextLCD);
		strToFrame(H_String_LED1, 60, 170, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame(S_String_LED1, 60, 280, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame(V_String_LED1, 60, 380, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame(H_String_LED2, 105, 170, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame(S_String_LED2, 105, 280, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame(V_String_LED2, 105, 380, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame(timeString, 150, 300, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("choose", 285, 70, 0xF800, 0x0, zoomTextLCD);
		strToFrame("back", 285, 250, 0x07E0, 0x0, zoomTextLCD);
		frameToLCD();

		// TO EXIT
		if (check_pressed_green(g_button))
		{
			clearScreen();
			return 2;
		}
	}
}

int createStaticLight_zoom1x()
{
	int new_r_knob;
	int choice = 0;

	rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
	b_knob = rgb_knobs_value & 0xFF;		 // blue knob position
	g_knob = (rgb_knobs_value >> 8) & 0xFF;  // green knob position
	r_knob = (rgb_knobs_value >> 16) & 0xFF; // red knob position

	while (1)
	{
		rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
		new_r_knob = (rgb_knobs_value >> 16) & 0xFF; // new red knob position
		r_button = (rgb_knobs_value >> 26) & 1;		 // red button
		g_button = (rgb_knobs_value >> 25) & 1;		 // green button
		b_button = (rgb_knobs_value >> 24) & 1;		 // blue button

		// TO MOVE THE POINTER
		if (new_r_knob > r_knob + 3) {
			choice++;
			choice = (choice == 5) ? 0 : choice;
			r_knob = new_r_knob;
		} else if (new_r_knob < r_knob - 3) {
			choice--;
			choice = (choice == -1) ? 4 : choice;
			r_knob = new_r_knob;
		}
		
		showColor1_Screen(RGB_LED1);
		showColor2_Screen(RGB_LED2);

		strToFrame("HUE", 15, 210, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("SATUR", 15, 300, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("VALUE", 15, 400, 0xFFFF, 0x0, zoomTextLCD);

		strToFrame(H_String_LED1, 60, 210, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame(S_String_LED1, 60, 315, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame(V_String_LED1, 60, 405, 0xFFFF, 0x0, zoomTextLCD);

		strToFrame(H_String_LED2, 105, 210, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame(S_String_LED2, 105, 315, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame(V_String_LED2, 105, 405, 0xFFFF, 0x0, zoomTextLCD);

		strToFrame("- - - - - - - - - - - - - - -", 150, 210, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("- - - - - - - - - - - - - - -", 195, 210, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("- - - - - - - - - - - - - - -", 240, 210, 0xFFFF, 0x0, zoomTextLCD);

		strToFrame("LED 1", 60, 50, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("LED 2", 105, 50, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("LED1&LED2", 150, 50, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("COPY 1->2", 195, 50, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("COPY 2->1", 240, 50, 0xFFFF, 0x0, zoomTextLCD);

		switch (choice) {
			case 0:
				strToFrame("LED 1", 60, 50, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
				    setStatic_zoom1x_LED1();
				}
				break;
			case 1:
				strToFrame("LED 2", 105, 50, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
				    setStatic_zoom1x_LED2();
				}
				break;
			case 2:
				strToFrame("LED1&LED2", 150, 50, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
				    setStatic_zoom1x_TOGETHER();
				}
				break;
			case 3:
				strToFrame("COPY 1->2", 195, 50, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
				    setStatic_zoom1x_COPY(1);
				}
				break;
			case 4:
				strToFrame("COPY 2->1", 240, 50, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
				    setStatic_zoom1x_COPY(2);
				}
				break;
			default:
				break;
		}

		strToFrame("Press GREEN to open Menu...", 300, 160, 0xFFFF, 0x0, zoomTextLCD);
		frameToLCD();
				
		// TO EXIT TO MAIN MENU
		if (check_pressed_green(g_button))
		{
			STATIC_MODE = ((LED1.H == 0.0 && LED1.S == 0.0 && LED1.V == 0.0) && (LED2.H == 0.0 && LED2.S == 0.0 && LED2.V == 0.0)) ? 0 : 1;
			clearScreen();
			return 1;
		}
	}
}

int createStaticLight_zoom2x()
{
	int new_r_knob;
	int choice = 0;

	rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
	b_knob = rgb_knobs_value & 0xFF;		 // blue knob position
	g_knob = (rgb_knobs_value >> 8) & 0xFF;  // green knob position
	r_knob = (rgb_knobs_value >> 16) & 0xFF; // red knob position

	while (1)
	{

		rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
		new_r_knob = (rgb_knobs_value >> 16) & 0xFF; // new red knob position
		r_button = (rgb_knobs_value >> 26) & 1;		 // red button
		g_button = (rgb_knobs_value >> 25) & 1;		 // green button
		b_button = (rgb_knobs_value >> 24) & 1;		 // blue button

		// TO MOVE THE POINTER
		if (new_r_knob > r_knob + 3) {
			choice++;
			choice = (choice == 5) ? 0 : choice;
			r_knob = new_r_knob;
		} else if (new_r_knob < r_knob - 3) {
			choice--;
			choice = (choice == -1) ? 4 : choice;
			r_knob = new_r_knob;
		}

		showColor1_Screen(RGB_LED1);
		showColor2_Screen(RGB_LED2);

		strToFrame("HUE", 10, 170, 0xDACB, 0x0, zoomTextLCD);
		strToFrame("SAT", 10, 280, 0xDACB, 0x0, zoomTextLCD);
		strToFrame("VAL", 10, 380, 0xDACB, 0x0, zoomTextLCD);

		strToFrame(H_String_LED1, 60, 170, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame(S_String_LED1, 60, 280, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame(V_String_LED1, 60, 380, 0xFFFF, 0x0, zoomTextLCD);

		strToFrame(H_String_LED2, 105, 170, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame(S_String_LED2, 105, 280, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame(V_String_LED2, 105, 380, 0xFFFF, 0x0, zoomTextLCD);

		strToFrame("LED 1", 60, 30, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("LED 2", 105, 30, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("- - - - - - - - - - - - - - - - - -", 170, 30, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("1&2", 220, 30, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("1->2", 220, 210, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("2->1", 220, 380, 0xFFFF, 0x0, zoomTextLCD);

		switch (choice) {
			case 0:
				strToFrame("LED 1", 60, 30, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
					clearPanel();
					setStatic_zoom2x_LED1();
					clearPanel();
				}
				break;
			case 1:
				strToFrame("LED 2", 105, 30, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
					clearPanel();
					setStatic_zoom2x_LED2();
					clearPanel();
				}
				break;
			case 2:
				strToFrame("1&2", 220, 30, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
					clearPanel();
					setStatic_zoom2x_TOGETHER();
					clearPanel();
				}
				break;
			case 3:
				strToFrame("1->2", 220, 210, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
					setStatic_zoom2x_COPY(1);
				}
				break;
			case 4:
				strToFrame("2->1", 220, 380, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
					setStatic_zoom2x_COPY(2);
				}
				break;
			default:
				break;
			}

		strToFrame("choose", 285, 70, 0xF800, 0x0, zoomTextLCD);
		strToFrame("back", 285, 250, 0x07E0, 0x0, zoomTextLCD);
		frameToLCD();

		// TO EXIT TO MAIN MENU
		if (check_pressed_green(g_button))
		{
			STATIC_MODE = ((LED1.H == 0.0 && LED1.S == 0.0 && LED1.V == 0.0) && (LED2.H == 0.0 && LED2.S == 0.0 && LED2.V == 0.0)) ? 0 : 1;
			clearScreen();
			return 2;
		}
	}
}

int createMenu_zoom1x()
{
	int new_r_knob;
	int choice = 0;

	rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
	b_knob = rgb_knobs_value & 0xFF;		 // blue knob position
	g_knob = (rgb_knobs_value >> 8) & 0xFF;  // green knob position
	r_knob = (rgb_knobs_value >> 16) & 0xFF; // red knob position

	while (1) {

		rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
		new_r_knob = (rgb_knobs_value >> 16) & 0xFF; // new red knob position
		r_button = (rgb_knobs_value >> 26) & 1;		 // red buttom
		b_button = (rgb_knobs_value >> 24) & 1;		 // blue button

		// TO MOVE THE POINTER
		if (new_r_knob > r_knob + 3) {
			choice++;
			choice = (choice == 4) ? 0 : choice;
			r_knob = new_r_knob;
		} else if (new_r_knob < r_knob - 3) {
			choice--;
			choice = (choice == -1) ? 3 : choice;
			r_knob = new_r_knob;
		}

		strToFrame("STATIC LIGHT", 70, 170, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("CONTINUOUS CHANGE", 120, 135, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("FLASHING", 170, 190, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("EXIT", 220, 220, 0xFFFF, 0x0, zoomTextLCD);

		switch (choice) {
			case 0:
				strToFrame("STATIC LIGHT", 70, 170, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
					clearScreen();
					return 3;
				}
				break;
			case 1:
				strToFrame("CONTINUOUS CHANGE", 120, 135, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
					clearScreen();
					return 5;
				}
				break;
			case 2:
				strToFrame("FLASHING", 170, 190, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
					clearScreen();
					return 7;
				}
				break;
			case 3:
				strToFrame("EXIT", 220, 220, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
					clearScreen();
					return 0;
				}
				break;
			default:
				break;
			}

		strToFrame("Press BLUE to zoom...", 300, 240, 0xFFFF, 0x0, zoomTextLCD);
		frameToLCD();

		// REDRAWING TO 2x ZOOM
		if (check_pressed_blue(b_button))
		{
			clearScreen();
			zoomTextLCD = 2;
			return 2;
		}
	}
}

int createMenu_zoom2x()
{
	int new_r_knob;
	int choice = 0;

	rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
	b_knob = rgb_knobs_value & 0xFF;		 // blue knob position
	g_knob = (rgb_knobs_value >> 8) & 0xFF;  // green knob position
	r_knob = (rgb_knobs_value >> 16) & 0xFF; // red knob position

	while (1) {

		rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
		new_r_knob = (rgb_knobs_value >> 16) & 0xFF; // new red knob position
		r_button = (rgb_knobs_value >> 26) & 1;		 // red buttom
		b_button = (rgb_knobs_value >> 24) & 1;		 // blue button

		// TO MOVE THE POINTER
		if (new_r_knob > r_knob + 3) {
			choice++;
			choice = (choice == 4) ? 0 : choice;
			r_knob = new_r_knob;
		} else if (new_r_knob < r_knob - 3) {
			choice--;
			choice = (choice == -1) ? 3 : choice;
			r_knob = new_r_knob;
		}

		strToFrame("STATIC LIGHT", 70, 130, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("CONTIN. CHANGE", 120, 100, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("FLASHING", 170, 160, 0xFFFF, 0x0, zoomTextLCD);
		strToFrame("EXIT", 220, 210, 0xFFFF, 0x0, zoomTextLCD);

		switch (choice) {
			case 0:
				strToFrame("STATIC LIGHT", 70, 130, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
					clearScreen();
					return 4;
				}
				break;
			case 1:
				strToFrame("CONTIN. CHANGE", 120, 100, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
					clearScreen();
					return 6;
				}
				break;
			case 2:
				strToFrame("FLASHING", 170, 160, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
					clearScreen();
					return 8;
				}
				break;
			case 3:
				strToFrame("EXIT", 220, 210, 0xFFFF, 0xDACB, zoomTextLCD);
				if (check_pressed_red(r_button)) {
					clearScreen();
					return 0;
				}
				break;
			default:
				break;
		}

		strToFrame("choose", 285, 70, 0xF800, 0x0, zoomTextLCD);
		strToFrame("zoom", 285, 380, 0x001F, 0x0, zoomTextLCD);
		frameToLCD();

		// REDRAWING TO 1x ZOOM
		if (check_pressed_blue(b_button))
		{
			clearScreen();
			zoomTextLCD = 1;
			return 1;
		}
	}
}