/*
 * app.c
 *
 *  Created on: Jan 13, 2020
 *      Author: my laptop
 */


#include "MCAL/interrupt.h"
#include "MCAL/timers.h"
#include<util/delay.h>

uint8 g_sec=0;
uint8 g_min=0;
uint8 g_hour=0;


void resetFunc(void);
void pauseFunc(void);
void resumeFunc(void);

void timerFunc(void);

InterruptSettings S_Reset;
InterruptSettings S_Pause;
InterruptSettings S_Resume;

TimerSettings S_Clock;

int main(void)
{
	SET_GLOBAL_INTERRUPT();

	S_Reset.interruptNumber=0;
	S_Reset.e_trigger=falling_edge;
	S_Reset.callBackFunc_Ptr=resetFunc;
	Interrupt_Enable(&S_Reset);

	S_Pause.interruptNumber=1;
	S_Pause.e_trigger=rising_edge;
	S_Pause.callBackFunc_Ptr=pauseFunc;
	Interrupt_Enable(&S_Pause);

	S_Resume.interruptNumber=2;
	S_Resume.e_trigger=falling_edge;
	S_Resume.callBackFunc_Ptr=resumeFunc;
	Interrupt_Enable(&S_Resume);



	S_Clock.timerNUmb=1;
	S_Clock.e_clockSetting=ck_prescalling_1024;
	S_Clock.e_ModeSetting=compare_mode;
	S_Clock.CTC_Settings.compVal=TIMERS_ConvertMSCnts(1000,&S_Clock);
	S_Clock.CTC_Settings.OCPinSettings=ctc_signal_disconnected;
	TIMERS_setCallBackFunc_T1_CTC_A(timerFunc);
	TIMERS_Enable(&S_Clock);


	SET_BIT(DDRC,0);
	SET_BIT(DDRC,1);
	SET_BIT(DDRC,2);
	SET_BIT(DDRC,3);


	SET_BIT(DDRA,0);
	SET_BIT(DDRA,1);
	SET_BIT(DDRA,2);
	SET_BIT(DDRA,3);
	SET_BIT(DDRA,4);
	SET_BIT(DDRA,5);

	while(1)
	{


		if(g_sec>=60)
		{
			g_sec=0;
			g_min++;
			if(g_min>=60)
			{
				g_min=0;
				g_hour++;
			}
		}


		SET_BIT(PORTA,0);
		PORTC= ( ( PORTC & 0XF0 ) | (g_sec%10) );
		_delay_ms(3);
		CLEAR_BIT(PORTA,0);

		SET_BIT(PORTA,1);
		PORTC= ( ( PORTC & 0XF0 ) | ( g_sec/10 ) );
		_delay_ms(3);
		CLEAR_BIT(PORTA,1);

		SET_BIT(PORTA,2);
		PORTC= ( ( PORTC & 0XF0 ) | (g_min%10) );
		_delay_ms(3);
		CLEAR_BIT(PORTA,2);

		SET_BIT(PORTA,3);
		PORTC= ( ( PORTC & 0XF0 ) | ( g_min/10 ) );
		_delay_ms(3);
		CLEAR_BIT(PORTA,3);

		SET_BIT(PORTA,4);
		PORTC= ( ( PORTC & 0XF0 ) | (g_hour%10));
		_delay_ms(3);
		CLEAR_BIT(PORTA,4);

		SET_BIT(PORTA,5);
		PORTC= ( ( PORTC & 0XF0 ) | ( g_hour/10 ) );
		_delay_ms(3);
		CLEAR_BIT(PORTA,5);

	}


	return 0;
}






void timerFunc(void)
{
	g_sec++;
}


void resetFunc(void)
{

	g_sec=0;
	g_min=0;
	g_hour=0;
	RESET_T1_CNTR();

}


void pauseFunc(void)
{
	TIMERS_Disable(T1);
}


void resumeFunc(void)
{
	TIMERS_Enable(&S_Clock);
}

