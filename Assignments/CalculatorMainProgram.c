/*
 * ---------------------
 * Title: Calculator!
 * ---------------------
 * Program Details:
 *  Program is used to implement a simple calculator, which can add, subtract, 
 * multiply, or divide, a two digit number by another to digit number.
 * Inputs: PortB (for Input 1, Input 2)
 * Outputs: Port D (to Output to Seven Segment)
 * Setup: C- Simulator
 * Date: April 8, 2023
 * File Dependencies / Libraries: It is required to include the 
 * Configuration Header File 
 * Compiler: xc8, 2.46
 * Author: Gabriel Jarvis
 * Versions:
 *      V1.0: Original
 *      V1.1: Correction of Zero Answer Glitch
 * Useful links:  
 *      Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
 *      PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4 
 *      List of Instructions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf 
 */

#include <xc.h>
//#include "../../../../../Program Files/Microchip/xc8/v2.40/pic/include/proc/pic18f46k42.h"
#include "C:/Program Files/Microchip/xc8/v2.46/pic/include/proc/pic18f47k42.h"
//#include "C:\Program Files\Microchip\xc8\v2.40\pic\include\proc\pic18f46k42"
#include ".\CalcHeader.h"

#define  _XTAL_FREQ  4000000
# define FCY        XTAL_FREQ/4

//---------------------
// Initialization
//---------------------
#include <xc.h>
//#include "../../../../../Program Files/Microchip/xc8/v2.40/pic/include/proc/pic18f46k42.h"
#include "C:/Program Files/Microchip/xc8/v2.46/pic/include/proc/pic18f47k42.h"
//#include "C:\Program Files\Microchip\xc8\v2.40\pic\include\proc\pic18f46k42"
#include ".\CalcHeader.h"

#define  _XTAL_FREQ  4000000
# define FCY        XTAL_FREQ/4


//---------------------
// Program Inputs
//---------------------
// Inputs to the Calculator, in Numerical Terms
   // Split Inputs
int Input1A = 0; // Input 1, Most Significant Bit
int Input1B = 0; // Input 1, Least Significant Bit
int Input2A = 0; // Input 2, Most Significant Bit
int Input2B = 0; // Input 2, Least Significant Bit
   // Combined Inputs: Used for Calculations
int CombInput1 = 0; 
int CombInput2 = 0;

// Outputs from the Calculation Stage
   // Combined Outputs: Natural Output 
int CombOutput = 0;
   // Split Outputs
int OutputDigitA = 0;
int OutputDigitB = 0;
int OutputDigitC = 0;
int OutputDigitD = 0;
 
//---------------------
// Program Constants
//---------------------
int MemInput1A = 0;
int MemInput1B = 0;
int MemInput2A = 0;
int MemInput2B = 0;

int Operation_REG = 0;
char InputPos = 11;
char NumDisp = 11;
int BUTTONPRESSDELAY = 0;
int ButtonOrOperation = 0;
int InputAnswer = 0;
#define DelayLength 3 //5000
unsigned char DigitToDisplay = 0;
char NegativeTrigger = 0;

int keypad[4] [4] = {1,2,3,10,
                     4,5,6,11,
                     7,8,9,12,
                     15,0,14,13};

// Static Values for Seven Segment
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
//---------------------
// Defining Functions
//---------------------
int CheckButtons();
// Check Buttons
  // Checks What Button is being pressed, and Outputs Selected Button
/**/
//////////////////// Input1 Read Functions ////////////////////
void InputRead1ATA();
// InputRead1A_TypeA
  // Occurs when InputPos is 11. This happens ONCE per equation.
  // This determines the first input's Highest Order Digit.
  // Calls Check Buttons.
  // If Operation Button was Pressed, keep InputPos at 11 and continue on.
  // If NOT an Operation Button was Pressed, 
      // set InputPos to 12, update Input1A and MemInput1A, and continue on.
  // IF NO BUTTON IS PRESSED, DO NOTHING AND CONTINUE.
void InputRead1B();
// InputRead1B
  // Occurs when InputPos is 12
  // This determines the first input's Lowest Order Digit.
  // Calls Check Buttons.
  // If Operation Button was Pressed, 
      // Move InputPos to 21, NumDisp to 21, Operation to selected and continue.
  // If NOT an Operation Button was Pressed, 
      // set InputPos to 13, update Input1B and MemInput1B, and continue on.
  // IF NO BUTTON IS PRESSED, DO NOTHING AND CONTINUE.
