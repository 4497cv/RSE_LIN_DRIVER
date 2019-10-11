/**
	\file adc.h
	\brief
		This is the header file for the Analog to Digital Converter device driver for Kinetis K64.
		It contains all the implementation for configuration functions and runtime functions.
	\authors: César Villarreal Hernández, ie707560
	          José Luis Rodríguez Gutierrez,ie705694
	\date	  29/03/2019
 */

#ifndef adc_H_
#define adc_H_

#include <stdint.h>
#include "bits.h"
#include "MK64F12.h"
#include "fsl_gpio.h"
#define ANALOG_LIMIT 3.3F
#define DIGITAL_LIMIT 255

/*\brief Funct to initialize the SE_ADC
 *\param[add] void
 * */
void ADC_init(void);

/*\brief Funct used to get direct values from adc by the registers
 *\param[add] void
 * */
uint8_t ADC_result(void);

/*\brief Funct used to return the value read on the ADC and used them later
 *\param[add] void
 * */
uint8_t ADC_read(void);

#endif
