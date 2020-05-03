/*
 * timers.c
 *
 *  Created on: Jan 10, 2020
 *      Author: my laptop
 */

/*************************************************
 *                   INCLUDES                    *
 *************************************************/

#include "timers.h"


/*************************************************
 *            PRIVAT FUNC PROTYPE                *
 *************************************************/
static float64 GetFreq(TimerSettings* a_timerSittings_Ptr);

/*************************************************
 *            PRIVAT GLOABAL VARIABLES           *
 *************************************************/

/*************************************************************************************
*                                     TIMER0                                        *
*************************************************************************************/

#if T0_OVF_INTERRUPT
volatile static void (*g_callBackFuncTimer0_OVF_Ptr)(void)=NULL_PTR;
#endif

#if T0_CTC_INTERRUPT
volatile static void (*g_callBackFuncTimer0_CTC_Ptr)(void)=NULL_PTR;
#endif

/*************************************************************************************
*                                     TIMER1                                        *
*************************************************************************************/

#if T1_CTC_INTERRUPT_A
volatile static void (*g_callBackFuncTimer1_CTC_Ptr_A)(void)=NULL_PTR;
#endif

#if T1_CTC_INTERRUPT_B
volatile static void (*g_callBackFuncTimer1_CTC_Ptr_B)(void)=NULL_PTR;
#endif


/*************************************************
 *                      ISR                      *
 *************************************************/

#if T0_OVF_INTERRUPT

ISR(TIMER0_OVF_vect)
{
	if(g_callBackFuncTimer0_OVF_Ptr!=NULL_PTR)
	{
		(*g_callBackFuncTimer0_OVF_Ptr)();
	}
}

#endif

#if T0_CTC_INTERRUPT
ISR(TIMER0_COMP_vect)
{
	if(g_callBackFuncTimer0_CTC_Ptr!=NULL_PTR)
	{
		(*g_callBackFuncTimer0_CTC_Ptr)();
	}
}
#endif



/*************************************************************************************
*                                     TIMER1                                        *
*************************************************************************************/


#if T1_CTC_INTERRUPT_A
ISR(TIMER1_COMPA_vect)
{
	if(g_callBackFuncTimer1_CTC_Ptr_A!=NULL_PTR)
	{
		(*g_callBackFuncTimer1_CTC_Ptr_A)();
	}
}
#endif



#if T1_CTC_INTERRUPT_B
ISR(TIMER1_COMPB_vect)
{
	if(g_callBackFuncTimer1_CTC_Ptr_B!=NULL_PTR)
	{
		(*g_callBackFuncTimer1_CTC_Ptr_B)();
	}
}
#endif



/*************************************************
 *             FUNCTION DEFENATIONS              *
 *************************************************/

