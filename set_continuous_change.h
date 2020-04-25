#ifndef SET_CONTINUOUS_CHANGE
#define SET_CONTINUOUS_CHANGE

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

void *playLED(void *args);
void *play_LED_in_antiphase(void *args);

#endif