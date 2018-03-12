/*
 * code.c
 *
 * Created: 3/4/2018 11:32:21 PM
 *  Author: Moham893
 * SAML21E         */
#include <atmel_start.h>
#include <adc_basic_example.h>
#include <adc_basic.h>
#include <atomic.h>
#include <stdio.h>
#include <string.h>
#include <click_example.h>
#include <clock_config.h>
#include <util/delay.h>
#include <RS232_click.h>
#include <usart_basic.h>

/*
Necessary Methods
extern void RS232_write(uint8_t data);
uint8_t RS232_read(void);
*/

int main(void){

	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Initializing ADC  */
	ADC_0_init();

	/* Begin application code */
	uint8_t old_state_btn_one;
	uint8_t old_state_btn_two;
	uint8_t state_btn_one;
	uint8_t state_btn_two;
	uint8_t msg;
	uint16_t actual_temperature_volts;
	uint16_t desired_temperature = 25;
	uint16_t desired_temp_volts;
	char *message;
	
// Setup Pins
	// Buttons
	DDRA &= ~(0x09); // set pin 0,3 to input (button1_pin0, button2_pin3)
	
	// heater (pin0) and AC unit (pin3) && temperature sensor MCP9700 and ADC
	DDRB |= (0x09); // set pin 0,3 to output (heater0 and AC_unit3)
	DDRB &= ~(1 << 6); //  pin 6 to input (temperature sensor)
	// temperature sensor MCP9700 maps to channel AIN[16] for ADC
	
// Loop
	// initial poll of buttons
	old_state_btn_one = (PINA & (0x01));
	old_state_btn_two = (PINA) & (0x08);

	while (1) {
		
		// read buttons, decrement/increment desired temperature using the 2
		state_btn_one = (PINA & (0x01));
		state_btn_two = (PINA & (0x08));
		if((old_state_btn_one & (0x01)) != (state_btn_one & (0x01))){
			// increment desired temperature
			message = "Temperature Incremented\r\n";
			msg = (uint8_t)(message[0]);
			RS232_write(msg);
			desired_temperature += 1;
			old_state_btn_one = (state_btn_one);
		}
		if((old_state_btn_two & (0x08)) != (state_btn_two & (0x08))){
			// decrement desired temperature
			message = "Temperature Decremented\r\n";
			msg = (uint8_t)(message[0]);
			RS232_write(msg);
			desired_temperature -= 1;			
			old_state_btn_two = (state_btn_two);
		}
		
		// calculate desired temperature in voltage (MCP9700 Data Sheet pg 2)
		// 500 mV = 0°C, 10mV/1°C in addition
		// we'll assume fan shouldn't be operating below freezing
		desired_temp_volts = 500 + (desired_temperature * 10); // Units: mV
		
		// read temperature from analog temperature sensor (MCP9700)
		actual_temperature_volts = ADC_0_get_conversion(16); // pin 6 for (temperature sensor), maps to channel AIN[16] for ADC Channel[Xplained datasheet pg29]
		// Compare temperature to desired_temperature
		if (actual_temperature_volts > desired_temp_volts){
			//turn a heater on/off using a GPIO pin when temp falls below desired
			message = "Heater turned off, AC turned on\r\n";
			msg = (uint8_t)(message[0]);
			RS232_write(msg);
			PORTB |= (0x01);
			PORTB &= ~(0x08);
			
		}
		else if (actual_temperature_volts < desired_temp_volts){
			// turn AC unit on using a GPIO pin when temp rises above desired
			message = "AC turned off, Heater turned on\r\n";
			msg = (uint8_t)(message[0]);
			RS232_write(msg);
			PORTB |= (0x08);
			PORTB &= ~(0x01);
		}
		
		
	}
}
