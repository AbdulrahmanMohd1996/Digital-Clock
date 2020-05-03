/*
 * Interrupt.c
 *
 *  Created on: Jan 10, 2020
 *      Author: my laptop
 */

/*_______________________________________________________________INCLUDES________________________________________________________________*/

#include "interrupt.h"

/*_______________________________________________________________Flobal Variables________________________________________________________________*/
#if INTERRUPT0
static volatile  void(*g_INT0_callBack_Ptr)(void)=NULL_PTR;
#endif

#if INTERRUPT1
static volatile  void(*g_INT1_callBack_Ptr)(void)=NULL_PTR;
#endif

#if INTERRUPT2
static volatile  void(*g_INT2_callBack_Ptr)(void)=NULL_PTR;
#endif
/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
#if INTERRUPT0

ISR(INT0_vect)
{

#if INTERRUPT_NESTING
	SREG|=(1<<7);
#endif

	if(g_INT0_callBack_Ptr!=NULL_PTR)
	{
		(*g_INT0_callBack_Ptr)();
	}
}

#endif


#if INTERRUPT1
ISR(INT1_vect)
{
#if INTERRUPT_NESTING
	SREG|=(1<<7);
#endif

	if(g_INT1_callBack_Ptr!=NULL_PTR)
	{
		(*g_INT1_callBack_Ptr)();
	}
}
#endif


#if INTERRUPT2
ISR(INT2_vect)
{
#if INTERRUPT_NESTING
	SREG|=(1<<7);
#endif

	if(g_INT2_callBack_Ptr!=NULL_PTR)
	{
		(*g_INT2_callBack_Ptr)();
	}

}
#endif

/*_______________________________________________________________FUNCTION Definitions________________________________________________________________*/




void Interrupt_Enable(InterruptSettings* a_info_Ptr)
{
	bool flage=TRUE;
	(READ_BIT(SREG,7))?(CLEAR_BIT(SREG,7)):(flage=FALSE);

	switch (a_info_Ptr->interruptNumber)
	{

	#if INTERRUPT0
	case 0:
		DDRD&=~(1<<PD2);
		#if INTERNAL_PULLUP0
		PORTD|=(1<<PD2);
		#endif

		g_INT0_callBack_Ptr= a_info_Ptr->callBackFunc_Ptr;
		GICR&=~(1<<INT0);
		MCUCR=(((MCUCR)&(~((1<<ISC00)|(1<<ISC01))))|(a_info_Ptr->e_trigger));
		GICR|=(1<<INT0);
		break;
#endif

#if INTERRUPT1
	case 1:
		DDRD&=~(1<<PD3);
#if INTERNAL_PULLUP1
		PORTD|=(1<<PD3);
#endif
		g_INT1_callBack_Ptr= a_info_Ptr->callBackFunc_Ptr;
		GICR&=~(1<<INT1);
		MCUCR=(((MCUCR)&(~((1<<ISC01)|(1<<ISC11))))|(a_info_Ptr->e_trigger));
		GICR|=(1<<INT1);
		break;
#endif

#if INTERRUPT2
	case 2:
		DDRB&=~(1<<PB2);
#if INTERNAL_PULLUP2
		PORTB|=(1<<PB2);
#endif
		g_INT2_callBack_Ptr= a_info_Ptr->callBackFunc_Ptr;
		GICR&=~(1<<INT2);
		( ( a_info_Ptr->e_trigger )== ( rising_edge ) )? ( MCUCSR|=(1<<ISC2) ) : ( MCUCSR&= ~(1<<ISC2) );   //number two as to make the value of enum be able to use it in the interrupt register as well
		GICR|=(1<<INT2);
		break;
#endif

	}

	(flage)?(SET_BIT(SREG,7)):(FALSE);

}


void Interrupt_Disable(InterruptSettings* a_info_Ptr)
{
	switch (a_info_Ptr->interruptNumber)
	{

#if INTERRUPT0
	case 0:
		GICR&=~(1<<INT0);
		return;
#endif

#if INTERRUPT1
	case 1:
		GICR&=~(1<<INT1);
		return;
#endif

#if INTERRUPT2
	case 2:
		GICR&=~(1<<INT2);
		return;
#endif

	}

}


void Interrupt_SetCallBackFunc(unsigned char a_interruptNumb, void(*a_func_Ptr)(void) )
{
	switch (a_interruptNumb)
	{

#if INTERRUPT0
	case 0:
		g_INT0_callBack_Ptr=a_func_Ptr;
		return;
#endif

#if INTERRUPT1
	case 1:
		g_INT1_callBack_Ptr=a_func_Ptr;
		return;
#endif

#if INTERRUPT2
	case 2:
		g_INT2_callBack_Ptr=a_func_Ptr;
		return;
#endif

	}


}

