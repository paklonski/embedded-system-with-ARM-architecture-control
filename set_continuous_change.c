#include "set_continuous_change.h"

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
#include "utils.h"

void *playLED(void *args) { 
	 
	struct HSV srcHSV;
	struct HSV dstHSV;
	struct HSV currentHSV;
	struct RGB currentRGB;

	if (LED1_CONTINUOUS == 1 || LEDs_CONTINUOUS == 1) {
		memcpy(&currentHSV, &LED1, sizeof(LED1));
		memcpy(&srcHSV, &LED1, sizeof(LED1));
		memcpy(&dstHSV, &LED2, sizeof(LED2));
	}
	else {
		memcpy(&currentHSV, &LED2, sizeof(LED2));
		memcpy(&srcHSV, &LED2, sizeof(LED2));
		memcpy(&dstHSV, &LED1, sizeof(LED1));
	}

	double distance_Hue = abs(LED1.H - LED2.H);
	double distance_Saturation = abs(LED1.S - LED2.S);
	double distance_Value = abs(LED1.V - LED2.V);

	int moveToDest_Hue = 1;
	int moveToDest_Saturation = 1;
	int moveToDest_Value = 1;

	double timeout_for_Hue = (double)timeChange / distance_Hue * 1000000L;		// ms
	double timeout_for_Saturation = (double)timeChange / distance_Saturation * 1000000L;
	double timeout_for_Value = (double)timeChange / distance_Value * 1000000L;

    struct timespec start_time_Hue, current_time_Hue;
    struct timespec start_time_Saturation, current_time_Saturation;
    struct timespec start_time_Value, current_time_Value;

    clock_gettime(CLOCK_REALTIME, &start_time_Hue);
    clock_gettime(CLOCK_REALTIME, &start_time_Saturation);
    clock_gettime(CLOCK_REALTIME, &start_time_Value);

    long int elapsed_time_Hue, elapsed_time_Saturation, elapsed_time_Value;

	while (1) {

	 	rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
	 	b_button = (rgb_knobs_value >> 24) & 1;

	 	if (CONTINUOUS_MODE == 0) {
	 		uint32_t newColorLED1 = RGB_LED1.R << 16 | RGB_LED1.G << 8 | RGB_LED1.B;
	 		uint32_t newColorLED2 = RGB_LED2.R << 16 | RGB_LED2.G << 8 | RGB_LED2.B;
	 		*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o) = newColorLED1;
	 		*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB2_o) = newColorLED2;
	 		break;
	 	}

	 	moveToDest_Hue = (distance_Hue < 0.1) ? 0 : ((distance_Hue > abs(LED1.H - LED2.H) - 0.1) ? 1 : moveToDest_Hue);
	 	moveToDest_Saturation = (distance_Saturation < 0.99) ? 0 : ((distance_Saturation > abs(LED1.S - LED2.S) - 0.99) ? 1 : moveToDest_Saturation);
	 	moveToDest_Value = (distance_Value < 0.99) ? 0 : ((distance_Value > abs(LED1.V - LED2.V) - 0.99) ? 1 : moveToDest_Value);

        clock_gettime(CLOCK_REALTIME, &current_time_Hue);
        clock_gettime(CLOCK_REALTIME, &current_time_Saturation);
        clock_gettime(CLOCK_REALTIME, &current_time_Value);

        elapsed_time_Hue = (long int)((current_time_Hue.tv_sec - start_time_Hue.tv_sec) * 1000000L + (current_time_Hue.tv_nsec - start_time_Hue.tv_nsec) / 1000);
        elapsed_time_Saturation = (long int)((current_time_Saturation.tv_sec - start_time_Saturation.tv_sec) * 1000000L 
                                                                                    + (current_time_Saturation.tv_nsec - start_time_Saturation.tv_nsec) / 1000);
        elapsed_time_Value = (long int)((current_time_Value.tv_sec - start_time_Value.tv_sec) * 1000000L + (current_time_Value.tv_nsec - start_time_Value.tv_nsec) / 1000);

	 	if (elapsed_time_Hue >= timeout_for_Hue) {
	 		if (moveToDest_Hue == 1) {
	 			currentHSV.H = (srcHSV.H > dstHSV.H) ? currentHSV.H - 1.0 : currentHSV.H + 1.0;
	 			distance_Hue--;
	 		} else {
	 			currentHSV.H = (srcHSV.H > dstHSV.H) ? currentHSV.H + 1.0 : currentHSV.H - 1.0;
	 			distance_Hue++;
	 		}
            clock_gettime(CLOCK_REALTIME, &start_time_Hue);
	 	}

	 	if (elapsed_time_Saturation >= timeout_for_Saturation) {
	 		if (moveToDest_Saturation == 1) {
	 			currentHSV.S = (srcHSV.S > dstHSV.S) ? currentHSV.S - 1.0 : currentHSV.S + 1.0;
	 			distance_Saturation--;
	 		} else {
	 			currentHSV.S = (srcHSV.S > dstHSV.S) ? currentHSV.S + 1.0 : currentHSV.S - 1.0;
	 			distance_Saturation++;
	 		}
            clock_gettime(CLOCK_REALTIME, &start_time_Saturation);
	 	}

	 	if (elapsed_time_Value > timeout_for_Value) {
	 		if (moveToDest_Value == 1) {
	 			currentHSV.V = (srcHSV.V > dstHSV.V) ? currentHSV.V - 1.0 : currentHSV.V + 1.0;
	 			distance_Value--;
	 		} else {
	 			currentHSV.V = (srcHSV.V > dstHSV.V) ? currentHSV.V + 1.0 : currentHSV.V - 1.0;
	 			distance_Value++;
	 		}
            clock_gettime(CLOCK_REALTIME, &start_time_Value);
	 	}

        currentRGB = HSVToRGB(currentHSV);

		if (LED1_CONTINUOUS == 1) {
			if (LED1_ON == 1) {
				uint32_t newColorLED = currentRGB.R << 16 | currentRGB.G << 8 | currentRGB.B;
				*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o) = newColorLED;
			} else {
				*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o) = 0x0;
			}
		} else if (LED2_CONTINUOUS == 1) {
			if (LED2_ON == 1) {
				uint32_t newColorLED = currentRGB.R << 16 | currentRGB.G << 8 | currentRGB.B;
				*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB2_o) = newColorLED;
			} else {
				*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB2_o) = 0x0;
			}
		} else if (LEDs_CONTINUOUS == 1){
			if (LED1_ON == 1) {
				uint32_t newColorLED = currentRGB.R << 16 | currentRGB.G << 8 | currentRGB.B;
				*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o) = newColorLED;
			} else {
				*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o) = 0x0;
			}
			if (LED2_ON == 1) {
				uint32_t newColorLED = currentRGB.R << 16 | currentRGB.G << 8 | currentRGB.B;
				*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB2_o) = newColorLED;
			} else {
				*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB2_o) = 0x0;
			}
		}
	}
	pthread_exit(0);
}


