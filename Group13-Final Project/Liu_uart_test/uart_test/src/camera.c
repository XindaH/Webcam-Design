/*
 * camera.c
 *
 * Created: 2/27/2018 7:50:26 PM
 *  Author: ece-lab3
 */ 

#include "camera.h"
#include "ov2640.h"

static volatile uint32_t g_ul_vsync_flag = false;
uint8_t *g_p_uc_cap_dest_buf;

volatile uint32_t image_length;
volatile uint32_t start_image;
volatile uint32_t end_image;

static void vsync_handler(uint32_t ul_id, uint32_t ul_mask)
{
	unused(ul_id);
	unused(ul_mask);

	g_ul_vsync_flag = true;
}

static void init_vsync_interrupts(void)
{
	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(OV_VSYNC_PIO, OV_VSYNC_ID, OV_VSYNC_MASK,
			OV_VSYNC_TYPE, vsync_handler);

	/* Enable PIO controller IRQs */
	NVIC_EnableIRQ((IRQn_Type)OV_VSYNC_ID);
}

static void pio_capture_init(Pio *p_pio, uint32_t ul_id)
{
	/* Enable periphral clock */
	pmc_enable_periph_clk(ul_id);

	/* Disable pio capture */
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_PCEN);

	/* Disable rxbuff interrupt */
	p_pio->PIO_PCIDR |= PIO_PCIDR_RXBUFF;

	/* 32bit width*/
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_DSIZE_Msk);
	p_pio->PIO_PCMR |= PIO_PCMR_DSIZE_WORD;

	/* Only HSYNC and VSYNC enabled */
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_ALWYS);
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_HALFS);
}

static uint8_t pio_capture_to_buffer(Pio *p_pio, uint8_t *uc_buf,uint32_t ul_size)
{
	/* Check if the first PDC bank is free */
	if ((p_pio->PIO_RCR == 0) && (p_pio->PIO_RNCR == 0)) {
		p_pio->PIO_RPR = (uint32_t)uc_buf;
		p_pio->PIO_RCR = ul_size;
		p_pio->PIO_PTCR = PIO_PTCR_RXTEN;
		return 1;
		} else if (p_pio->PIO_RNCR == 0) {
		p_pio->PIO_RNPR = (uint32_t)uc_buf;
		p_pio->PIO_RNCR = ul_size;
		return 1;
		} else {
		return 0;
	}
}

void configure_twi(void)
{
		twi_options_t opt;
		gpio_configure_pin(TWI0_DATA_GPIO, TWI0_DATA_FLAGS);
		gpio_configure_pin(TWI0_CLK_GPIO, TWI0_CLK_FLAGS);
		/* Enable TWI peripheral */
		pmc_enable_periph_clk(ID_BOARD_TWI);

		/* Init TWI peripheral */
		opt.master_clk = sysclk_get_cpu_hz();
		opt.speed      = TWI_CLK;
		twi_master_init(BOARD_TWI, &opt);

		/* Configure TWI interrupts */
		NVIC_DisableIRQ(BOARD_TWI_IRQn);
		NVIC_ClearPendingIRQ(BOARD_TWI_IRQn);
		NVIC_SetPriority(BOARD_TWI_IRQn, 0);
		NVIC_EnableIRQ(BOARD_TWI_IRQn);
}


void init_camera(void)
{
	pmc_enable_pllbck(7,0x1,1);
	
	init_vsync_interrupts();
	
	gpio_configure_pin(PIN_PCK1, PIN_PCK1_FLAGS);
	
	/* Init PCK0 to work at 24 Mhz */
	/* 96/4=24 Mhz */
	PMC->PMC_PCK[1] = (PMC_PCK_PRES_CLK_4 | PMC_PCK_CSS_PLLB_CLK);
	PMC->PMC_SCER = PMC_SCER_PCK1;
	while (!(PMC->PMC_SCSR & PMC_SCSR_PCK1)) {
	}
	
	gpio_configure_pin(OV_HSYNC_GPIO, OV_HSYNC_FLAGS);
	gpio_configure_pin(OV_VSYNC_GPIO, OV_VSYNC_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D2, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D3, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D4, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D5, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D6, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D7, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D8, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D9, OV_DATA_BUS_FLAGS);
	delay_ms(100);

	configure_twi();
	delay_ms(100);
	
	configure_camera();
	
	/* Init PIO capture*/
	pio_capture_init(OV_DATA_BUS_PIO, OV_DATA_BUS_ID);
	delay_ms(3000);
	
}

void configure_camera(void)
{
	while (ov_init(BOARD_TWI) == 1) {
	}
	ov_configure(BOARD_TWI, JPEG_INIT);
	ov_configure(BOARD_TWI, YUV422);
	ov_configure(BOARD_TWI, JPEG);
	ov_configure(BOARD_TWI, JPEG_640x480);
	
	delay_ms(3000);	
}

uint8_t start_capture(void)
{
	NVIC_EnableIRQ((IRQn_Type)OV2640_VSYNC_ID);

	/* Enable vsync interrupt*/
	pio_enable_interrupt(OV2640_VSYNC_PIO, OV2640_VSYNC_MASK);

	/* Capture acquisition will start on rising edge of Vsync signal.
	 * So wait g_vsync_flag = 1 before start process
	 */
	while (!g_ul_vsync_flag) {
	}

	/* Disable vsync interrupt*/
	pio_disable_interrupt(OV2640_VSYNC_PIO, OV2640_VSYNC_MASK);

	/* Enable pio capture*/
	pio_capture_enable(OV_DATA_BUS_PIO);

	/* Capture data and send it to external SRAM memory thanks to PDC
	 * feature */
	pio_capture_to_buffer(OV_DATA_BUS_PIO,image_buffer ,
			100000 >> 2);
	
	/* Wait end of capture*/
	while (!((OV_DATA_BUS_PIO->PIO_PCISR & PIO_PCIMR_RXBUFF) ==
			PIO_PCIMR_RXBUFF)) {
	}
	
	/* Disable pio capture*/
	pio_capture_disable(OV_DATA_BUS_PIO);

	/* Reset vsync flag*/
	g_ul_vsync_flag = false;
	
	if (find_image_len() != 0){
		return 1;
	}
	
	return 0;
}

uint8_t find_image_len(void)
{
	
	for(uint32_t i=0;i<100000;i++){   
		if((image_buffer[i] ==255)&&(image_buffer[i+1] == 216)){
			start_image =  i;
			break;
		}
	}
	for(uint32_t i=start_image;i<100000;i++){
		if( (image_buffer[i] == 255) && (image_buffer[i+1] == 217)){
			image_length = i+2 -start_image;
			break;
		}
	}
	if(image_length ==0)
		return 0;
	else 
		return 1;
}



