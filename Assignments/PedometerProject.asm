;---------------------
; Title: Assignment: Pedometer
;---------------------
;Program Details:
; This project is for creating a basic pedometer. Using a 7 segment display,
; We can count up or count down by pressing and holding 1 of 2 buttons.
; For use as an actual Pedometer, a shaking switch will be installed in place of
; the "Count Up" Button.

; Inputs: PortB 0,1,
; Outputs: PortD (Connected to 7 Segment)
; Date: Mar 25, 2024
; File Dependencies / Libraries: None 
; Compiler: ASM 2.4
; Author: Gabriel Jarvis
; Versions:
;       V1.0: Original Program
; Useful links: 
;       Datasheet: https://ww1.microchip.com/downloads/en/DeviceDoc/PIC18(L)F26-27-45-46-47-55-56-57K42-Data-Sheet-40001919G.pdf 
;       PIC18F Instruction Sets: https://onlinelibrary.wiley.com/doi/pdf/10.1002/9781119448457.app4 
;       List of Instrcutions: http://143.110.227.210/faridfarahmand/sonoma/courses/es310/resources/20140217124422790.pdf 

;---------------------
; Initialization
;---------------------
#include "C:\Users\gjarv\MPLABXProjects\EE310Assignment2_BasisPedometer.X\AssemblyConfig.inc"
#include <xc.inc>

;---------------------
; Program Inputs
;---------------------

 
;---------------------
; Program Constants
;---------------------
Position	    equ	00h
Mode_Sel	    equ 01h
Count_Major	    equ 02h
Count_Minor	    equ 03h
;---------------------
; Definitions
;---------------------  
#define IncButton      PORTB, 0
#define	DecButton      PORTB, 1

;---------------------
; Main Program
;---------------------
    PSECT absdata,abs,ovrld
    
    ORG          20H
    ; Initializing Ports
    CALL	_InitializePortB
    CALL	_InitializePortD
    BANKSEL	0   ; Check later to see if needed
    ; Initializing 7 Segment Display
    CALL	_SetTo0
    
    ; The Main, repeating, program
    _MainProgram:
    CALL	_CheckButtons	; Checks Pressed Buttons
    CALL	_PedometerChange; Updates 7 Segment
    ;CALL	_Delay		; Delays Program
    GOTO	_MainProgram	; Repeat

;---------------------
; Program Memory: Saved Values
;---------------------    
    ORG	    7F0H
    DB	    0xFC	    ; Value of 0
    DB	    0x60	    ; Value of 1
    DB	    0xDA	    ; Value of 2
    DB	    0xF2	    ; Value of 3
    DB	    0x66	    ; Value of 4
    DB	    0xB6	    ; Value of 5
    DB	    0xBE	    ; Value of 6
    DB	    0xE0	    ; Value of 7
    DB	    0xFE	    ; Value of 8
    DB	    0xE6	    ; Value of 9
    DB	    0xEE	    ; Value of 10 (A on 7-Segment)
    DB	    0x3E	    ; Value of 11 (B on 7-Segment)
    DB	    0x9C	    ; Value of 12 (C on 7-Segment)
    DB	    0x7A	    ; Value of 13 (D on 7-Segment)
    DB	    0x9E	    ; Value of 14 (E on 7-Segment)
    DB	    0x8E	    ; Value of 15 (F on 7-Segment)
    
;---------------------
; Functions
;---------------------
    ORG	    1010H
_InitializePortB:
; Sets Port B such that Bit 0 and 1 of it will be our inputs
    ; Initializing PortB
    BANKSEL	PORTB ;
    CLRF	PORTB ;Init PORTB
    BANKSEL	0
    BANKSEL	LATB ;Data Latch
    CLRF	LATB ;
    BANKSEL	ANSELB ;
    CLRF	ANSELB ;digital I/O
    BANKSEL	TRISB ;
    MOVLW	0b0000011 ;Set RB as all outputs except RB0 and RB1
    MOVWF	TRISB
Return

_InitializePortD:
; Sets Port D such that it is all outputs
    ; Initializing PortD
    BANKSEL	PORTD ;
    CLRF	PORTD ;Init PORTD
    BANKSEL	0
    BANKSEL	LATD ;Data Latch
    CLRF	LATD ;
    BANKSEL	ANSELD ;
    CLRF	ANSELD ;digital I/O
    BANKSEL	TRISD ;
    MOVLW	0b0000000 ;Set RD as all Outputs
    MOVWF	TRISD
Return
    
