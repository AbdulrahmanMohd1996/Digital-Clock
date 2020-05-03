/*
 * common_macros.h
 *
 *  Created on: Jan 11, 2020
 *      Author: my laptop
 */

#ifndef COMMON_MACROS_H_
#define COMMON_MACROS_H_

//#define CPU_F           (1000000U)

#define SET_BIT(REG,PIN)     (REG|=(1<<PIN))
#define CLEAR_BIT(REG,PIN)   (REG&=~(1<<PIN))
#define TOOGLE_BIT(REG,PIN)  (REG^=(1<<PIN))
#define READ_BIT(REG,PIN)    ((REG&(1<<PIN))>>PIN)

#define SET_GLOBAL_INTERRUPT()   (SET_BIT(SREG,7))
#define CLEAR_GLOBAL_INTERRUPT()   (CLEAR_BIT(SREG,7))


#endif /* COMMON_MACROS_H_ */
