/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>
#include "camera.h"
#include "wifi.h"

void board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
	
	ioport_init();
	
	ioport_set_pin_dir(NETWORK_STATUS_PIN, IOPORT_DIR_INPUT);
	
	ioport_set_pin_dir(CTS_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(CTS_PIN, false);
	
	ioport_set_pin_dir(WIFI_RESET, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(WIFI_RESET, false);
	
	ioport_set_pin_dir(CAM_RESET_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(CAM_RESET_PIN, true);
}


