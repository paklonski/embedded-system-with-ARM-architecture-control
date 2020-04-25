#include "set_time.h"

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
#include "lcdframe.h"

void setTime1x(int mode)
{
	rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
	int redPos = (rgb_knobs_value >> 16) & 0xFF;  // new red knob position

	while (1)
	{
		rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
		r_knob = (rgb_knobs_value >> 16) & 0xFF; // new red knob position
		b_button = (rgb_knobs_value >> 24) & 1;  // blue button
		r_button = (rgb_knobs_value >> 26) & 1;  // red button

		if (r_button == 0) {

			switch (mode) {
				case 1:
					if (r_knob > redPos || (redPos == 255 && r_knob == 0)) {
						timeChange += 1;
					} else if ((r_knob < redPos || (redPos == 0 && r_knob == 255)) && timeChange > 0) {
						timeChange -= 1;
					}
					redPos = r_knob;
					sprintf(timeString, "%d", timeChange);
					strToFrame("       ", 150, 320, 0x0, 0x0, zoomTextLCD);
					strToFrame(timeString, 150, 320, 0xFFFF, 0x0, zoomTextLCD);
					strToFrame("s", 150, 355, 0xFFFF, 0x0, zoomTextLCD);
					strToFrame("                                                         ", 300, 50, 0x0, 0x0, zoomTextLCD);
					strToFrame("Press BLUE to apply changes...", 300, 140, 0xFFFF, 0x0, zoomTextLCD);
					frameToLCD();
					break;
				case 2:
					if (r_knob > redPos || (redPos == 255 && r_knob == 0)) {
						timeOn += 1;
					} else if ((r_knob < redPos || (redPos == 0 && r_knob == 255)) && timeOn > 0) {
						timeOn -= 1;
					}
					redPos = r_knob;
					sprintf(timeOnString, "%d", timeOn);
					strToFrame("       ", 60, 320, 0x0, 0x0, zoomTextLCD);
					strToFrame(timeOnString, 60, 320, 0xFFFF, 0x0, zoomTextLCD);
					strToFrame("                                                         ", 300, 50, 0x0, 0x0, zoomTextLCD);
					strToFrame("Press BLUE to apply changes...", 300, 140, 0xFFFF, 0x0, zoomTextLCD);
					frameToLCD();
					break;
				case 3:
					if (r_knob > redPos || (redPos == 255 && r_knob == 0)) {
						timeOff += 1;
					} else if ((r_knob < redPos || (redPos == 0 && r_knob == 255)) && timeOff > 0) {
						timeOff -= 1;
					}
					redPos = r_knob;
					sprintf(timeOffString, "%d", timeOff);
					strToFrame("       ", 105, 320, 0x0, 0x0, zoomTextLCD);
					strToFrame(timeOffString, 105, 320, 0xFFFF, 0x0, zoomTextLCD);
					strToFrame("                                                         ", 300, 50, 0x0, 0x0, zoomTextLCD);
					strToFrame("Press BLUE to apply changes...", 300, 140, 0xFFFF, 0x0, zoomTextLCD);
					frameToLCD();
					break;
				case 4:
					if (r_knob > redPos || (redPos == 255 && r_knob == 0)) {
						timeShift += 1;
					} else if ((r_knob < redPos || (redPos == 0 && r_knob == 255)) && timeShift > 0) {
						timeShift -= 1;
					}
					redPos = r_knob;
					sprintf(timeShiftString, "%d", timeShift);
					strToFrame("       ", 150, 320, 0x0, 0x0, zoomTextLCD);
					strToFrame(timeShiftString, 150, 320, 0xFFFF, 0x0, zoomTextLCD);
					strToFrame("                                                         ", 300, 50, 0x0, 0x0, zoomTextLCD);
					strToFrame("Press BLUE to apply changes...", 300, 140, 0xFFFF, 0x0, zoomTextLCD);
					frameToLCD();
					break;
				default:
					break;
			}

			if (b_button == 1) {
				strToFrame("                                                         ", 300, 50, 0x0, 0x0, zoomTextLCD);
				frameToLCD();
				break;
			}
		}
	}
}

void setTime2x(int mode)
{
	rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
	int redPos = (rgb_knobs_value >> 16) & 0xFF;  // new red knob position

	while (1)
	{
		rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
		r_knob = (rgb_knobs_value >> 16) & 0xFF; // new red knob position
		b_button = (rgb_knobs_value >> 24) & 1;  // blue button
		r_button = (rgb_knobs_value >> 26) & 1;  // red button

		if (r_button == 0) {

			switch (mode) {
				case 1:
					strToFrame("apply", 285, 380, 0x001F, 0x0, zoomTextLCD);
					if (r_knob > redPos || (redPos == 255 && r_knob == 0)) {
						timeChange += 1;
					} else if ((r_knob < redPos || (redPos == 0 && r_knob == 255)) && timeChange > 0) {
						timeChange -= 1;
					}
					redPos = r_knob;
					sprintf(timeString, "%d", timeChange);
					strToFrame("         ", 150, 300, 0x0, 0x0, zoomTextLCD);
					strToFrame(timeString, 150, 300, 0xFFFF, 0x0, zoomTextLCD);
					frameToLCD();
					break;
				case 2:
					strToFrame("apply", 285, 380, 0x001F, 0x0, zoomTextLCD);
					if (r_knob > redPos || (redPos == 255 && r_knob == 0)) {
						timeOn += 1;
					} else if ((r_knob < redPos || (redPos == 0 && r_knob == 255)) && timeOn > 0) {
						timeOn -= 1;
					}
					redPos = r_knob;
					sprintf(timeOnString, "%d", timeOn);
					strToFrame("         ", 60, 320, 0x0, 0x0, zoomTextLCD);
					strToFrame(timeOnString, 60, 320, 0xFFFF, 0x0, zoomTextLCD);
					frameToLCD();
					break;
				case 3:
					strToFrame("apply", 285, 380, 0x001F, 0x0, zoomTextLCD);
					if (r_knob > redPos || (redPos == 255 && r_knob == 0)) {
						timeOff += 1;
					} else if ((r_knob < redPos || (redPos == 0 && r_knob == 255)) && timeOff > 0) {
						timeOff -= 1;
					}
					redPos = r_knob;
					sprintf(timeOffString, "%d", timeOff);
					strToFrame("       ", 105, 320, 0x0, 0x0, zoomTextLCD);
					strToFrame(timeOffString, 105, 320, 0xFFFF, 0x0, zoomTextLCD);
					frameToLCD();
					break;
				case 4:
					strToFrame("apply", 285, 380, 0x001F, 0x0, zoomTextLCD);
					if (r_knob > redPos || (redPos == 255 && r_knob == 0)) {
						timeShift += 1;
					} else if ((r_knob < redPos || (redPos == 0 && r_knob == 255)) && timeShift > 0) {
						timeShift -= 1;
					}
					redPos = r_knob;
					sprintf(timeShiftString, "%d", timeShift);
					strToFrame("         ", 150, 320, 0x0, 0x0, zoomTextLCD);
					strToFrame(timeShiftString, 150, 320, 0xFFFF, 0x0, zoomTextLCD);
					frameToLCD();
					break;
				default:
					break;
			}

			if (b_button == 1) {
				break;
			}
		}
	}
}