void *play_LED_in_antiphase(void *args) {

    struct HSV srcHSV_LED1;
	struct HSV dstHSV_LED1;
	struct HSV currentHSV_LED1;
	struct RGB currentRGB_LED1;

    struct HSV srcHSV_LED2;
	struct HSV dstHSV_LED2;
	struct HSV currentHSV_LED2;
	struct RGB currentRGB_LED2;

    memcpy(&currentHSV_LED1, &LED1, sizeof(LED1));
    memcpy(&srcHSV_LED1, &LED1, sizeof(LED1));
    memcpy(&dstHSV_LED1, &LED2, sizeof(LED2));

    memcpy(&currentHSV_LED2, &LED2, sizeof(LED2));
    memcpy(&srcHSV_LED2, &LED2, sizeof(LED2));
    memcpy(&dstHSV_LED2, &LED1, sizeof(LED1));

    double distance_Hue = abs(LED1.H - LED2.H);
	double distance_Saturation = abs(LED1.S - LED2.S);
	double distance_Value = abs(LED1.V - LED2.V);

    int moveToDest_Hue = 1;
	int moveToDest_Saturation = 1;
	int moveToDest_Value = 1;

	double timeout_for_Hue = (double)timeChange / distance_Hue * 1000000L;
	double timeout_for_Saturation = (double)timeChange / distance_Saturation * 1000000L;
	double timeout_for_Value = (double)timeChange / distance_Value * 1000000L;

    struct timespec start_time_Hue, current_time_Hue;
    struct timespec start_time_Saturation, current_time_Saturation;
    struct timespec start_time_Value, current_time_Value;

    clock_gettime(CLOCK_REALTIME, &start_time_Hue);
    clock_gettime(CLOCK_REALTIME, &start_time_Saturation);
    clock_gettime(CLOCK_REALTIME, &start_time_Value);

    long int elapsed_time_Hue, elapsed_time_Saturation, elapsed_time_Value;

    while (1) {

	 	rgb_knobs_value = *(volatile uint32_t *)(mem_base + SPILED_REG_KNOBS_8BIT_o);
	 	b_button = (rgb_knobs_value >> 24) & 1;

	 	if (CONTINUOUS_MODE == 0) {
	 		uint32_t newColorLED1 = RGB_LED1.R << 16 | RGB_LED1.G << 8 | RGB_LED1.B;
	 		uint32_t newColorLED2 = RGB_LED2.R << 16 | RGB_LED2.G << 8 | RGB_LED2.B;
	 		*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o) = newColorLED1;
	 		*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB2_o) = newColorLED2;
	 		break;
	 	}

	 	moveToDest_Hue = (distance_Hue < 0.1) ? 0 : ((distance_Hue > abs(LED1.H - LED2.H) - 0.1) ? 1 : moveToDest_Hue);
	 	moveToDest_Saturation = (distance_Saturation < 0.99) ? 0 : ((distance_Saturation > abs(LED1.S - LED2.S) - 0.99) ? 1 : moveToDest_Saturation);
	 	moveToDest_Value = (distance_Value < 0.99) ? 0 : ((distance_Value > abs(LED1.V - LED2.V) - 0.99) ? 1 : moveToDest_Value);

        clock_gettime(CLOCK_REALTIME, &current_time_Hue);
        clock_gettime(CLOCK_REALTIME, &current_time_Saturation);
        clock_gettime(CLOCK_REALTIME, &current_time_Value);

        elapsed_time_Hue = (long int)((current_time_Hue.tv_sec - start_time_Hue.tv_sec) * 1000000L + (current_time_Hue.tv_nsec - start_time_Hue.tv_nsec) / 1000);
        elapsed_time_Saturation = (long int)((current_time_Saturation.tv_sec - start_time_Saturation.tv_sec) * 1000000L 
                                                                                    + (current_time_Saturation.tv_nsec - start_time_Saturation.tv_nsec) / 1000);
        elapsed_time_Value = (long int)((current_time_Value.tv_sec - start_time_Value.tv_sec) * 1000000L + (current_time_Value.tv_nsec - start_time_Value.tv_nsec) / 1000);

	 	if (elapsed_time_Hue >= timeout_for_Hue) {
	 		if (moveToDest_Hue == 1) {
	 			currentHSV_LED1.H = (srcHSV_LED1.H > dstHSV_LED1.H) ? currentHSV_LED1.H - 1.0 : currentHSV_LED1.H + 1.0;
                currentHSV_LED2.H = (srcHSV_LED2.H > dstHSV_LED2.H) ? currentHSV_LED2.H - 1.0 : currentHSV_LED2.H + 1.0;
	 			distance_Hue--;
	 		} else {
	 			currentHSV_LED1.H = (srcHSV_LED1.H > dstHSV_LED1.H) ? currentHSV_LED1.H + 1.0 : currentHSV_LED1.H - 1.0;
                currentHSV_LED2.H = (srcHSV_LED2.H > dstHSV_LED2.H) ? currentHSV_LED2.H + 1.0 : currentHSV_LED2.H - 1.0;
	 			distance_Hue++;
	 		}
            clock_gettime(CLOCK_REALTIME, &start_time_Hue);
	 	}

	 	if (elapsed_time_Saturation >= timeout_for_Saturation) {
	 		if (moveToDest_Saturation == 1) {
	 			currentHSV_LED1.S = (srcHSV_LED1.S > dstHSV_LED1.S) ? currentHSV_LED1.S - 1.0 : currentHSV_LED1.S + 1.0;
                currentHSV_LED2.S = (srcHSV_LED2.S > dstHSV_LED2.S) ? currentHSV_LED2.S - 1.0 : currentHSV_LED2.S + 1.0;
	 			distance_Saturation--;
	 		} else {
	 			currentHSV_LED1.S = (srcHSV_LED1.S > dstHSV_LED1.S) ? currentHSV_LED1.S + 1.0 : currentHSV_LED1.S - 1.0;
                currentHSV_LED2.S = (srcHSV_LED2.S > dstHSV_LED2.S) ? currentHSV_LED2.S + 1.0 : currentHSV_LED2.S - 1.0;
	 			distance_Saturation++;
	 		}
            clock_gettime(CLOCK_REALTIME, &start_time_Saturation);
	 	}

	 	if (elapsed_time_Value > timeout_for_Value) {
	 		if (moveToDest_Value == 1) {
	 			currentHSV_LED1.V = (srcHSV_LED1.V > dstHSV_LED1.V) ? currentHSV_LED1.V - 1.0 : currentHSV_LED1.V + 1.0;
                currentHSV_LED2.V = (srcHSV_LED2.V > dstHSV_LED2.V) ? currentHSV_LED2.V - 1.0 : currentHSV_LED2.V + 1.0;
	 			distance_Value--;
	 		} else {
	 			currentHSV_LED1.V = (srcHSV_LED1.V > dstHSV_LED1.V) ? currentHSV_LED1.V + 1.0 : currentHSV_LED1.V - 1.0;
                currentHSV_LED2.V = (srcHSV_LED2.V > dstHSV_LED2.V) ? currentHSV_LED2.V + 1.0 : currentHSV_LED2.V - 1.0;
	 			distance_Value++;
	 		}
            clock_gettime(CLOCK_REALTIME, &start_time_Value);
	 	}

	  	currentRGB_LED1 = HSVToRGB(currentHSV_LED1);
        currentRGB_LED2 = HSVToRGB(currentHSV_LED2);

		if (LED1_ON == 1) {
			uint32_t newColor_LED1 = currentRGB_LED1.R << 16 | currentRGB_LED1.G << 8 | currentRGB_LED1.B;
			*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o) = newColor_LED1;
		} else {
			*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o) = 0x0;
		}

		if (LED2_ON == 1) {
			uint32_t newColor_LED2 = currentRGB_LED2.R << 16 | currentRGB_LED2.G << 8 | currentRGB_LED2.B;
			*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB2_o) = newColor_LED2;
		} else {
			*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB2_o) = 0x0;
		}
    }
    pthread_exit(0);
}