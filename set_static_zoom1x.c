#include "set_static_zoom1x.h"

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

void setStatic_zoom1x_COPY(int from) 
{
	while (1) {

		rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
		r_button = (rgb_knobs_value >> 26) & 1;  // red button

		if (r_button == 0) {
			(from == 1) ? copyFromTo(&LED1, &LED2) : copyFromTo(&LED2, &LED1);
			RGB_LED1 = HSVToRGB(LED1);
			RGB_LED2 = HSVToRGB(LED2);
			showValuesLCD_zoom1x(LED1, H_String_LED1, S_String_LED1, V_String_LED1, 60);
			showValuesLCD_zoom1x(LED2, H_String_LED2, S_String_LED2, V_String_LED2, 105);
			uint32_t newColorLED1 = RGB_LED1.R << 16 | RGB_LED1.G << 8 | RGB_LED1.B;
			uint32_t newColorLED2 = RGB_LED2.R << 16 | RGB_LED2.G << 8 | RGB_LED2.B;
			*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o) = newColorLED1;
			*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB2_o) = newColorLED2;
			break;
		}
	}
}

void setStatic_zoom1x_TOGETHER() 
{
	rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
	int redPos = (rgb_knobs_value >> 16) & 0xFF;  // new red knob position
	int greenPos = (rgb_knobs_value >> 8) & 0xFF; // new red knob position
	int bluePos = (rgb_knobs_value)&0xFF;		  // new red knob position

	while (1) {

		rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
		r_knob = (rgb_knobs_value >> 16) & 0xFF; // new red knob position
		g_knob = (rgb_knobs_value >> 8) & 0xFF;  // new red knob position
		b_knob = (rgb_knobs_value)&0xFF;		 // new red knob position
		b_button = (rgb_knobs_value >> 24) & 1;  // blue button
		r_button = (rgb_knobs_value >> 26) & 1;  // red button

		if (r_button == 0) {

			changeHSV(&LED1, r_knob, g_knob, b_knob, redPos, greenPos, bluePos);
			changeHSV(&LED2, r_knob, g_knob, b_knob, redPos, greenPos, bluePos);
			RGB_LED1 = HSVToRGB(LED1);
			RGB_LED2 = HSVToRGB(LED2);
			showColor1_Screen(RGB_LED1);
			showColor2_Screen(RGB_LED2);
			showValuesLCD_zoom1x(LED1, H_String_LED1, S_String_LED1, V_String_LED1, 60);
			showValuesLCD_zoom1x(LED2, H_String_LED2, S_String_LED2, V_String_LED2, 105);
			uint32_t newColorLED1 = RGB_LED1.R << 16 | RGB_LED1.G << 8 | RGB_LED1.B;
			uint32_t newColorLED2 = RGB_LED2.R << 16 | RGB_LED2.G << 8 | RGB_LED2.B;
			*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o) = newColorLED1;
			*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB2_o) = newColorLED2;
			redPos = r_knob;
			greenPos = g_knob;
			bluePos = b_knob;
			strToFrame("                                 ", 300, 140, 0x0, 0x0, zoomTextLCD);
			strToFrame("Press BLUE to apply changes...", 300, 140, 0xFFFF, 0x0, zoomTextLCD);
			if (b_button == 1) {
				strToFrame("                                ", 300, 100, 0xFFFF, 0x0, zoomTextLCD);
				break;
			}

		}
	}
}

void setStatic_zoom1x_LED2() 
{
	rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
	int redPos = (rgb_knobs_value >> 16) & 0xFF;  // new red knob position
	int greenPos = (rgb_knobs_value >> 8) & 0xFF; // new red knob position
	int bluePos = (rgb_knobs_value)&0xFF;		  // new red knob position

	while (1) {

		rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
		r_knob = (rgb_knobs_value >> 16) & 0xFF; // new red knob position
		g_knob = (rgb_knobs_value >> 8) & 0xFF;  // new red knob position
		b_knob = (rgb_knobs_value)&0xFF;		 // new red knob position
		b_button = (rgb_knobs_value >> 24) & 1;  // blue button
		r_button = (rgb_knobs_value >> 26) & 1;  // red button

		if (r_button == 0) {
			changeHSV(&LED2, r_knob, g_knob, b_knob, redPos, greenPos, bluePos);
			RGB_LED2 = HSVToRGB(LED2);
			showColor2_Screen(RGB_LED2);
			showValuesLCD_zoom1x(LED2, H_String_LED2, S_String_LED2, V_String_LED2, 105);
			uint32_t newColorLED2 = RGB_LED2.R << 16 | RGB_LED2.G << 8 | RGB_LED2.B;
			*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB2_o) = newColorLED2; 
			redPos = r_knob;
			greenPos = g_knob;
			bluePos = b_knob;
			strToFrame("                                 ", 300, 140, 0x0, 0x0, zoomTextLCD);
			strToFrame("Press BLUE to apply changes...", 300, 140, 0xFFFF, 0x0, zoomTextLCD);
			if (b_button == 1) {
				strToFrame("                                ", 300, 100, 0xFFFF, 0x0, zoomTextLCD);
				break;
			}
		}
	}
}

void setStatic_zoom1x_LED1() 
{
	rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
	int redPos = (rgb_knobs_value >> 16) & 0xFF;  // new red knob position
	int greenPos = (rgb_knobs_value >> 8) & 0xFF; // new red knob position
	int bluePos = (rgb_knobs_value)&0xFF;		  // new red knob position

	while (1) {

		rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
		r_knob = (rgb_knobs_value >> 16) & 0xFF; // new red knob position
		g_knob = (rgb_knobs_value >> 8) & 0xFF;  // new red knob position
		b_knob = (rgb_knobs_value) & 0xFF;		 // new red knob position
		b_button = (rgb_knobs_value >> 24) & 1;  // blue button
		r_button = (rgb_knobs_value >> 26) & 1;  // red button

		if (r_button == 0) {
			changeHSV(&LED1, r_knob, g_knob, b_knob, redPos, greenPos, bluePos);
			RGB_LED1 = HSVToRGB(LED1);
			showColor1_Screen(RGB_LED1);
			showValuesLCD_zoom1x(LED1, H_String_LED1, S_String_LED1, V_String_LED1, 60);
			uint32_t newColorLED1 = RGB_LED1.R << 16 | RGB_LED1.G << 8 | RGB_LED1.B;
			*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o) = newColorLED1;
			redPos = r_knob;
			greenPos = g_knob;
			bluePos = b_knob;
			strToFrame("                                 ", 300, 140, 0x0, 0x0, zoomTextLCD);
			strToFrame("Press BLUE to apply changes...", 300, 140, 0xFFFF, 0x0, zoomTextLCD);
			if (b_button == 1) {
				strToFrame("                               ", 300, 100, 0xFFFF, 0x0, zoomTextLCD);
				break;
			}

		}
	}
}