void TIMERS_Enable(TimerSettings* a_sittings_Ptr)
{
	bool flage=TRUE;
	(READ_BIT(SREG,7))?(CLEAR_BIT(SREG,7)):(flage=FALSE);

	switch((a_sittings_Ptr->timerNUmb))
	{

/*************************************************************************************
 *                                     TIMER0                                        *
 *************************************************************************************/

#if TIMER0
		case T0:
			switch((uint8)(a_sittings_Ptr->e_ModeSetting))
			{
				#if OVF_MODE
				case (over_flow_mode):
						TCCR0 =(1<<FOC0)|(a_sittings_Ptr->e_clockSetting);
				break;
				#endif

				#if T0_CTC_MODE
				case(compare_mode):
						TCCR0=(1<<FOC0)|(1<<WGM01) | (a_sittings_Ptr->e_clockSetting);

						if(a_sittings_Ptr->CTC_Settings.OCPinSettings!=0)
						{
							SET_BIT(DDRB,PB3); //DDRB|=(1<<PB3);
							TCCR0|=(a_sittings_Ptr->CTC_Settings.OCPinSettings<<COM00);
						}
						OCR0=a_sittings_Ptr->CTC_Settings.compVal;
				break;
				#endif

				#if T0_FAST_PWM
				case(fast_pwm_mode):
						TCCR0=(1<<WGM00)|(1<<WGM01)|(a_sittings_Ptr->PWM_Settings.OCPinSettings<<COM00)|(a_sittings_Ptr->e_clockSetting);
						SET_BIT(DDRB,PB3);
				/*PWM_FREQ = (F_CLOCK)/PRE_SCALER*256)*/
				/* (DUTY_CYCLE/100) = ( (F_CLOCK) / (PRE_SCALER) * (CTC_VAUE) ) / (F_CLOCK) / (PRE_SCALER) * (256) )
				 * SO THE CTC_VALUE = ((DUTY_CYCLE/100)*256)                     */

						switch((uint8)a_sittings_Ptr->PWM_Settings.OCPinSettings)
						{
							case(non_inverted):
								OCR0= ( ( ( a_sittings_Ptr->PWM_Settings.dutycycle ) / 100.0) * 255 );
							break;

							case(inverted):
								OCR0 = ( ( ( ( a_sittings_Ptr->PWM_Settings.dutycycle) / 100.0 ) * 255 ) - 255 );
							break;
						}
				break;
				#endif

				#if T0_PHASE_CORRECT
				case(phase_correct_pwm_mode):
				TCCR0=(1<<WGM00)|(a_sittings_Ptr->PWM_Settings.OCPinSettings<<COM00)|(a_sittings_Ptr->e_clockSetting);
				SET_BIT(DDRB,PB3);
			/*PWM_FREQ = (F_CLOCK)/PRE_SCALER*510)   CAN SUPPORT LOWER FREQ*/
			/* (DUTY_CYCLE/100) = ( (F_CLOCK) / (PRE_SCALER) * (CTC_VAUE) ) / (F_CLOCK) / (PRE_SCALER) * (510) )
			 * SO THE CTC_VALUE = ((DUTY_CYCLE/100)*510)                     */
				switch((uint8)a_sittings_Ptr->PWM_Settings.OCPinSettings)
				{
					case(non_inverted):
					OCR0= ( ( ( a_sittings_Ptr->PWM_Settings.dutycycle ) / 100.0 ) * 510 );
					break;
					case(inverted):
					OCR0 = ( ( ( ( a_sittings_Ptr->PWM_Settings.dutycycle) / 100.0 ) * 510 ) - 510 );
					break;
				}
				break;
				#endif
			}
	#if T0_CTC_INTERRUPT
				ENABLE_T0_CTC_INT;
	#endif

	#if T0_OVF_INTERRUPT
				ENABLE_T0_OVF_INT;
	#endif
	break;
#endif



/*************************************************************************************
*                                     TIMER1                                        *
*************************************************************************************/
#if TIMER1
		case T1:
			switch((uint8)(a_sittings_Ptr->e_ModeSetting))
			{

				#if T1_CTC_MODE
				case(compare_mode):
						TCCR1A=(1<<FOC1A);
						TCCR1B=(1<<WGM12)|a_sittings_Ptr->e_clockSetting;
						if(a_sittings_Ptr->CTC_Settings.OCPinSettings!=0)
						{
							SET_BIT(DDRD,PD5);
							TCCR1A|=(a_sittings_Ptr->CTC_Settings.OCPinSettings<<COM1A1);
						}

						#if COUNTER_B
						if(a_sittings_Ptr->CTC_Settings.OCPinSettings_B!=0)
						{
							TCCR1A|=(a_sittings_Ptr->CTC_Settings.OCPinSettings_B<<COM1B1);
							SET_BIT(DDRD,PD4);
						}
						OCR1B=a_sittings_Ptr->CTC_Settings.compVal_CntrB;
						#endif
						OCR1A=a_sittings_Ptr->CTC_Settings.compVal;
				break;
				#endif

				#if T1_PWM_MODE
				case(fast_pwm_mode):
						TCCR1A=(1<<WGM10)|(1<<WGM11)|(a_sittings_Ptr->PWM_Settings.OCPinSettings<<COM1B0);
						TCCR1B=(1<<WGM12)|(1<<WGM13)|a_sittings_Ptr->e_clockSetting;
						SET_BIT(DDRD,PD4);

				/*PWM_FREQ = (F_CLOCK)/PRE_SCALER*(1+TOP))*/
				/* (DUTY_CYCLE/100) = ( (F_CLOCK) / (PRE_SCALER) * (CTC_VAUE) ) / (F_CLOCK) / (PRE_SCALER) * (1+TOP) )
				 * SO THE CTC_VALUE = ((DUTY_CYCLE/100)*(1+TOP))                     */

						OCR1A= (uint16)( ( GetFreq(a_sittings_Ptr ) / (a_sittings_Ptr->PWM_Settings.freq) ) -1 );

						switch((uint8)a_sittings_Ptr->PWM_Settings.OCPinSettings)
						{
							case(non_inverted):
								OCR1B= (uint16) ( ( ( a_sittings_Ptr->PWM_Settings.dutycycle ) / 100.0) * OCR1A  );
							break;

							case(inverted):
								OCR1B = (uint16) ( ( ( ( a_sittings_Ptr->PWM_Settings.dutycycle) / 100.0 ) * OCR1A ) - OCR1A );
							break;
						}
				break;
				#endif
			}


		#if T1_CTC_INTERRUPT_A
		ENABLE_T1_CTC_INT_A;
		#endif

		#if T1_CTC_INTERRUPT_B
		ENABLE_T1_CTC_INT_B;
		#endif
		break;

#endif
	}
	(flage)?(SET_BIT(SREG,7)):(FALSE);
}


