#ifndef MENU_CREATOR
#define MENU_CREATOR

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
#include "set_flashing.h"

int createFlashing_zoom2x();
int createFlashing_zoom1x();
int createContinuousChange_zoom1x();
int createContinuousChange_zoom2x();
int createStaticLight_zoom1x();
int createStaticLight_zoom2x();
int createMenu_zoom1x();
int createMenu_zoom2x();

#endif

