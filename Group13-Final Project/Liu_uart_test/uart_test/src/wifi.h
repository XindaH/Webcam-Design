/*
 * wifi.h
 *
 * Created: 2/24/2018 4:22:10 PM
 *  Author: ece-lab1111111111111111111111111111
 */ 

#ifndef WIFI_H_
#define WIFI_H_

#include <asf.h>
#include <string.h>

#define BOARD_ID_USART             ID_USART0

#define BOARD_USART                USART0

#define BOARD_USART_BAUDRATE       115200

#define USART_Handler              USART0_Handler

#define USART_IRQn                 USART0_IRQn

/** USART1 pin RX */
#define PIN_USART0_RXD    {PIO_PA5A_RXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART0_RXD_IDX        (PIO_PA5_IDX)
#define PIN_USART0_RXD_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/** USART1 pin TX */
#define PIN_USART0_TXD    {PIO_PA6A_TXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART0_TXD_IDX        (PIO_PA6_IDX)
#define PIN_USART0_TXD_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/** USART1 pin CTS */
#define PIN_USART0_CTS    {PIO_PA8A_CTS0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART0_CTS_IDX        (PIO_PA8_IDX)
#define PIN_USART0_CTS_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)
/** USART1 pin RTS */
#define PIN_USART0_RTS    {PIO_PA7A_RTS0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT}
#define PIN_USART0_RTS_IDX        (PIO_PA7_IDX)
#define PIN_USART0_RTS_FLAGS      (PIO_PERIPH_A | PIO_DEFAULT)

#define ALL_INTERRUPT_MASK  0xffffffff

#define MAX_INPUT_WIFI      1000

#define WIFI_COMM_PIN_MASK          PIO_PB10
#define WIFI_COMM_PIN_PIO           PIOB
#define WIFI_COMM_PIN_ID            ID_PIOB
#define WIFI_COMM_PIN_TYPE          PIO_INPUT
#define WIFI_COMM_PIN_ATTR          PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE

volatile uint32_t input_pos_wifi;
volatile uint32_t received_byte_wifi;
volatile char input_line_wifi[MAX_INPUT_WIFI];
volatile uint32_t new_rx_wifi;
volatile uint32_t wifi_web_setup_flag;
volatile uint32_t wifi_status_pin_trigger;
volatile uint32_t start_transfer;
volatile uint32_t start_transfer_flag;

void configure_wifi(void);
void process_incoming_byte_wifi(uint8_t in_byte);
void configure_wifi_setup_pin(void);
void configure_usart_wifi(void);
void configure_wifi_comm_pin(void);
void write_wifi_command(char* comm, uint8_t cnt);
void write_image_to_file(void);
void process_data_wifi(void);
void wifi_web_setup_handler(uint32_t ul_id, uint32_t ul_mask);
void configure_wifi_web_setup_pin(void);

/* Push button board defines. */
#define wifi_setup_pin_PIO           PIOA
#define wifi_setup_pin_ID            ID_PIOA
#define wifi_setup_pin_MSK           PIO_PA22
#define wifi_setup_pin_ATTR          PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE

#define WIFI_RESET PIO_PB0_IDX
#define NETWORK_STATUS_PIN PIO_PB11_IDX
#define CTS_PIN PIO_PA7_IDX

#endif /* WIFI_H_ */