//////////////////////////////////////////////////////////////////////////
void TIMERS_Disable(uint8 a_timerNUmb)
{
	switch(a_timerNUmb)
	{
/*************************************************************************************
*                                     TIMER0                                        *
*************************************************************************************/
	#if TIMER0
	case T0:
		TCCR0&=~((1<<CS00)|(1<<CS01)|(1<<CS02));
		return;
	#endif
/*************************************************************************************
*                                     TIMER1                                        *
*************************************************************************************/
#if TIMER1
	case T1:
		TCCR1B&=(~((1<<CS10)|(1<<CS11)|(1<<CS12)));
		return;
#endif
	}
}

//////////////////////////////////////////////////////////////////////////

/*************************************************************************************
*                                     TIMER0                                        *
*************************************************************************************/
#if T0_OVF_INTERRUPT
void TIMERS_setCallBackFunc_T0_OVF(void (*a_func_Ptr)(void))
{
	g_callBackFuncTimer0_OVF_Ptr=a_func_Ptr;
}
#endif

#if T0_CTC_INTERRUPT
void TIMERS_setCallBackFunc_T0_CTC(void (*a_func_Ptr)(void))
{
	g_callBackFuncTimer0_CTC_Ptr=a_func_Ptr;
}
#endif

/*************************************************************************************
*                                     TIMER1                                        *
*************************************************************************************/

#if T1_CTC_INTERRUPT_A
void TIMERS_setCallBackFunc_T1_CTC_A(void (*a_func_Ptr)(void))
{
	g_callBackFuncTimer1_CTC_Ptr_A=a_func_Ptr;
}
#endif


#if T1_CTC_INTERRUPT_B
void TIMERS_setCallBackFunc_T1_CTC_B(void (*a_func_Ptr)(void))
{
	g_callBackFuncTimer1_CTC_Ptr_B=a_func_Ptr;
}
#endif


#if T1_PWM_MODE
void TIMERS_PWM_T1_ChangeFreq(uint64 freq,TimerSettings* a_timerSittings_Ptr)
{
	OCR1A= (uint16)( ( GetFreq(a_timerSittings_Ptr ) / freq ) -1 );

}
#endif


uint16 TIMERS_ConvertMSCnts(uint32 a_time,TimerSettings* a_timerSittings_Ptr)
{
	 return ( (uint16)( ( a_time / 1000.0 ) * ( GetFreq( a_timerSittings_Ptr ) ) ) );
}


///////////////////////////////////////////////////////////////////////////////////////

static float64 GetFreq(TimerSettings* a_timerSittings_Ptr)
{
	float64 freq=0;

	switch ((uint8)a_timerSittings_Ptr->e_clockSetting)
	{
	case (ck_prescalling_8):
			freq=(F_CPU)/8.0;
	return freq;
	case (ck_prescalling_64):
			freq=(F_CPU)/64.0;
	return freq;
	case (ck_prescalling_256):
			freq=(F_CPU)/256.0;
	return freq;
	case (ck_prescalling_1024):
			freq=(F_CPU)/1024.0;
	return freq;
	}
	return freq;
}




