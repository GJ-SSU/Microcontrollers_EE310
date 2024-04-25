/* 
 * File:   Variables.h
 * Author: gjarv
 *
 * Created on April 25, 2024, 9:45 AM
 */
#include <xc.h>


#ifndef VARIABLES_H
#define	VARIABLES_H

#ifdef	__cplusplus
extern "C" {
#endif

//---------------------
// Program Inputs
//---------------------
unsigned char Secret_Code = 0x11;
unsigned char Secret_Code_Digit_A = 0x00;
unsigned char Secret_Code_Digit_B = 0x00;
unsigned char UserInputA = 0x00;
unsigned char UserInputB = 0x00;
unsigned char Mode_Sel = 0x00;
unsigned char Prior_Action = 0x00;
unsigned char Current_Action = 0x00;
#define Timer_Length 9000;
unsigned int  Move_On_Timer = Timer_Length;

//---------------------
// Program Constants
//---------------------
char SevenSeg0 = 0xFC;
char SevenSeg1 = 0x60;
char SevenSeg2 = 0xDA;
char SevenSeg3 = 0xF2;
char SevenSeg4 = 0x66;
char SevenSeg5 = 0xB6;
char SevenSeg6 = 0xBE;
char SevenSeg7 = 0xE0;
char SevenSeg8 = 0xFE;
char SevenSeg9 = 0xE6;


#ifdef	__cplusplus
}
#endif

#endif	/* VARIABLES_H */

