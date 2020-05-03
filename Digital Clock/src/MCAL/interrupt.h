/*
 * Interrupt.h
 *
 *  Created on: Jan 10, 2020
 *      Author: my laptop
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_
/*_______________________________________________________________INCLUDES________________________________________________________________*/

#include<avr/io.h>
#include<avr/interrupt.h>
#include"std_types.h"
#include"common_macros.h"

/*_______________________________________________________________Definitions and Configurations_RELATED________________________________________________________________*/




#define INTERRUPT_NESTING         DISABLE


#define INTERRUPT0                ENABLE
#define INTERRUPT1         		  ENABLE
#define INTERRUPT2                ENABLE


#if INTERRUPT0
#define   INTERNAL_PULLUP0       ENABLE
#endif

#if INTERRUPT1
#define   INTERNAL_PULLUP1       DISABLE
#endif


#if INTERRUPT2
#define   INTERNAL_PULLUP2       ENABLE
#endif


/*_______________________________________________________________ENUMS________________________________________________________________*/
typedef enum
{
	low_level=0,
	any_logical_change=1,
	falling_edge=2,
	rising_edge=3
}ActivateInterruptAt;

//INERUUPT2 SUPPORTS FALLING AND RISING ONLY

/*_______________________________________________________________Structures and Unions________________________________________________________________*/

typedef struct
{
	uint8 interruptNumber;
	ActivateInterruptAt e_trigger;
	void(*callBackFunc_Ptr)(void);
}InterruptSettings;


/*_______________________________________________________________FUNCTION DECLARATIONS________________________________________________________________*/

void Interrupt_Enable(InterruptSettings* a_info_Ptr);
void Interrupt_Disable(InterruptSettings* a_info_Ptr);
void Interrupt_SetCallBackFunc(uint8 a_interruptNumb, void(*a_func_Ptr)(void) );


#endif /* INTERRUPT_H_ */
