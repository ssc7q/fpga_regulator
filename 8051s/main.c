#include "system8051s_hw_platform.h"

#define APB3_BASE_ADRESS	0xF000
#define GPIO0_addr APB3_BASE_ADRESS + COREGPIO_0
#define GPIO1_addr APB3_BASE_ADRESS + COREGPIO_1
#define TIMER_addr APB3_BASE_ADRESS + CORETIMER_1



__xdata at GPIO1_addr + 0xA0 unsigned char dataout;


__xdata at GPIO0_addr unsigned char configREG_0;
__xdata at GPIO0_addr + 0x04 unsigned char configREG_1;
__xdata at GPIO0_addr+0x08 unsigned char configREG_2;
__xdata at GPIO0_addr+0x0C unsigned char configREG_3;
__xdata at GPIO0_addr+0x10 unsigned char configREG_4;
__xdata at GPIO0_addr+0x14 unsigned char configREG_5;


__xdata at GPIO0_addr + 0x90 unsigned char datain;

__xdata at TIMER_addr unsigned char TimerLoad;
__xdata at TIMER_addr + 0x08 unsigned char TimerControl;
__xdata at TIMER_addr + 0x0C unsigned char TimerPrescale;
__xdata at TIMER_addr + 0x10 unsigned char TimerIntClr;
__xdata at TIMER_addr + 0x14 unsigned char TimerRIS;

__data at 0x9A unsigned char XWB1;
__data at 0x9B unsigned char XWB2;
__data at 0x9C unsigned char XWB3;

__data at 0x9D unsigned char XRB1;
__data at 0x9E unsigned char XRB2;
__data at 0x9F unsigned char XRB3;

unsigned char temp;
unsigned char a;
unsigned char cnt;
volatile int i;
void main(){
	for(i = 0; i<12; i++){
		*(&configREG_0 + i*0x04)=0x03;
	}

	XWB3=0x00;
	XWB2=0x4C;
	XWB1=0x4B;
	TimerLoad=0x40; //1 sec timer
	TimerPrescale=0x00;//5Mhz
	TimerControl=0x03; //011

	for(;;){
		if(TimerRIS==1){
			XWB3=0x00;
			XWB2=0x00;
			XWB1=0x00;
			cnt=datain-a;
			a=datain-a;
			if(temp==0x00){
				if(cnt<10)
				XWB3=0x00;
				XWB2=0x00;
				XWB1=0x02;//0000 0010 for <0, 0000 0011 for >0
				dataout=0xFF;
				temp=0x01;
			}
			else{
			XWB3=0x00;
			XWB2=0x00;
			XWB1=0x00;
			dataout=0x00;
			temp=0x00;}
			TimerIntClr=0;
			}
		}
	}
