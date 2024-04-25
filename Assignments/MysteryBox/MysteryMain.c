/*
 * ---------------------
 * Title: What's in the Box?
 * ---------------------
 * Program Details:
 *  This is essentially an advanced safe. There is a code that must be matched
 * by the two Photo Resistors. If code matches, open box. If code does not match,
 * enable buzzer. There is a reset switch that is part of an interrupt.
 * Inputs: PortA (Bit 0 for Working Led [Output], Bit 1 and 2 for Photo Resistor)
 * Outputs: PortB (Bit 0 Interrupt [Input], Bit 1 Relay, Bit 2 Buzzer), 
 * Port D (to Output to Seven Segment)
 * Setup: C- Simulator
 * Date: April 21, 2023
 * File Dependencies / Libraries: It is required to include the 
 * Configuration Header File 
 * Compiler: xc8, 2.46
 * Author: Gabriel Jarvis
 * Versions:
 *      V1.0: Original
 * Useful links:  
 *      Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
 *      PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4 
 *      List of Instructions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf 
 */


#include <xc.h>
#include ".\Function Definitions.h"
#include ".\Configurations.h"
#include ".\Variables.h"









//---------------------
// Interrupt Service Routine
//---------------------

void __interrupt(irq(IRQ_INT0),base(0x4008)) INT0_ISR(void)
{
        // Check if interrupt flag for INT0 is set to 1 - (note INT0 is your input)
            // if so, reset the ENTIRE Program, and play a Melody
        // always clear the interrupt flag for INT0 when done 
    if (PIR1bits.INT0IF == 1){
        for(int i=0; i < 20; i++){
        PORTBbits.RB2 = 1;
        __delay_ms(1000);
        PORTBbits.RB2 = 0;
        __delay_ms(1000);
            
        }
        }
        //PIR1bits.INT0IF = 0;
        //PORTDbits.RD0 = 0;
        //PORTBbits.RB0 = 1;
    }


//---------------------
// Main Program
//---------------------
void main(void) {
    //Initialize Ports, and Display
    asm("BANKSEL    PORTA");
    asm("CLRF       PORTA");
    asm("BANKSEL    LATA");
    asm("CLRF       LATA");
    asm("BANKSEL    ANSELA");
    asm("CLRF       ANSELA");
    asm("BANKSEL    TRISA");
    asm("MOVLW      0b00000110");
    asm("MOVWF      TRISA");
    
    asm("BANKSEL    PORTB");
    asm("CLRF       PORTB");
    asm("BANKSEL    LATB");
    asm("CLRF       LATB");
    asm("BANKSEL    ANSELB");
    asm("CLRF       ANSELB");
    asm("BANKSEL    TRISB");
    asm("MOVLW      0b00000001");
    asm("MOVWF      TRISB");
    
    asm("BANKSEL    PORTD");
    asm("CLRF       PORTD");
    asm("BANKSEL    LATD");
    asm("CLRF       LATD");
    asm("BANKSEL    ANSELD");
    asm("CLRF       ANSELD");
    asm("BANKSEL    TRISD");
    asm("MOVLW      0x00");
    asm("MOVWF      TRISD");
    
    INTERRUPT_Initialize();
    // Split Secret Code
    SCS();
    // Turn on Active Light (PortA, Bit 0))
    PORTAbits.RA0 = 1;
    while(1){
    if (Mode_Sel == 0){ // Highest Order Ph.Res. Check
        PHRES1Inputs();
    }    
    if (Mode_Sel == 1){ // Lowest Order Ph.Res. Check
        PHRES2Inputs();
    }
    if (Mode_Sel == 3){ // We done inputing our code. Time to see if it matches
        if (Secret_Code_Digit_A == UserInputA){
            if (Secret_Code_Digit_B == UserInputB){
                //If reached this means it works! Activate Motors!
                PORTBbits.RB1 = 1; // Open Box!
                __delay_ms(10000);
                PORTBbits.RB1 = 0; // The Box is Open, Stop Opening it More
                Mode_Sel = 5;
            }
            else{ 
                Mode_Sel = 4;
        }
        }
        else{
            Mode_Sel = 4;
        }   
    }
    if (Mode_Sel == 4){ // ANGY MODE
        WORLDS_ANGYEST_BUZZER();
    } 
    if (Mode_Sel == 5){ // Victory Mode! Hold here forever!
    _VICTORYYYY:
    goto _VICTORYYYY;
    }
    }
    return;
}

