
#define GPIOTE_ENABLED 1


#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"
#include "boards.h"
#include "shiftreg.h"


#define ADC_IN BSP_BUTTON_0
#define DIGDIV_IN BSP_BUTTON_1


void in_adc_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
//	if(action == NRF_GPIOTE_POLARITY_LOTOHI ){//NRF_GPIOTE_POLARITY_HITOLO){
		selectAdc();
		makeShiftReg();
		programDevice();
		//	}
}

void in_digdiv_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{

	selectDigDiv();
	makeShiftReg();
	programDevice();


}
/**
 * @brief Function for configuring: PIN_IN pin for input, PIN_OUT pin for output, 
 * and configures GPIOTE to give an interrupt on pin change.
 */
static void gpio_init(void)
{
    ret_code_t err_code;

    err_code = nrf_drv_gpiote_init();
//    APP_ERROR_CHECK(err_code);
    
    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_LOTOHI(true);
    in_config.pull = NRF_GPIO_PIN_PULLUP;

    err_code = nrf_drv_gpiote_in_init(ADC_IN, &in_config, in_adc_handler);
	//  APP_ERROR_CHECK(err_code);
	
    err_code = nrf_drv_gpiote_in_init(DIGDIV_IN, &in_config, in_digdiv_handler);
//    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_event_enable(ADC_IN, true);
}


/**
 * @brief Function for application main entry.
 */
int main(void)
{

	gpio_init();

	initialize();
	makeShiftReg();

	
    // Toggle LEDs.
    while (true)
    {
		
	}
}


