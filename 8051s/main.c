#include "system8051s_hw_platform.h"
//#include "floatlib.c"
#define APB3_BASE_ADRESS	0xF000
#define GPIO0_addr APB3_BASE_ADRESS + COREGPIO_0
#define GPIO1_addr APB3_BASE_ADRESS + COREGPIO_1
#define TIMER_addr APB3_BASE_ADRESS + CORETIMER_1

__xdata at GPIO0_addr unsigned char configREG_0;
__xdata at GPIO1_addr unsigned char configREG_1;

__xdata at GPIO0_addr + 0x90 unsigned char datain;
__xdata at GPIO1_addr + 0xA0 unsigned char dataout;

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


#define u0				0.009
#define RADIAN_PER_CNT 	0.0000304948
#define CLK 			0.2

__xdata volatile float Wx, R0, R1, R2, R3, SR, SP, P0, P1, P2, P3, Q0, Q1, Q2, Q3, u1 = 0.0;
__xdata volatile signed short cnt, pre_cnt = 0;
__xdata volatile int i = 0;

volatile signed short getcWx(signed short, signed short);
volatile unsigned char PWM(float);

void main(){

	for(i; i<12; i++){
		*(&configREG_0 + i*0x04)=0x03;
		*(&configREG_1 + i*0x04)=0x03;
	}



	R0=0.7071;
	R1=0.7071;
	//****timer****
	XWB3=0x00;
	XWB2=0x01;
	XWB1=0x31;
	TimerLoad=0x2D; // 5 000 000976 //0.2 sec timer
	TimerPrescale=0x07; // :128
	TimerControl=0x03; //011
	//****timer****

	XWB3 = 0;
	XWB2 = 0;
	XWB1 = 0;

	for(;;){
		if(TimerRIS==1){
			XWB3 = 0x00;
			XWB2 = 0x00;
			if(u1<0.0){
				XWB1 = 0x02;
				dataout = PWM((-1)*u1);}
			else{
				XWB1 = 0x03;
				dataout = PWM(u1);}

			cnt = XRB1*sizeof(datain) + datain;
			XRB3 = 0x00;
			XRB2 = 0x00;
			XRB1 = 0x00;
			Wx = (float)getcWx(cnt, pre_cnt);
			Wx = Wx*RADIAN_PER_CNT;

			P1 = Wx/2.0*CLK;
			P2 = 0.0;//Wy/2*CLK;
			P3 = 0.0;//Wz/2*CLK;

			SP = P1*P1 + P2*P2 + P3*P3;
			SR = R0*R0 + R1*R1 + R2*R2 + R3*R3;

			P0 = (3.0 - SP - SR)/2;

			Q0 = R0*P0 - R1*P1 - R2*P2 - R3*P3;
			Q1 = R0*P1 + R1*P0 + R2*R3 - R3*P2;
			Q2 = R0*P2 + R2*P0 + R3*P1 - R1*P3;
			Q3 = R0*P3 + R3*P0 + R1*P2 - R2*P1;

			u1 = -(2*(Q0*Q1) + 7*Wx);
			//u2 = -(2*(Q0*Q2) + 7*Wy);
			//u3 = -(2*(Q0*Q3) + 7*Wz);

			R0 = Q0;
			R1 = Q1;
			R2 = Q2;
			R3 = Q3;
			pre_cnt = cnt;
			XWB3 = 0;
			XWB2 = 0x00;
			XWB1 = 0x00;
			dataout = 0x00;
			}
		}
	}

signed short getcWx(signed short cnt, signed short pre_cnt){
	if(cnt>=pre_cnt){
		if((cnt-pre_cnt)<1024)
			return cnt-pre_cnt;
		else
			return cnt-(2048+pre_cnt);}
	else{
		if((pre_cnt-cnt)<1024)
			return cnt-pre_cnt;
		else
			return 2048+cnt-pre_cnt;}
}

unsigned char PWM(float u){
	if(u<u0)
		return 0x00;
	else if(u0<u && u<=(1.5*u0))
		return 0x28; //40 impulses
	else if((1.5*u0)<u && u<=(2.0*u0))
		return 0x50; //80 impulses
	else if((2.0*u0)<u && u<=(2.5*u0))
		return 0x78; //120
	else if((2.5*u0)<u && u<=(3.0*u0))
		return 0xA0; //160
	else
		return 0xC8; //200
}