void InputRead1ATB();
// InputRead1A_TypeB
  // Occurs when InputPos is 13. This is the REPEATABLE Version.
  // Like TypeA, but Operation Buttons have an effect now.
  // Calls Check Buttons.
  // If Operation Button was Pressed, keep InputPos at 11 and continue on.
  // If NOT an Operation Button was Pressed, 
      // set InputPos to 12, update Input1A and MemInput1A, and continue on.
  // IF NO BUTTON IS PRESSED, DO NOTHING AND CONTINUE.
/**/
//////////////////// Input 2 Read Functions ////////////////////
void InputRead2ATA();
// InputRead2A_TypeA
  // Occurs when InputPos is 21. This happens ONCE per equation.
  // This determines the second input's Highest Order Digit.
  // Calls Check Buttons.
  // If Operation Button was Pressed, keep InputPos at 21 and continue on.
  // If NOT an Operation Button was Pressed, 
      // set InputPos to 22, update Input2A and MemInput2A, and continue on.
  // IF NO BUTTON IS PRESSED, DO NOTHING AND CONTINUE
void InputRead2B();
// InputRead2B
  // Occurs when InputPos is 22
  // This determines the second input's Lowest Order Digit.
  // Calls Check Buttons.
  // If Operation Button other than pound was Pressed, 
      // Keep InputPos at 22 and continue on
  // If Operation Button POUND specifically is Pressed,
      // set InputPos to 30, DispPos to 31, 
  // If NOT an Operation Button was Pressed, 
      // set InputPos to 23, update Input2B and MemInput2B, and continue on.
  // IF NO BUTTON IS PRESSED, DO NOTHING AND CONTINUE
void InputRead2ATB();
// InputRead2A_TypeB
  // Occurs when InputPos is 23. This is REPEATABLE.
  // Operation Pound now has an effect.
  // Calls Check Buttons.
  // If Operation Button (Other than Pound) was Pressed, 
      // keep InputPos at 23 and continue on.
  // If Operation Pound is Pressed,
      // set InputPos to 30, DispPos to 31,
  // If NOT an Operation Button was Pressed, 
      // set InputPos to 22, update Input2A and MemInput2A, and continue on.
  // IF NO BUTTON IS PRESSED, DO NOTHING AND CONTINUE
/**/
//////////////////// END OF EQUATION and RESET Functions ////////////////////
void ResultDisplayModeInit();
// Result_Display_Mode_Initial
  // Occurs at InputPos = 30
  // Resets Input1A, Input1B, Input2A, Input2B, MemInput1A, MemInput1B, 
     // MemInput2A, MemInput2B, CombInput1, and CombInput2.
  // After which, sets InputPos = 31.
void ResultDisplayModeRepeat();
// Result_Display_Mode_Repeatable
  // Occurs at InputPos = 31
  // Will essentially keep the Output screen going. When exiting this mode,
      // we are starting a new equation.
  // Check Buttons
  // If NO Input, Keep InputPos 31, and continue on.
  // If ANY Button is Pressed, InputPos = 0, and continue on.
void StartingAnew();
// Starting_Anew
  // Occurs at InputPos = 0
  // Resets CombOutput, OutputDigitA, OutputDigitB, OutputDigitC, and OutputDigitD
  // Negative Trigger is also reset here.
  // Sets InputPos = 11, NumDisp = 11, and continues on.
/**/
//////////////////// ARITHMATIC FUNCTIONS ////////////////////
void Fadd();
// FAdd
  // Adds both inputs together, and Outputs result to CombOutput
  // Resets Inputs and Mem Inputs in the process
void FSubtract();
// FSubtract
  // Subtracts Input1 by Input2 together, and Outputs result to CombOutput
  // Resets Inputs and Mem Inputs in the process
void FMultiply();
// FMultiply
  // Multiplies both inputs together, and Outputs result to CombOutput
  // Resets Inputs and Mem Inputs in the process
void FDivide();
// FDivide
  // Divides Input1 by Input2, and Outputs result to CombOutput. No remainder
  // Resets Inputs and Mem Inputs in the process
