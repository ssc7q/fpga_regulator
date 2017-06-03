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
#define RADIAN_PER_CNT 	0.0000304
#define CLK 			0.2

__xdata volatile float Wx, R0, R1, R2, R3, SR, SP, P0, P1, P2, P3, Q0, Q1, Q2, Q3, u1 = 0.0;
__xdata volatile unsigned short cnt, pre_cnt = 0;
__xdata volatile int i = 0;
__xdata volatile unsigned char pwm = 0;
volatile signed short getcWx(unsigned short, unsigned short);
volatile unsigned char PWM(float);

void main(){

	for(i; i<12; i++){
		*(&configREG_0 + i*0x04)=0x03;
		*(&configREG_1 + i*0x04)=0x03;
	}

	//****timer****
	XWB3=0x00;
	XWB2=0x00;
	XWB1=0x3D;
	TimerLoad=0x09; // 15 625 //0.2 sec timer
	TimerPrescale=0x06; // :128
	TimerControl=0x03; //011
	//****timer****

	R0=0.7071;
	R1=0.7071;

	for(;;){
		if(TimerRIS==1){
			TimerIntClr = 0;
			XWB3 = 0x00;
			XWB2 = 0x00;
			XWB1 = 0x00;
		/*********check clk********
			if(dataout==0x00){
				dataout = 0x11;}
			else{
				dataout = 0x00;}
		//*********check impulse*****/

		//********************PWM********************
			if(u1>0.0){
				pwm = PWM(u1);
				XWB1 = 0x03;
				dataout = pwm;}
			else{
				pwm = PWM(-u1);
				XWB1 = 0x02;
				dataout = pwm;}
		//********************PWM********************/

			pre_cnt = cnt;
			cnt = (unsigned short)datain;
			cnt = (unsigned short)XRB1*256 + cnt;

			Wx = getcWx(cnt, pre_cnt)*RADIAN_PER_CNT/CLK;

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


			if(Wx>-0.1)
				u1 = -1;
			else
				u1 = 0;

			if(R1<0.34202) //turn angle
				u1 = -(2*(Q0*Q1)+7*Wx);

			//u1 = -(2*(Q0*Q1) + 7*Wx);
			//u2 = -(2*(Q0*Q2) + 7*Wy);
			//u3 = -(2*(Q0*Q3) + 7*Wz);

			R0 = Q0;
			R1 = Q1;
			R2 = Q2;
			R3 = Q3;

			XWB3 = 0;
			XWB2 = 0x00;
			XWB1 = XWB1-0x02;
			dataout = 0x00;
			}
		}
	}

signed short getcWx(unsigned short cnt, unsigned short pre_cnt){
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
		return 0xC7; //200
}
