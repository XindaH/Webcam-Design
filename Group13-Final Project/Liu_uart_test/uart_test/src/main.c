/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

/////////////////////lIU CAO
#include <asf.h>
#include "wifi.h"
#include "conf_board.h"
#include "conf_clock.h"
#include "timer_interface.h"
#include "camera.h"

int main (void)
{

	sysclk_init();
	wdt_disable(WDT);
	board_init();
	configure_tc();
	
	configure_usart_wifi();
	configure_wifi_comm_pin();
	configure_wifi_web_setup_pin();
	
	ioport_set_pin_level(WIFI_RESET, false);
	delay_ms(50);
	ioport_set_pin_level(WIFI_RESET, true);
	
	ioport_set_pin_level(CAM_RESET_PIN, true);
	init_camera();
	
	while (!ioport_get_pin_level(NETWORK_STATUS_PIN))
	{
		if (wifi_web_setup_flag)
		{
			write_wifi_command("setup web\r\n", 2);
			wifi_web_setup_flag =  false;
		}
	}
	
	write_wifi_command("set system.cmd.promt_enabled 0\r\n", 2);
	write_wifi_command("set system.cmd.echo off\r\n", 2);
	
	while(1){
		if (wifi_web_setup_flag)
		{
			write_wifi_command("setup web\r\n", 2);
			wifi_web_setup_flag =  false;
			continue;
		}
		if (!ioport_get_pin_level(NETWORK_STATUS_PIN))
		{
			continue;
		}
		if (!start_transfer)
		{
			write_wifi_command("poll all\r\n", 2);
			delay_ms(1000);
		}
		else if (start_transfer){
			start_capture();
			write_image_to_file();
		}
	}
}





