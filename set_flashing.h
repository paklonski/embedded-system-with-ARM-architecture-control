#ifndef SET_FLASHING
#define SET_FLASHING

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

void *manage_flashing (void *args);
void *manage_flashing_shifted_LED2(void *args);
void *manage_flashing_shifted(void *args);

#endif