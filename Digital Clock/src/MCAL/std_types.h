/*
 * std_types.h
 *
 *  Created on: Jan 10, 2020
 *      Author: my laptop
 */

#ifndef STD_TYPES_H_
#define STD_TYPES_H_

typedef unsigned char bool;

#ifndef TRUE
#define TRUE (1u)
#endif

#ifndef FALSE
#define FALSE (0u)
#endif

#define ENABLE (1u)
#define DISABLE (0u)


#define NULL_PTR (void*)0

typedef unsigned char uint8;                        	/*  0 to 255   								  */
typedef signed char sint8;   						   /*  (0 to 127 ) & (-1 to-128)       			 */
typedef unsigned short uint16;  					  /*  0 to 65536   					  			*/
typedef unsigned short sint16;  					 /*  (0 to  32767) & (-1 to 32768 )   		   */
typedef unsigned long uint32;   				    /*  0 to 4293967296   						  */
typedef unsigned long sint32;   				   /*  (0 to 2146980647) & (-1 to 2146980648)   */
typedef unsigned long long uint64;
typedef unsigned long long sint64;
typedef float float32;
typedef double float64;






#endif /* STD_TYPES_H_ */
