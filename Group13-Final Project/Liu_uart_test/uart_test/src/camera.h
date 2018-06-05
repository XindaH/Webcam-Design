/*
 * camera.h
 *
 * Created: 2/27/2018 7:54:01 PM
 *  Author: ece-lab3
 */ 


#ifndef CAMERA_H_
#define CAMERA_H_

#include <asf.h>

#define CAM_RESET_PIN			PIO_PA20_IDX

// Image sensor VSYNC pin.
#define OV2640_VSYNC_PIO	       PIOA
#define OV2640_VSYNC_ID		       ID_PIOA
#define OV2640_VSYNC_MASK          PIO_PA15
#define OV2640_VSYNC_TYPE          PIO_PULLUP

/** OV_HSYNC pin definition */
#define OV_HSYNC_GPIO                  PIO_PA16_IDX
#define OV_HSYNC_FLAGS                 (PIO_PULLUP | PIO_IT_RISE_EDGE)
#define OV_HSYNC_MASK                  PIO_PA16
#define OV_HSYNC_PIO                   PIOA
#define OV_HSYNC_ID                    ID_PIOA
#define OV_HSYNC_TYPE                  PIO_PULLUP

/** OV_VSYNC pin definition */
#define OV_VSYNC_GPIO                  PIO_PA15_IDX
#define OV_VSYNC_FLAGS                 (PIO_PULLUP | PIO_IT_RISE_EDGE)
#define OV_VSYNC_MASK                  PIO_PA15
#define OV_VSYNC_PIO                   PIOA
#define OV_VSYNC_ID                    ID_PIOA
#define OV_VSYNC_TYPE                  PIO_PULLUP


/** OV Data Bus pins */
#define OV_DATA_BUS_D2                 PIO_PA24_IDX
#define OV_DATA_BUS_D3                 PIO_PA25_IDX
#define OV_DATA_BUS_D4                 PIO_PA26_IDX
#define OV_DATA_BUS_D5                 PIO_PA27_IDX
#define OV_DATA_BUS_D6                 PIO_PA28_IDX
#define OV_DATA_BUS_D7                 PIO_PA29_IDX
#define OV_DATA_BUS_D8                 PIO_PA30_IDX
#define OV_DATA_BUS_D9                 PIO_PA31_IDX
#define OV_DATA_BUS_FLAGS              (PIO_INPUT | PIO_PULLUP)
#define OV_DATA_BUS_MASK               (0xFF000000UL)
#define OV_DATA_BUS_PIO                PIOA
#define OV_DATA_BUS_ID                 ID_PIOA
#define OV_DATA_BUS_TYPE               PIO_INPUT
#define OV_DATA_BUS_ATTR               PIO_DEFAULT


#define PINS_TWI0                      PIN_TWI_TWD0, PIN_TWI_TWCK0
#define ID_BOARD_TWI		               ID_TWI0
#define BOARD_TWI			                 TWI0
#define BOARD_TWI_IRQn		             TWI0_IRQn
#define TWI_CLK     (400000UL)

/** TWI0 data pin */
#define PIN_TWI_TWD0                   {PIO_PA3A_TWD0, PIOA, ID_PIOA, \
PIO_PERIPH_A, PIO_DEFAULT}
/** TWI0 clock pin */
#define PIN_TWI_TWCK0                  {PIO_PA4A_TWCK0, PIOA, ID_PIOA,	\
PIO_PERIPH_A, PIO_DEFAULT}


#define TWI0_DATA_GPIO                 PIO_PA3_IDX
#define TWI0_DATA_FLAGS                (PIO_PERIPH_A |PIO_DEFAULT)
#define TWI0_DATA_MASK                 PIO_PA3
#define TWI0_DATA_PIO	               PIOA
#define TWI0_DATA_ID		           ID_PIOA
#define TWI0_DATA_TYPE                 PIO_DEFAULT

#define TWI0_CLK_GPIO                  PIO_PA4_IDX
#define TWI0_CLK_FLAGS                 (PIO_PERIPH_A |PIO_DEFAULT)
#define TWI0_CLK_MASK                  PIO_PA4
#define TWI0_CLK_PIO	               PIOA
#define TWI0_CLK_ID		               ID_PIOA
#define TWI0_CLK_TYPE                  PIO_DEFAULT

#define PIN_PCK1                       (PIO_PA17_IDX)
#define PIN_PCK1_FLAGS                 (PIO_PERIPH_B |PIO_DEFAULT)

#define PIN_XCLK0                      (PIO_PA17_IDX)
#define PIN_XCLK0_FLAGS                (PIO_PERIPH_B |PIO_DEFAULT)

#define SRAM_BASE                      (0x60000000UL) 
#define CAP_DEST                       SRAM_BASE
#define IMAGE_HEIGHT                   (240UL)
#define IMAGE_WIDTH                    (320UL)
#define ARRAY_SIZE						100000

volatile uint32_t start_image;
volatile uint32_t end_image;
volatile uint32_t image_length;

volatile uint8_t image_buffer[ARRAY_SIZE];


void configure_twi(void);
uint8_t find_image_len(void);
void init_camera(void);
void configure_camera(void);
uint8_t start_capture(void);

#endif /* CAMERA_H_ */