/**/
//////////////////// NUMBER TRANSFORMATION FUNCTIONS ////////////////////
int InputComb(int InputA, int InputB);
// Input Combiner
    // Combines 2 Inputs, and merges them to a single register.
void NumSplit();
// Number Split
  // Splits CombOutput into OutputDigitA, OutputDigitB, OutputDigitC, OutputDigitD 
void NumCorrect();
// Number Correction
  // Takes 2 Levels, Updates them
  // Converts a Number from Hex to Decimal of the lower level.
  // If the Hex Number is Greater than or Equal to 10, we increment upper level. 
  // By Nature, Code should convert Highest Level to Lowest Level bits
void MathComb();
// Math Combiner
    // Combines Inputs using their Retrospective Operation 
int NumToSevenSegConversion(int InputInQuestion);
// NumToSevenSegConversion
   // Takes an input and returns their equivalent 7 segment counterpart
void DisplayNumber();
// Display Number
    // Checks NumDisp, and determines mode
    // If NumDisp = 11 or 12, Displays the first or second digit of first input.
    // If NumDisp = 21 or 22, Displays the first or second digit of second input
    // If NumDisp = 31,32,33,34, Displays the __ digit of the output
    // If NumDisp = 1_, flip to the other one.
    // If NumDisp = 2_, flip to the other one.
    // If NumDisp = 3_, follow order: 1->2->3->4->1->...
/**/

//---------------------
// Main Program
//---------------------
void main(){
// Initialize Ports
    //TRISD = 0;
    //TRISE = 0;
    //TRISB = 0xF0;
    //PORTB = 0xF0;
    asm("BANKSEL    PORTB");
    asm("CLRF       PORTB");
    asm("BANKSEL    LATB");
    asm("CLRF       LATB");
    asm("BANKSEL    ANSELB");
    asm("CLRF       ANSELB");
    asm("BANKSEL    TRISB");
    asm("MOVLW  0b11110000");
    asm("MOVWF TRISB");
    
    asm("BANKSEL    PORTD");
    asm("CLRF       PORTD");
    asm("BANKSEL    LATD");
    asm("CLRF       LATD");
    asm("BANKSEL    ANSELD");
    asm("CLRF       ANSELD");
    asm("BANKSEL    TRISD");
    asm("MOVLW      0x00");
    asm("MOVWF      TRISD");
    
    asm("BANKSEL    PORTA");
    asm("CLRF       PORTA");
    asm("BANKSEL    LATA");
    asm("CLRF       LATA");
    asm("BANKSEL    ANSELA");
    asm("CLRF       ANSELA");
    asm("BANKSEL    TRISA");
    asm("MOVLW      0x00");
    asm("MOVWF      TRISA");
    
    WPUB = 0xF0;
    
    while(1){ // To create INFINITE LOOP
        // Input Phase
        // Are We allowing New Inputs?
        if (BUTTONPRESSDELAY == 0){
            // If Yes, it is now time to find what exactly is occurring/changing
            if (InputPos == 11){
                InputRead1ATA();
                goto _Post_InputPhase;
            }
            if (InputPos == 12){
                InputRead1B();
                goto _Post_InputPhase;
            }
            if (InputPos == 13){
                InputRead1ATB();
                goto _Post_InputPhase;
            }
            if (InputPos == 21){
                InputRead2ATA();
                goto _Post_InputPhase;
            }
            if (InputPos == 22){
                InputRead2B();
                goto _Post_InputPhase;
            }
            if (InputPos == 23){
                InputRead2ATB();
                goto _Post_InputPhase;
            }
            // These are for Outputs and Reseting Back into Initial State
            if (InputPos == 30){
                ResultDisplayModeInit();
            }
            if (InputPos == 31){
                ResultDisplayModeRepeat();
                goto _Post_InputPhase;
            }
            if (InputPos == 0){
                StartingAnew();
            }
        }
        _Post_InputPhase:
        // Decrement Phase
        if (BUTTONPRESSDELAY != 0){ // If there is a Delay to Decrement, do it
            BUTTONPRESSDELAY = BUTTONPRESSDELAY - 1;
        }
        // Display Phase
        DisplayNumber();
    }
}

