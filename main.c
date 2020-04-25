/*******************************************************************
  Simple program to check LCD functionality on MicroZed
  based MZ_APO board designed by Petr Porazil at PiKRON

  mzapo_lcdtest.c       - main and only file

  (C) Copyright 2004 - 2017 by Pavel Pisa
      e-mail:   pisa@cmp.felk.cvut.cz
      homepage: http://cmp.felk.cvut.cz/~pisa
      work:     http://www.pikron.com/
      license:  any combination of GPL, LGPL, MPL or BSD licenses

 *******************************************************************/

#define _POSIX_C_SOURCE 200112L

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
#include "menu_creator.h"

int main(int argc, char *argv[])
{
	printf("Hello world!\n");
	
	mem_base = map_phys_address(SPILED_REG_BASE_PHYS, SPILED_REG_SIZE, 0);
	parlcd_mem_base = map_phys_address(PARLCD_REG_BASE_PHYS, PARLCD_REG_SIZE, 0);

	if (mem_base == NULL)
		exit(1);
	if (parlcd_mem_base == NULL)
		exit(1);

	*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o) = 0x0;
	*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB2_o) = 0x0;

	setFirst();

	int menu = createMenu_zoom1x();

	while (1) {
		switch (menu) {
			case 0:
				LED1_ON = 0;
				LED2_ON = 0;
				LED1_CONTINUOUS = 0;
				LED2_CONTINUOUS = 0;
				LED1_FLASHING = 0;
				LED2_FLASHING = 0;
				FLASHING_MODE = 0;
				*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB1_o) = 0x0;
				*(volatile uint32_t *)(mem_base + SPILED_REG_LED_RGB2_o) = 0x0;
				printf("Goodbye world!\n");
				return 0;
			case 1:
				menu = createMenu_zoom1x();
				break;
			case 2:
				menu = createMenu_zoom2x();
				break;
			case 3:
				menu = createStaticLight_zoom1x();
				break;
			case 4:
			 	menu = createStaticLight_zoom2x();
				break;
			case 5:
			 	menu = createContinuousChange_zoom1x();
				break;
			case 6:
			 	menu = createContinuousChange_zoom2x();
				break;
			case 7:
			 	menu = createFlashing_zoom1x();
				break;
			case 8:
			 	menu = createFlashing_zoom2x();
				break;
			default:
				break;
		}
	}
	
	return 0;
}
