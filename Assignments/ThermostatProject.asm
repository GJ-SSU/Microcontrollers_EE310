;---------------------
; Title: Assignment: Thermostat
;---------------------
;Program Details:
; The purpose of this program is to take two predetermined temperatures, 
; one as our measured temp, and one as our desired temp.
; Then depending on our desired temperature (or refTemp) in comparison to our measured temp, 
; we either turn on or off PortD's 1st or 2nd's bits.
; Decimal Versions of our refTemp and measTemp are also stored in registers 0x60-0x62 and 0x70-0x72.

; Inputs: measTemp, refTemp. No physical inputs to board
; Outputs: PortD
; Date: Mar 6, 2024
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
#include "C:\Users\gjarv\MPLABXProjects\EE310Assignment1_Thermostat.X\AssemblyConfig.inc"
#include <xc.inc>

;---------------------
; Program Inputs
;---------------------
refTemp		    equ	20h
measTemp	    equ	21h
 
;---------------------
; Program Constants
;---------------------
ConReg		    equ	22h
ValReg		    equ	23h
refTempCopy	    equ	24h
measTempCopy	    equ	25h
SubAmount	    equ 10 ; decimal
Quo		    equ 27h

refTempDecimalHigh  equ	60h
refTempDecimalMid   equ	61h
refTempDecimalLow   equ	62h
   
measTempDecimalHigh equ	70h
measTempDecimalMid  equ	71h
measTempDecimalLow  equ	72h
;---------------------
; Definitions
;---------------------  
#define HEATER      PORTD,1
#define	COOLER 	    PORTD,2

;---------------------
; Main Program
;---------------------
    PSECT absdata,abs,ovrld
    
    ORG          20H
; Initializing PortD
;    BANKSEL	PORTD ;
;    CLRF	PORTD ;Init PORTA
;    BANKSEL	LATD ;Data Latch
;    CLRF	LATD ;
;    BANKSEL	ANSELD ;
;    CLRF	ANSELD ;digital I/O
;    BANKSEL	TRISD ;
;    MOVLW	0b11111001 ;Set RD[7:4, 0] as inputs
;    MOVWF	TRISD ;and set RD1, RD2, as ouputs
    
; Initializing refTemp and measTemp
    MOVLW   15
    MOVWF   refTemp
    
    MOVLW   -5
    MOVWF   measTemp
    
    ; Check if refTemp and/or measTemp is Negative
    ; Checking if refTemp is Negative
    BTFSC   refTemp, 7
    CALL    _NegationOfrefTemp
    ;Checking if measTemp is Negative
    BTFSC   measTemp, 7
    CALL    _NegationOfmeasTemp
    ;Copying refTemp and measTemp into their copies refTempCopy, measTempCopy
    MOVFF   refTemp, refTempCopy
    MOVFF   measTemp, measTempCopy
    
; Bit Seperation Phase
    ; Seperating refTemp
    CALL    _ClearStatusReg
    MOVLW   SubAmount
    CLRF    Quo, 1
_D_1:
    INCF    Quo, 1
    SUBWF   refTempCopy
    BC	    _D_1
    ADDWF   refTempCopy
    DECF    Quo, 1
    MOVFF   refTempCopy, refTempDecimalLow
    MOVFF   Quo, refTempCopy
    CLRF    Quo
_D_2:
    INCF    Quo, 1
    SUBWF   refTempCopy
    BC	    _D_2
    ADDWF   refTempCopy
    DECF    Quo, 1
    MOVFF   refTempCopy, refTempDecimalMid
    MOVFF   Quo, refTempDecimalHigh
    ; Seperating measTemp
    CALL    _ClearStatusReg
    MOVLW   SubAmount
    CLRF    Quo, 1
_D_1b:
    INCF    Quo, 1
    SUBWF   measTempCopy
    BC	    _D_1b
    ADDWF   measTempCopy
    DECF    Quo, 1
    MOVFF   measTempCopy, measTempDecimalLow
    MOVFF   Quo, measTempCopy
    CLRF    Quo
_D_2b:
    INCF    Quo, 1
    SUBWF   measTempCopy
    BC	    _D_2b
    ADDWF   measTempCopy
    DECF    Quo, 1
    MOVFF   measTempCopy, measTempDecimalMid
    MOVFF   Quo, measTempDecimalHigh


; Comparative Mode
    ; Check Bit 0 of ValReg
    BTFSC   ValReg, 0
    GOTO    _YesRoute
    ; Bit 0 = No
    BTFSC   ValReg, 1
    GOTO    _settingContReg1
    ; Bit 1 = No
    MOVFF   refTemp, WREG
    
    CPFSEQ  measTemp
    GOTO    _GreaterOrLessA
    GOTO    _settingContReg0
    _GreaterOrLessA:
    CPFSLT  measTemp
    GOTO    _settingContReg2
    GOTO    _settingContReg1
    
_YesRoute:    
    BTFSC   ValReg, 1
    GOTO    _settingContReg1
    ; Bit 1 = No
    MOVFF   refTemp, WREG
    CPFSEQ measTemp
    GOTO    _GreaterOrLessB
    GOTO    _settingContReg0
    _GreaterOrLessB:
    CPFSLT  measTemp
    GOTO    _settingContReg1
    GOTO    _settingContReg2

    
; ContReg Pit Stop
_settingContReg0:
    MOVLW   0
    MOVWF   ConReg
    GOTO    _EndGame
_settingContReg1:
    MOVLW   1
    MOVWF   ConReg
    GOTO    _EndGame    
_settingContReg2:
    MOVLW   2
    MOVWF   ConReg
    GOTO    _EndGame
    
; System Activation
_EndGame:
    MOVLW   2
    CPFSLT  ConReg
    GOTO    _ENABLECOOLER
    MOVLW   1
    CPFSLT  ConReg
    GOTO    _ENABLEHEATER
    GOTO    _DISABLEALL

; End of Program
_TheEND:
    GOTO    _TheEND

_NegationOfmeasTemp:
    NEGF    measTemp, 1
    MOVLW   0x01
    ADDWF   ValReg, 1
    RETURN
_NegationOfrefTemp:
    NEGF    refTemp, 1
    MOVLW   0x10
    ADDWF   ValReg, 1
    RETURN
_ClearStatusReg:
    MOVLW   0x00
    MOVWF   STATUS
    RETURN
    
_ENABLEHEATER:
    BCF	COOLER
    BSF	HEATER
    GOTO    _TheEND
_ENABLECOOLER:
    BCF	HEATER
    BSF	COOLER
    GOTO    _TheEND
_DISABLEALL:
    BCF	HEATER
    BCF COOLER
    GOTO    _TheEND
    
END