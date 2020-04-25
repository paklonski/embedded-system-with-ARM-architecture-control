#ifndef SET_STATIC_ZOOM1X
#define SET_STATIC_ZOOM1X

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

void setStatic_zoom1x_COPY(int from);
void setStatic_zoom1x_TOGETHER();
void setStatic_zoom1x_LED2();
void setStatic_zoom1x_LED1();

#endif