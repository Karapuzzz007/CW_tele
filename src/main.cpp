#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/cm3/nvic.h>
#include <stdio.h>

//int counter = 0;

void setup_clock(){
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_SPI1);
    rcc_periph_clock_enable(RCC_GPIOE);
}

void setup_gpio (){
   gpio_mode_setup(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO9); 
}

void setup_spi () {

    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO5 | GPIO6 | GPIO7);
    gpio_set_af(GPIOA, GPIO_AF5, GPIO5 | GPIO6 | GPIO7);
    gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO5 | GPIO6 | GPIO7);

    spi_disable(SPI1);
    spi_set_master_mode(SPI1);
    spi_set_baudrate_prescaler(SPI1, SPI_CR1_BR_FPCLK_DIV_64);
    spi_set_clock_polarity_0(SPI1);
    spi_set_clock_phase_0(SPI1);
    spi_set_data_size(SPI1, SPI_CR2_DS_8BIT);
    spi_send_msb_first(SPI1);
    spi_enable_software_slave_management(SPI1);
    spi_set_nss_high(SPI1);
    spi_fifo_reception_threshold_8bit(SPI1);

    spi_enable(SPI1);

}

void setup(){
    setup_clock();
    setup_gpio();
    setup_spi();
}

void loop () {
    
    spi_send8(SPI1, 0x54);
    gpio_toggle(GPIOE, GPIO9);
    uint16_t rx_value = spi_read8(SPI1);
    
    //printf("    SPI Received Byte: %i\r\n", rx_value);
    for(volatile uint32_t i=0; i<100000; i++);
    //counter++;
}

int main () {
    setup();
    while (true) {
        loop();
    }   
}