//---------------------
// Functions
//---------------------
int CheckButtons(){
// Check Buttons
  // Checks What Button is being pressed, and Outputs Selected Button
    unsigned char temp, Col=0, Row=4;
    temp = PORTB;
    temp ^= 0xF0;
    if(!temp) return -1;
    while(temp<<=1) Col++;
    PORTB = 0xFE;
    if(PORTB != 0xFE){
        Row = 0;
    }
    else{
        PORTB = 0xFD;
        if(PORTB != 0xFD){
            Row = 1;
        }
        else{
            PORTB = 0xFB;
            if(PORTB != 0xFB){
                Row = 2;
            }
            else{
                PORTB = 0xF7;
                if(PORTB != 0xF7){
                    Row = 3;
                }
            }
        }
    }
    PORTB = 0xF0;
    while (PORTB != 0xF0) PORTB = 0xF0; 
    if (Row<4){
    return keypad[Row][Col];
    }
    else{
        return -1;
    }
}
/**/
//////////////////// Input1 Read Functions ////////////////////
void InputRead1ATA(){
// InputRead1A_TypeA
  // Occurs when InputPos is 11. This happens ONCE per equation.
  // This determines the first input's Highest Order Digit.
  // Calls Check Buttons.
  // If Operation Button was Pressed, keep InputPos at 11 and continue on.
  // If NOT an Operation Button was Pressed, 
      // set InputPos to 12, update Input1A and MemInput1A, and continue on.
  // IF NO BUTTON IS PRESSED, DO NOTHING AND CONTINUE.
    ButtonOrOperation = CheckButtons(); 
    if (ButtonOrOperation == -1){
        return;
    }
    if (ButtonOrOperation >= 10){
        return;
    }
    if (ButtonOrOperation < 10){
        InputPos = 12;
        Input1A = ButtonOrOperation;
        MemInput1A = Input1A;
        BUTTONPRESSDELAY = DelayLength;
        return;
    }
}
void InputRead1B(){
// InputRead1B
  // Occurs when InputPos is 12
  // This determines the first input's Lowest Order Digit.
  // Calls Check Buttons.
  // If Operation Button was Pressed, 
      // Move InputPos to 21, NumDisp to 21, Operation to selected and continue.
  // If NOT an Operation Button was Pressed, 
      // set InputPos to 13, update Input1B and MemInput1B, and continue on.
  // IF NO BUTTON IS PRESSED, DO NOTHING AND CONTINUE.
    ButtonOrOperation = CheckButtons(); 
    if (ButtonOrOperation == -1){
        return;
    }
    if (ButtonOrOperation >= 10){
        CombInput1 = InputComb(Input1A, Input1B);
        Operation_REG = ButtonOrOperation;
        InputPos = 21;
        NumDisp = 21;
        BUTTONPRESSDELAY = DelayLength;
        return;
    }
    if (ButtonOrOperation < 10){
        InputPos = 13;
        Input1B = ButtonOrOperation;
        MemInput1B = Input1B;
        BUTTONPRESSDELAY = DelayLength;
        return;
    }
}
void InputRead1ATB(){
// InputRead1A_TypeB
  // Occurs when InputPos is 13. This is the REPEATABLE Version.
  // Like TypeA, but Operation Buttons have an effect now.
  // Calls Check Buttons.
  // If Operation Button was Pressed, keep InputPos at 11 and continue on.
  // If NOT an Operation Button was Pressed, 
      // set InputPos to 12, update Input1A and MemInput1A, and continue on.
  // IF NO BUTTON IS PRESSED, DO NOTHING AND CONTINUE.
    ButtonOrOperation = CheckButtons(); 
    if (ButtonOrOperation == -1){
        return;
    }
    if (ButtonOrOperation >= 10){
        CombInput1 = InputComb(Input1A, Input1B);
        Operation_REG = ButtonOrOperation;
        InputPos = 21;
        NumDisp = 21;
        BUTTONPRESSDELAY = DelayLength;
        return;
    }
    if (ButtonOrOperation < 10){
        InputPos = 12;
        Input1A = ButtonOrOperation;
        MemInput1A = Input1A;
        BUTTONPRESSDELAY = DelayLength;
        return;
    }
}
/**/