_CheckButtons:
; Checks what Buttons are Pressed by checking what is Triggered
; Then it takes the result, and plops it straight into Mode_Sel
; The result of Mode_Sel will later determine PedometerChange.
    
    ; Is IncButton Triggered? -- Check 1
    BTFSS   IncButton
    GOTO    _FailRoute1   
    ; [Check 1 Pass] Is DecButton Triggered? -- Check 2a
    BTFSS   DecButton
    GOTO    _Mode_Set_1 ; [Check 1P 2aF] IncButton Implimented
    GOTO    _Mode_Set_3 ; [Check 1/2a Pass] Both Buttons are being Pressed!

    _FailRoute1:
    ; [Check 1 Pass] Is Dec Button Triggered? -- Check 2b
    BTFSS   DecButton
    GOTO    _Mode_Set_0 ; [Check 1/2b Fail] Neither Button is being Pressed!
    GOTO    _Mode_Set_2	; [Check 1F 2bP] DecButton Implimented    

; Just barely too many lines to properly skip...
; Sets Mode for Mode_Sel and Returns to Main Program
_Mode_Set_0:
Movlw	00H
Movwf	Mode_Sel
Return
_Mode_Set_1:
Movlw	01H
Movwf	Mode_Sel
Return
_Mode_Set_2:
Movlw	02H
Movwf	Mode_Sel
Return
_Mode_Set_3:
Movlw	03H
Movwf	Mode_Sel
Return

_PedometerChange:
; Checks the value in Mode_Sel and performs the necessary action
; Such actions are:
; If Mode_Sel = 0, Remain, end function
; If Mode_Sel = 1, Increment, end function
; If Mode_Sel = 2, Decrement, end function
; If Model_Sel = 3, Reset to 0, end function
; Additionally:
; If Position at F and Increment is Called, Position moves to 0
; If Position at 0 and Decrement is Called, Position moves to F

    ; Is Mode_Sel = 0?
    Movlw   0H
    CPFSGT  Mode_Sel
Return
    
    ; Is Mode_Sel = 1?
    Movlw   1H
    CPFSEQ  Mode_Sel
    BRA	    _Mode_Sel_2
    ; Is Counter at F?
    Movlw   0FH
    CPFSEQ  Position
    Goto _Increment ; Otherwise
    Goto _SetTo0
    
    _Mode_Sel_2:
    ; Is Mode_Sel = 2?
    Movlw   02H
    CPFSEQ  Mode_Sel
    BRA	    _Mode_Sel_3
    ; Is Counter at 0?
    Movlw   00H
    CPFSEQ  Position
    Goto _Decrement ; Otherwise
    Goto _SetToF
    
    _Mode_Sel_3:
    ; Is Mode_Sel = 3? The answer is likely yes at this point...
    Goto _SetTo0
    
_Increment:
; Advances 7 Segment Display by 1
; Done by incrementing Table Pointer, Reading, and then Sending to PortD
    TBLRD+*
    Movff   TABLAT, WREG
    Movwf   PORTD
    Movlw   01H
    Addwf   Position, 1 ; Check later
Return

_Decrement:
; Decreases 7 Segment Display by 1
; Done by decrementing Table Pointer, Reading, and then Sending to PortD  
    TBLRD*-
    TBLRD*
    Movff   TABLAT, WREG
    Movwf   PORTD
    Movlw   01H
    Subwf   Position, 1 ; Check later    
Return
    
_SetTo0:
; Sets 7 Segment Display to 0
; Done by repositioning the Table Pointer, Reading, and then Sending to PortD
    Movlw   0xF0
    Movwf   TBLPTRL
    Movlw   0x07
    Movwf   TBLPTRH
    Movlw   0x00
    Movwf   TBLPTRU
    TBLRD*
    Movff   TABLAT, WREG
    Movwf   PORTD
    Movlw   00H
    Movwf   Position
Return
    
_SetToF:
; Sets 7 Segment Display to F
; Done by repositioning the Table Pointer, Reading, and then Sending to PortD
    Movlw   0xFF
    Movwf   TBLPTRL
    Movlw   0x07
    Movwf   TBLPTRH
    Movlw   0x00
    Movwf   TBLPTRU
    TBLRD*
    Movff   TABLAT, WREG
    Movwf   PORTD
    Movlw   0FH
    Movwf   Position
Return

_Delay:
; Impliments a delay to the function, such that:
; 1. The display is readable
; 2. The 7 Segment can be properly stopped at a specific value
; Delay is 0.15 Seconds 
    movlw   250
    movwf   Count_Major
    movlw   200
    movwf   Count_Minor
    _InnerLoops:
    Decf    Count_Minor, 1
    BNZ	    _InnerLoops
    Movlw   200
    Movwf   Count_Minor
    DecF    Count_Major
    BNZ	    _InnerLoops
Return
end