#ifndef SET_STATIC_ZOOM2X
#define SET_STATIC_ZOOM2X

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

void setStatic_zoom2x_COPY(int from);
void setStatic_zoom2x_TOGETHER();
void setStatic_zoom2x_LED2();
void setStatic_zoom2x_LED1();

#endif