//////////////////// Input 2 Read Functions ////////////////////
void InputRead2ATA(){
// InputRead2A_TypeA
  // Occurs when InputPos is 21. This happens ONCE per equation.
  // This determines the second input's Highest Order Digit.
  // Calls Check Buttons.
  // If Operation Button was Pressed, keep InputPos at 21 and continue on.
  // If NOT an Operation Button was Pressed, 
      // set InputPos to 22, update Input2A and MemInput2A, and continue on.
  // IF NO BUTTON IS PRESSED, DO NOTHING AND CONTINUE
    ButtonOrOperation = CheckButtons();
    if (ButtonOrOperation == -1){
        return;
    }
    if (ButtonOrOperation >= 10){
        return;
    }
    if (ButtonOrOperation < 10){
        InputPos = 22;
        Input2A = ButtonOrOperation;
        MemInput1A = Input2A;
        BUTTONPRESSDELAY = DelayLength;
        return;
    }
}
void InputRead2B(){
// InputRead2B
  // Occurs when InputPos is 22
  // This determines the second input's Lowest Order Digit.
  // Calls Check Buttons.
  // If Operation Button other than pound was Pressed, 
      // Keep InputPos at 22 and continue on
  // If Operation Button POUND specifically is Pressed,
      // set InputPos to 30, DispPos to 31, 
  // If NOT an Operation Button was Pressed, 
      // set InputPos to 23, update Input2B and MemInput2B, and continue on.
  // IF NO BUTTON IS PRESSED, DO NOTHING AND CONTINUE
    ButtonOrOperation = CheckButtons(); 
    if (ButtonOrOperation == -1){
        return;
    }
    if (ButtonOrOperation >= 10){
    if (ButtonOrOperation == 14){ // If Pound is Pressed
        CombInput2 = InputComb(Input2A, Input2B);
        MathComb();
        NumSplit();
        InputPos = 30;
        NumDisp = 31;
        BUTTONPRESSDELAY = DelayLength;
        return;
    }
    else{
        return;
    }
    }
    if (ButtonOrOperation < 10){
        InputPos = 23;
        Input2B = ButtonOrOperation;
        MemInput2B = Input2B;
        BUTTONPRESSDELAY = DelayLength;
        return;
    }
}
void InputRead2ATB(){
// InputRead2A_TypeB
  // Occurs when InputPos is 23. This is REPEATABLE.
  // Operation Pound now has an effect.
  // Calls Check Buttons.
  // If Operation Button (Other than Pound) was Pressed, 
      // keep InputPos at 23 and continue on.
  // If Operation Pound is Pressed,
      // set InputPos to 30, DispPos to 31,
  // If NOT an Operation Button was Pressed, 
      // set InputPos to 22, update Input2A and MemInput2A, and continue on.
  // IF NO BUTTON IS PRESSED, DO NOTHING AND CONTINUE
    ButtonOrOperation = CheckButtons(); 
    if (ButtonOrOperation == -1){
        return;
    }
    if (ButtonOrOperation >= 10){
    if (ButtonOrOperation == 14){ // If Pound is Pressed
        CombInput2 = InputComb(Input2A, Input2B);
        MathComb();
        NumSplit();
        InputPos = 30;
        NumDisp = 31;
        BUTTONPRESSDELAY = DelayLength;
        return;
    }
    else{
        return;
    }
    }
    if (ButtonOrOperation < 10){
        InputPos = 22;
        Input2A = ButtonOrOperation;
        MemInput2A = Input2A;
        BUTTONPRESSDELAY = DelayLength;
        return;
    }
}
/**/
//////////////////// END OF EQUATION and RESET Functions ////////////////////
void ResultDisplayModeInit(){
// Result_Display_Mode_Initial
  // Occurs at InputPos = 30
  // Resets Input1A, Input1B, Input2A, Input2B, MemInput1A, MemInput1B, 
  // MemInput2A, MemInput2B, CombInput1, and CombInput2.
  // After which, sets InputPos = 31.
    Input1A = 0;
    Input1B = 0;
    Input2A = 0;
    Input2B = 0;
    //MemInput1A = 0;
    //MemInput1B = 0;
    //MemInput2A = 0;
    //MemInput2B = 0;
    CombInput1 = 0;
    CombInput2 = 0;
    InputPos = 31;
    return;
}
void ResultDisplayModeRepeat(){
// Result_Display_Mode_Repeatable
  // Occurs at InputPos = 31
  // Will essentially keep the Output screen going. When exiting this mode,
      // we are starting a new equation.
  // Check Buttons
  // If NO Input, Keep InputPos 31, and continue on.
  // If ANY Button is Pressed, InputPos = 0, and continue on.
    ButtonOrOperation = CheckButtons();
    if (ButtonOrOperation == -1){
        return;
    }
    else{
        InputPos = 0;
        BUTTONPRESSDELAY = DelayLength;
        return;
    }       
}
void StartingAnew(){
// Starting_Anew
  // Occurs at InputPos = 0
  // Resets CombOutput, OutputDigitA, OutputDigitB, OutputDigitC, and OutputDigitD
  // Negative Trigger is also reset here.
  // Sets InputPos = 11, NumDisp = 11, and continues on.
    CombOutput = 0;
    OutputDigitA = 0;
    OutputDigitB = 0;
    OutputDigitC = 0;
    OutputDigitD = 0;
    NegativeTrigger = 0;
    InputPos = 11;
    NumDisp = 11;
    return;
}
/**/
//////////////////// ARITHMATIC FUNCTIONS ////////////////////
void Fadd(){
// FAdd
  // Adds both inputs together, and Outputs result to CombOutput
  // Resets Inputs and Mem Inputs in the process
    CombOutput = CombInput1 + CombInput2;
    return;
}
void FSubtract(){
// FSubtract
  // Subtracts Input1 by Input2 together, and Outputs result to CombOutput
  // Resets Inputs and Mem Inputs in the process
    if (CombInput1 >= CombInput2){
        CombOutput = CombInput1 - CombInput2;
    return;
    }
    else{
        CombOutput = CombInput2 - CombInput1;
        NegativeTrigger = 1;
    return;
    }
}
void FMultiply(){
// FMultiply
  // Multiplies both inputs together, and Outputs result to CombOutput
  // Resets Inputs and Mem Inputs in the process
    CombOutput = CombInput1 * CombInput2;
    return;
}
void FDivide(){
// FDivide
  // Divides Input1 by Input2, and Outputs result to CombOutput. No remainder
  // Resets Inputs and Mem Inputs in the process
    CombOutput = CombInput1 / CombInput2;
    return;
}
/**/
//////////////////// NUMBER TRANSFORMATION FUNCTIONS ////////////////////
int InputComb(int InputA, int InputB){
// Input Combiner
    // Combines 2 Inputs, and merges them to a single register.
    int InputAnswer = 10*InputA + InputB;
return InputAnswer;
}
void NumSplit(){
// Number Split
  // Splits CombOutput into OutputDigitA, OutputDigitB, OutputDigitC, OutputDigitD
    
    //OutputDigitD = CombOutput & 0x0F;
    //OutputDigitC = (CombOutput>>4) & 0x0F;
    //OutputDigitB = (CombOutput>>8) & 0x0F;
    //OutputDigitA = (CombOutput>>16) & 0x0F;
    
    if (CombOutput != 0){
    _SplittingWorld:
    CombOutput = CombOutput - 1;
    OutputDigitD = OutputDigitD + 1;
    if (OutputDigitD == 10){
        OutputDigitD = 0;
        OutputDigitC = OutputDigitC + 1;
        if (OutputDigitC == 10){
            OutputDigitC = 0;
            OutputDigitB = OutputDigitB+1;
            if (OutputDigitB == 10){
                OutputDigitB = 0;
                OutputDigitA = OutputDigitA + 1;
            }
        }
    }
    if (CombOutput != 0){
        goto _SplittingWorld;
    }
    }
    else{
        OutputDigitA = 0;
        OutputDigitB = 0;
        OutputDigitC = 0;
        OutputDigitD = 0;
    }
    /*
    while((CombOutput % 0xF000) != 0){
        CombOutput = CombOutput - 0x1000;
        OutputDigitA = OutputDigitA + 1;
    }
    while((CombOutput % 0x0F00) != 0){
        CombOutput = CombOutput - 0x0100;
        OutputDigitB = OutputDigitB + 1;
    }
    while((CombOutput % 0x00F0) != 0){
        CombOutput = CombOutput - 0x0010;
        OutputDigitC = OutputDigitC + 1;
    }
    while((CombOutput % 0x000F) != 0){
        CombOutput = CombOutput - 0x0001;
        OutputDigitD = OutputDigitD + 1;
     }
     */
    
    //OutputDigitD = CombOutput & 0x000F;
    //CombOutput >> 4;
    //OutputDigitC = CombOutput & 0x000F; //0x00F0;
    //CombOutput >> 4;
    //OutputDigitB = CombOutput & 0x000F; //0x0F00;
    //CombOutput >> 4;
    //OutputDigitA = CombOutput & 0x000F; //0xF000;
    //NumCorrect();
    return;
}
void NumCorrect(){
// Number Correction
  // Takes 2 Levels, Updates them
  // Converts a Number from Hex to Decimal of the lower level.
  // If the Hex Number is Greater than or Equal to 10, we increment upper level. 
  // By Nature, Code should convert Highest Level to Lowest Level bits
    _OutputBFix:
    if (OutputDigitB >= 10){
        OutputDigitB = OutputDigitB-10;
        OutputDigitA = OutputDigitA + 1;
    }
    _OutputCFix:
    if (OutputDigitC >= 10){
        OutputDigitC = OutputDigitC-10;
        OutputDigitB = OutputDigitB + 1;
        goto _OutputBFix;
    }
    if (OutputDigitD >= 10){
        OutputDigitD = OutputDigitD-10;
        OutputDigitC = OutputDigitC + 1;
        goto _OutputCFix;
    }
    return;
}
void MathComb(){
// Math Combiner
    // Combines Inputs using their Retrospective Operation
    if (Operation_REG == 10){
        Fadd();
        return;
    }
    if (Operation_REG == 11){
        FSubtract();
        return;
    }
    if (Operation_REG == 12){
        FMultiply();
        return;
    }
    if (Operation_REG == 13){
        FDivide();
        return;
    }
}
int NumToSevenSegConversion(int InputInQuestion){
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
}
void DisplayNumber(){
// Display Number
    // Checks NumDisp, and determines mode
    // If NumDisp = 11 or 12, Displays the first or second digit of first input.
    // If NumDisp = 21 or 22, Displays the first or second digit of second input
    // If NumDisp = 31,32,33,34, Displays the __ digit of the output
    // If NumDisp = 1_, flip to the other one.
    // If NumDisp = 2_, flip to the other one.
    // If NumDisp = 3_, follow order: 1->2->3->4->1->...
    if (NumDisp == 11){
        DigitToDisplay = NumToSevenSegConversion(Input1A);
        PORTD = DigitToDisplay; // To Seven Segment
        PORTA = 0x02; // To Selector
        NumDisp = 12;
        return;
    }
    if (NumDisp == 12){
        DigitToDisplay = NumToSevenSegConversion(Input1B);
        PORTD = DigitToDisplay; // To Seven Segment
        PORTA = 0x01; // To Selector
        NumDisp = 11;
        return;
    }
    if (NumDisp == 21){
        DigitToDisplay = NumToSevenSegConversion(Input2A);
        PORTD = DigitToDisplay; // To Seven Segment
        PORTA = 0x02; // To Selector
        NumDisp = 22;
        return;
    }
    if (NumDisp == 22){
        DigitToDisplay = NumToSevenSegConversion(Input2B);
        PORTD = DigitToDisplay; // To Seven Segment
        PORTA = 0x01; // To Selector
        NumDisp = 21;
        return;
    }
    if (NumDisp == 31){
        DigitToDisplay = NumToSevenSegConversion(OutputDigitA);
        PORTD = DigitToDisplay; // To Seven Segment
        PORTA = 0x08; // To Selector
        NumDisp = 32;
        if (NegativeTrigger == 1){
            PORTD = PORTD + 1;
        }
        return;
    }
    if (NumDisp == 32){
        DigitToDisplay = NumToSevenSegConversion(OutputDigitB);
        PORTD = DigitToDisplay; // To Seven Segment
        PORTA = 0x04; // To Selector
        NumDisp = 33;
        return;
    }
    if (NumDisp == 33){
        DigitToDisplay = NumToSevenSegConversion(OutputDigitC);
        PORTD = DigitToDisplay; // To Seven Segment
        PORTA = 0x02; // To Selector
        NumDisp = 34;
        return;
    }
    if (NumDisp == 34){
        DigitToDisplay = NumToSevenSegConversion(OutputDigitD);
        PORTD = DigitToDisplay; // To Seven Segment
        PORTA = 0x01; // To Selector
        NumDisp = 31;
        return;
    }
}
/**/
