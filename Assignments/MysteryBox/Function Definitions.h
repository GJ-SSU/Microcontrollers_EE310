/* 
 * File:   Function Definitions.h
 * Author: gjarv
 *
 * Created on April 25, 2024, 9:41 AM
 */
#include <xc.h>
#include ".\Configurations.h"
#include ".\Variables.h"


#ifndef FUNCTION_DEFINITIONS_H
#define	FUNCTION_DEFINITIONS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4
//---------------------
// Defining Functions
//---------------------

void SCS(void); // Secret Code Split
unsigned char SSC(unsigned char InputInQuestion); // Seven Segment Conversion
void DisplayInput(); // Display Input
void PHRES1Inputs(void);
void PHRES2Inputs(void);
void DisplayYourInput(char InputTaken);
void WORLDS_ANGYEST_BUZZER(void);
void INTERRUPT_Initialize (void);

//---------------------
// Functions
//---------------------
void SCS(void){
// Takes our Secret Code and Splits It Into Two Neat Parts
    Secret_Code_Digit_b = Secret_Code & 0x0F;
    Secret_Code_Digit_A = Secret_Code & 0xF0;
    // Correct Secret Code Digit B
    Secret_Code_Digit_A = Secret_Code_Digit_A >> 4;
    return;
}
unsigned char SSC(unsigned char InputInQuestion){
// NumToSevenSegConversion
   // Takes an input and returns their equivalent 7 segment counterpart
    if (InputInQuestion == 0){
        return SevenSeg0;
    }
    if (InputInQuestion == 1){
        return SevenSeg1;
    }
    if (InputInQuestion == 2){
        return SevenSeg2;
    }
    if (InputInQuestion == 3){
        return SevenSeg3;
    }
    if (InputInQuestion == 4){
        return SevenSeg4;
    }
    if (InputInQuestion == 5){
        return SevenSeg5;
    }
    if (InputInQuestion == 6){
        return SevenSeg6;
    }
    if (InputInQuestion == 7){
        return SevenSeg7;
    }
    if (InputInQuestion == 8){
        return SevenSeg8;
    }
    if (InputInQuestion == 9){
        return SevenSeg9;
    }
    else{
        return 0;
    }
}
void PHRES1Inputs(void){
    UserInputA = 0;
    DisplayYourInput(UserInputA);
    _MANDATORY_PRESS_A_DANG_BUTTON:
    if (PORTAbits.RA1 == 1){goto _MANDATORY_PRESS_A_DANG_BUTTON;} // Locking until Something
    Prior_Action = 5;
    while(1){
        Current_Action = PORTAbits.RA1;
        if (Prior_Action == 1){
            if (Current_Action == 0){
                UserInputA = UserInputA + 1;
                DisplayYourInput(UserInputA);
            }
        }
        //Prior_Action = Current_Action;
        if (Prior_Action == Current_Action){
            if (Current_Action == 1){
            Move_On_Timer = Move_On_Timer - 1;
            if (Move_On_Timer == 0){
                Mode_Sel = 1;
                Move_On_Timer = Timer_Length;
                return;
            }
            }
        }
        else{
            Prior_Action = Current_Action;
            Move_On_Timer = Timer_Length;
        }
    }
}
void PHRES2Inputs(void){
    UserInputB = 0;
    DisplayYourInput(UserInputB);
    _MANDATORY_PRESS_A_DANG_BUTTON_BUT_B:
    if (PORTAbits.RA2 == 1){goto _MANDATORY_PRESS_A_DANG_BUTTON_BUT_B;} // Locking until Something
    Prior_Action = 5;
    while(1){
        Current_Action = PORTAbits.RA2;
        if (Prior_Action == 1){
            if (Current_Action == 0){
                UserInputB = UserInputB + 1;
                DisplayYourInput(UserInputB);
            }
        }
        //Prior_Action = Current_Action;
        if (Prior_Action == Current_Action){
            if (Current_Action == 1){
            Move_On_Timer = Move_On_Timer - 1;
            if (Move_On_Timer == 0){
                Mode_Sel = 3;
                Move_On_Timer = Timer_Length;
                DisplayYourInput(17);
                return;
            }
            }
        }
        else{
            Prior_Action = Current_Action;
            Move_On_Timer = Timer_Length;
        }
    }
}
void DisplayYourInput(char InputTaken){
    PORTD = SSC(InputTaken);
}
void WORLDS_ANGYEST_BUZZER(void){
    while(1){
        for(int i=0; i < 20; i++){
        PORTBbits.RB2 = 1;
        __delay_ms(100);
        PORTBbits.RB2 = 0;
        __delay_ms(100);
        }
        for(int i=0; i < 4; i++){
        PORTBbits.RB2 = 1;
        __delay_ms(1000);
        PORTBbits.RB2 = 0;
        __delay_ms(1000);
        }
    }
}
void INTERRUPT_Initialize (void)
{
    // Enable interrupt priority bit in INTCON0 (check INTCON0 register and find the bit)
    INTCON0bits.IPEN = 1;
    // Enable high priority interrupts using bits in INTCON0
    INTCON0bits.GIEH = 1;
    // Enable low priority interrupts using bits in INTCON0
    INTCON0bits.GIEL = 1;
    // Interrupt on falling edge of INT0 pin using bits in INTCON0
    INTCON0bits.INT0EDG = 0;
    // Set the interrupt high priority (IP) for INT0 - INT0IP
    IPR1bits.INT0IP = 1;
    // Enable the interrupt (IE) for INT0
    PIE1bits.INT0IE = 1;
    //Clear interrupt flag for INT0
    PIR1bits.INT0IF = 0;
    //PIR5bits.INT1IF = 0; 
    // Unneeded, Code asked for INT01, but INT01 does not exist. Was typo, but
    // including anyway, "to-be-safe" -- Gabe
    
    // Change IVTBASE by doing the following
    // Set IVTBASEU to 0x00 
    IVTBASEU = 0x00;
    // Set IVTBASEH to  0x40;
    IVTBASEH = 0x40;
    // Set IVTBASEL to 0x08; 
    IVTBASEL = 0x08;
}

#ifdef	__cplusplus
}
#endif

#endif	/* FUNCTION_DEFINITIONS_H */

