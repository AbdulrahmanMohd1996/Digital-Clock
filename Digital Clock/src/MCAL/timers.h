/*
 * timers.h
 *
 *  Created on: Jan 10, 2020
 *      Author: my laptop
 */

#ifndef TIMERS_H_
#define TIMERS_H_

/*************************************************
 *                   INCLUDES                    *
 *************************************************/


#include"std_types.h"
#include"common_macros.h"
#include<avr/io.h>
#include<avr/interrupt.h>


/*************************************************
 *       Definitions and Configurations          *
 *************************************************/



#define T0 									0
#define T1 									1

/*************************************************************************************
*                                     TIMER0                                        *
*************************************************************************************/


#define TIMER0                        DISABLE
#define TIMER1                        ENABLE


#define OVF_MODE                      DISABLE
#define CTC_MODE                      ENABLE
#define PWM_MODE                      DISABLE


#if TIMER1
#define COUNTER_B                      DISABLE
#endif

#if CTC_MODE

#if TIMER0
#define T0_CTC_MODE                  ENABLE
#endif

#if TIMER1
#define T1_CTC_MODE                  ENABLE
#endif

#endif


#if PWM_MODE

#if TIMER0
#define T0_PWM_MODE                  ENABLE
#endif

#if TIMER1
#define T1_PWM_MODE                  ENABLE

#endif

#endif


#if T0_PWM_MODE
#define T0_FAST_PWM                     ENABLE
#define T0_PHASE_CORRECT                DISABLE
#endif


#if TIMER0
#define T0_OVF_INTERRUPT                 DISABLE
#define T0_CTC_INTERRUPT				 DISABLE

#endif


#if TIMER1
#define T1_CTC_INTERRUPT_A				 ENABLE
#define T1_CTC_INTERRUPT_B				 DISABLE
#endif
/*************************************************
 *                   ENUMS                       *
 *************************************************/

typedef enum
{
	over_flow_mode=0,
	compare_mode=1,
	fast_pwm_mode=2,
	phase_correct_pwm_mode=3

}Modes;


typedef enum
{
	no_clock=0,
	ck_prescalling_0=1,
	ck_prescalling_8=2,
	ck_prescalling_64=3,
	ck_prescalling_256=4,
	ck_prescalling_1024=5,
	external_clock_falling_edge=6,
	external_clock_rising_edge=7,

}Clock;

#if CTC_MODE
typedef enum
{
	ctc_signal_disconnected=0,
	toggle_pin_at_match=1,
	clear_pin_at_match=2,
	set_pin_at_match=3
}CTC_Modes;
#endif

#if PWM_MODE
typedef enum
{
	pwm_signal_disconnected=0,
	non_inverted=2,
	inverted=3
}PWM_Modes;
#endif


/*************************************************
 *                   STRUCTURES                  *
 *************************************************/


typedef struct
{
	#if CTC_MODE
	struct
	{
		uint16 compVal;

		#if COUNTER_B
		uint16 compVal_CntrB;
		CTC_Modes OCPinSettings_B:2;
		#endif

		CTC_Modes OCPinSettings:2;

		}CTC_Settings;
	#endif

		#if PWM_MODE
		struct
		{
			uint16 dutycycle;
			PWM_Modes OCPinSettings:2;

			#if COUNTER_B
			uint16 freq;
			#endif

		}PWM_Settings;
		#endif

		Clock e_clockSetting:8;
		uint8 timerNUmb:2;
		Modes e_ModeSetting:2;
}TimerSettings;

/*************************************************
 *                 DRIVER MACROS                 *

 *************************************************/


/*************************************************************************************
*                                     TIMER0                                        *
*************************************************************************************/
#if TIMER0
	#define ENABLE_T0_OVF_INT   			 			  		(SET_BIT(TIMSK,TOIE0))
	#define DISABLE_T0_OVF_INT   								(CLEAR_BIT(TIMSK,TOIE0))

	#define ENABLE_T0_CTC_INT   								(SET_BIT(TIMSK,OCIE0))
	#define DISABLE_T0_CTC_INT   								(CLEAR_BIT(TIMSK,OCIE0))

	#if T0_FAST_PWM
	#define T0_NON_INVERT_DUTY_CYCLE(VAL)   					OCR0 = ( ( VAL / 100.0) * 255 );
	#define T0_INVERT_DUTY_CYCLE(VAL)   						OCR0 = ( ( ( VAL / 100.0) * 255 ) - 255);
	#endif

	#if T0_PHASE_CORRECT
	#define T0_NON_INVERT_DUTY_CYCLE_VAL(VAL)   				OCR0 = ( ( VAL / 100.0) * 510 );
	#define T0_INVERT_DUTY_CYCLE_VAL(VAL)   					OCR0 = ( ( ( VAL / 100.0) * 510 ) - 510);
	#endif

    #define READ_T0_CONTR()                                      (TCNT0)
	#define RESET_T0_CNTR()                                      TCNT0=0
#endif

/*************************************************************************************
*                                     TIMER1                                        *
*************************************************************************************/
#if TIMER1
	#if T1_CTC_INTERRUPT_A
	#define ENABLE_T1_CTC_INT_A  (SET_BIT(TIMSK,OCIE1A))
	#define DISABLE_T1_CTC_INT_A   (CLEAR_BIT(TIMSK,OCIE1A))
	#endif

	#if T1_CTC_INTERRUPT_B
	#define ENABLE_T1_CTC_INT_B   (SET_BIT(TIMSK,OCIE1B))
	#define DISABLE_T1_CTC_INT_B   (CLEAR_BIT(TIMSK,OCIE1B))
	#endif

	#ifdef T1_PWM_MODE
	#define T1_NON_INVERT_DUTY_CYCLE_VAL(VAL)   OCR1B = ( ( VAL / 100.0) * OCR1A );
	#define T1_INVERT_DUTY_CYCLE_VAL(VAL)   OCR1B = ( ( ( VAL / 100.0) * 255 ) - OCR1A);
	#endif

	#define READ_T1_CONTR()                                      (TCNT1)
	#define RESET_T1_CNTR()                                      TCNT1=0

#endif



/*************************************************
 *             FUNCTIONS PROTO TYPES              *
 *************************************************/


void TIMERS_Enable(TimerSettings* a_sittings_Ptr);
void TIMERS_Disable(unsigned char a_timerNUmb);

uint16 TIMERS_ConvertMSCnts(uint32 a_time,TimerSettings* a_timerSittings_Ptr);

#if TIMER0
void TIMERS_setCallBackFunc_T0_OVF(void (*a_func_Ptr)(void));
void TIMERS_setCallBackFunc_T0_CTC(void (*a_func_Ptr)(void));
#endif

#if TIMER1

#if PWM_MODE
void TIMERS_PWM_T1_ChangeFreq(uint64 freq,TimerSettings* a_timerSittings_Ptr);
#endif
#if T1_CTC_INTERRUPT_A
void TIMERS_setCallBackFunc_T1_CTC_A(void (*a_func_Ptr)(void));
#endif
#if T1_CTC_INTERRUPT_B
void TIMERS_setCallBackFunc_T1_CTC_B(void (*a_func_Ptr)(void));
#endif

#endif


#endif /* TIMERS_H_ */
