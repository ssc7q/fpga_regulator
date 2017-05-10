/*
 * main.c
 *
 *  Created on: 29.03.2017
 *      Author: vmed
 */
#include "C:\vmed\system8051s_hw_platform.h"
#define APB_base 0xF000U
#define GPIO_addr APB_base + COREGPIO_0

__xdata at GPIO_addr + 0xA0 unsigned char gpio_out;
__xdata at GPIO_addr + 0x90 unsigned char gpio_in;
__xdata at GPIO_addr unsigned char global_config_reg00;
__xdata at GPIO_addr + 0x04 unsigned char global_config_reg01;
__xdata at GPIO_addr + 0x08 unsigned char global_config_reg02;
__xdata at GPIO_addr + 0x0с unsigned char global_config_reg03;

unsigned char inreg;
unsigned char outreg;

#define en_reg 0x03; //enable inreg, outreg for particular bit

void main(){
	volatile int a;
	volatile int config_reg01;
	outreg = 0x00;
	inreg = 0x00;
	config_reg01 = global_config_reg01;
	global_config_reg01 = 0x03;
	global_config_reg00 = 0x03;
	global_config_reg02 = 0x03;
	global_config_reg03 = 0x03;
	a = 1;
	for(;;){
		a++;
		inreg = gpio_in;
	}
}
