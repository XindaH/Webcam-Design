/*
 * wifi.c
 *
 * Created: 2/24/2018 4:19:08 PM
 *  Author: ece-lab3
 */ 
#include "wifi.h"
#include "camera.h"
#include "timer_interface.h"



volatile uint32_t received_byte_wifi = 0;
volatile uint32_t new_rx_wifi = false;
volatile uint32_t input_pos_wifi = 0;
volatile uint32_t wifi_web_setup_flag = false;
volatile uint32_t start_transfer = false;
volatile uint32_t start_transfer_flag = false;

void USART_Handler(void)
{
	uint32_t ul_status;

	/* Read USART status. */
	ul_status = usart_get_status(BOARD_USART);

	/* Receive buffer is full. */
	if (ul_status & US_CSR_RXBUFF) {
		usart_read(BOARD_USART, &received_byte_wifi);
		new_rx_wifi = true;
		process_incoming_byte_wifi((uint8_t)received_byte_wifi);	
	}
}

void configure_usart_wifi(void)
{

	gpio_configure_pin(PIN_USART0_RXD_IDX, PIN_USART0_RXD_FLAGS);
	gpio_configure_pin(PIN_USART0_TXD_IDX, PIN_USART0_TXD_FLAGS);
	gpio_configure_pin(PIN_USART0_CTS_IDX, PIN_USART0_CTS_FLAGS);
	gpio_configure_pin(PIN_USART0_RTS_IDX, PIN_USART0_RTS_FLAGS);

	static uint32_t ul_sysclk;
	const sam_usart_opt_t usart_console_settings = {
		BOARD_USART_BAUDRATE,
		US_MR_CHRL_8_BIT,
		US_MR_PAR_NO,
		US_MR_NBSTOP_1_BIT,
		US_MR_CHMODE_NORMAL,
		/* This field is only used in IrDA mode. */
		0
	};
	
	/* Get peripheral clock. */
	ul_sysclk = sysclk_get_peripheral_hz();

	/* Enable peripheral clock. */
	sysclk_enable_peripheral_clock(BOARD_ID_USART);

	/* Configure USART. */
	usart_init_hw_handshaking(BOARD_USART, &usart_console_settings, ul_sysclk);

	/* Disable all the interrupts. */
	usart_disable_interrupt(BOARD_USART, ALL_INTERRUPT_MASK);
	
	/* Enable TX & RX function. */
	usart_enable_tx(BOARD_USART);
	usart_enable_rx(BOARD_USART);

usart_enable_interrupt(BOARD_USART, US_IER_RXRDY);

	/* Configure and enable interrupt of USART. */
	NVIC_EnableIRQ(USART_IRQn);
}

static void wifi_command_response_handler(uint32_t ul_id, uint32_t ul_mask)
{
	unused(ul_id);
	unused(ul_mask);
	
	process_data_wifi();
	for(uint32_t ii=0;ii<MAX_INPUT_WIFI;ii++) input_line_wifi[ii] = 0;
	input_pos_wifi = 0;
}


void configure_wifi_comm_pin(void)
{
	/* Configure PIO clock. */
	pmc_enable_periph_clk(WIFI_COMM_PIN_ID);

	/* Adjust PIO debounce filter using a 10 Hz filter. */
	pio_set_debounce_filter(WIFI_COMM_PIN_PIO, WIFI_COMM_PIN_MASK, 10);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(WIFI_COMM_PIN_PIO, WIFI_COMM_PIN_ID, WIFI_COMM_PIN_MASK,
			WIFI_COMM_PIN_ATTR, wifi_command_response_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)WIFI_COMM_PIN_ID);

	/* Enable PIO interrupt lines. */
	pio_enable_interrupt(WIFI_COMM_PIN_PIO, WIFI_COMM_PIN_MASK);
}

void configure_wifi_web_setup_pin(void)
{
	/* Configure PIO clock. */
	pmc_enable_periph_clk(wifi_setup_pin_ID);

	/* Adjust PIO debounce filter using a 10 Hz filter. */
	pio_set_debounce_filter(wifi_setup_pin_PIO, wifi_setup_pin_MSK, 10);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(wifi_setup_pin_PIO, wifi_setup_pin_ID, wifi_setup_pin_MSK,
			wifi_setup_pin_ATTR, wifi_web_setup_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)wifi_setup_pin_ID);

	/* Enable PIO interrupt lines. */
	pio_enable_interrupt(wifi_setup_pin_PIO, wifi_setup_pin_MSK);
}

void process_incoming_byte_wifi(uint8_t in_byte)
{
	input_line_wifi[input_pos_wifi++] = in_byte;
	
}

void process_data_wifi(void)
{
	if(strstr(input_line_wifi, "0,0")){
		start_transfer = true;
	}
	else if (strstr(input_line_wifi, "Start transfer"))
	{
		start_transfer_flag = true;
	}
	else if (strstr(input_line_wifi, "Websocket disconnected"))
	{
		start_transfer = false;
	}
}

void wifi_web_setup_handler(uint32_t ul_id, uint32_t ul_mask)
{
	unused(ul_id);
	unused(ul_mask);

	wifi_web_setup_flag = true;
}

void write_wifi_command(char* comm, uint8_t cnt)
{
	counts = 0;
	usart_write_line(BOARD_USART, comm);
	while (counts < cnt && !new_rx_wifi)
	{}
}

void write_image_to_file(void)
{
	if(image_length != 0){
		char buffer[20];
		sprintf(buffer, "image_transfer %d\r\n", image_length);
		write_wifi_command(buffer, 2);
		
		while (!start_transfer_flag && start_transfer){
			
		}
		if (start_transfer)
		{
			for (int i = start_image; i < start_image + image_length; i++)
			{
				usart_putchar(BOARD_USART, image_buffer[i]);
			}
		}
		delay_ms(50);
		start_transfer_flag = false;
	}
}