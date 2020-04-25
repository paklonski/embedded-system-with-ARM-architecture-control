#include "set_flashing.h"

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

void *manage_flashing(void *args) {

    int *NUMBER_OF_LED = (int *)args;

    long int time_ON = (long int)timeOn * 1000000L;
    long int time_OFF = (long int)timeOff * 1000000L;
    long int elapsed_time;
    struct timespec start_time, current_time;
    clock_gettime(CLOCK_REALTIME, &start_time);

    while (1) {

        if (FLASHING_MODE == 0) {
            if (STATIC_MODE == 1) {
                *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o) = RGB_LED1.R << 16 | RGB_LED1.G << 8 | RGB_LED1.B;
                *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB2_o) = RGB_LED2.R << 16 | RGB_LED2.G << 8 | RGB_LED2.B;
            }
            LED1_ON = 1;
            LED2_ON = 1;
            break;
        }

        clock_gettime(CLOCK_REALTIME, &current_time);
        elapsed_time = (long int)((current_time.tv_sec - start_time.tv_sec) * 1000000L + (current_time.tv_nsec - start_time.tv_nsec) / 1000);

        if (*NUMBER_OF_LED == 1) {

            if (STATIC_MODE == 1) {

                uint32_t colorLED1 = *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o);

                if (elapsed_time >= time_ON && colorLED1 != 0x0) {
                    *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o) = 0x0;
                    clock_gettime(CLOCK_REALTIME, &start_time);
                    elapsed_time = 0;
                }
                if (elapsed_time >= time_OFF && colorLED1 == 0x0) {
                    *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o) = RGB_LED1.R << 16 | RGB_LED1.G << 8 | RGB_LED1.B;
                    clock_gettime(CLOCK_REALTIME, &start_time);
                    elapsed_time = 0;
                }
            }

            if (CONTINUOUS_MODE == 1) {

                if (elapsed_time >= time_ON && LED1_ON == 1) {
                    LED1_ON = 0;
                    clock_gettime(CLOCK_REALTIME, &start_time);
                    elapsed_time = 0;
                }

                if (elapsed_time >= time_OFF && LED1_ON == 0) {
                    LED1_ON = 1;
                    clock_gettime(CLOCK_REALTIME, &start_time);
                    elapsed_time = 0;
                }
            }
        }

        if (*NUMBER_OF_LED == 2) {

            if (STATIC_MODE == 1) {

                uint32_t colorLED2 = *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB2_o);

                if (elapsed_time >= time_ON && colorLED2 != 0x0) {
                    *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB2_o) = 0x0;
                    clock_gettime(CLOCK_REALTIME, &start_time);
                    elapsed_time = 0;
                }
                if (elapsed_time >= time_OFF && colorLED2 == 0x0) {
                    *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB2_o) = RGB_LED2.R << 16 | RGB_LED2.G << 8 | RGB_LED2.B;
                    clock_gettime(CLOCK_REALTIME, &start_time);
                    elapsed_time = 0;
                }
            }

            if (CONTINUOUS_MODE == 1) {

                if (elapsed_time >= time_ON && LED2_ON == 1) {
                    LED2_ON = 0;
                    clock_gettime(CLOCK_REALTIME, &start_time);
                    elapsed_time = 0;
                }

                if (elapsed_time >= time_OFF && LED2_ON == 0) {
                    LED2_ON = 1;
                    clock_gettime(CLOCK_REALTIME, &start_time);
                    elapsed_time = 0;
                }

            }
        }

        if (*NUMBER_OF_LED == 3) {

            if (STATIC_MODE == 1) {

                uint32_t colorLED1 = *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o);
                uint32_t colorLED2 = *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB2_o);

                if (elapsed_time >= time_ON && (colorLED1 != 0x0 || colorLED2 != 0x0)) {
                    *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o) = 0x0;
                    *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB2_o) = 0x0;
                    clock_gettime(CLOCK_REALTIME, &start_time);
                    elapsed_time = 0;
                }

                if (elapsed_time >= time_OFF && colorLED1 == 0x0 && colorLED2 == 0x0) {
                    *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o) = RGB_LED1.R << 16 | RGB_LED1.G << 8 | RGB_LED1.B;
                    *(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB2_o) = RGB_LED2.R << 16 | RGB_LED2.G << 8 | RGB_LED2.B;
                    clock_gettime(CLOCK_REALTIME, &start_time);
                    elapsed_time = 0;
                }
            }

            if (CONTINUOUS_MODE == 1) {

                if (elapsed_time >= time_ON && LED1_ON == 1 && LED2_ON == 1) {
                    LED1_ON = 0;
                    LED2_ON = 0;
                    clock_gettime(CLOCK_REALTIME, &start_time);
                    elapsed_time = 0;
                }

                if (elapsed_time >= time_OFF && LED1_ON == 0 && LED2_ON == 0) {
                    LED1_ON = 1;
                    LED2_ON = 1;
                    clock_gettime(CLOCK_REALTIME, &start_time);
                    elapsed_time = 0;
                }
            }
        }
    }
    pthread_exit(0);
}