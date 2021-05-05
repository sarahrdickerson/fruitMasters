; Print.s
; Student names: change this to your names or look very silly
; Last modification date: change this to the last modification date or look very silly
; Runs on TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; SSD1306_OutChar   outputs a single 8-bit ASCII character
; SSD1306_OutString outputs a null-terminated string 

    IMPORT   SSD1306_OutChar
    IMPORT   SSD1306_OutString
    PRESERVE8
    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB



;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
n EQU 0 ; input parameter

LCD_OutDec PROC
    EXPORT   LCD_OutDec
;put your Lab 7 solution here
	PUSH{R0, LR}
	
	;base case 
	CMP R0, #9
	BLS base
	
	;divide n by ten
	MOV R2, #10
	UDIV R0, R2
	
	;call function with new parameter
	BL LCD_OutDec
	
	;find modulo of n and output it to display
	MOV R2, #10
	LDR R1, [SP, #n] 
	MOV R3, R1  
	UDIV R1, R2
	MUL R1, R2 
	SUB R0, R3, R1

	ADD R0, #0x30
	BL SSD1306_OutChar
	B done
		
base
	ADD R0, #0x30
	BL SSD1306_OutChar
	
	;return to previous function call
done
	ADD SP, #4
	POP{PC}
    ENDP
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.01, range 0.00 to 9.99
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.00 "
;       R0=3,    then output "0.03 "
;       R0=89,   then output "0.89 "
;       R0=123,  then output "1.23 "
;       R0=999,  then output "9.99 "
;       R0>999,  then output "*.** "
; Invariables: This function must not permanently modify registers R4 to R11
m EQU 0
count EQU 4 
FP RN 11
LCD_OutFix PROC
         EXPORT   LCD_OutFix
	PUSH{R11, LR}
	SUB SP, #8
	MOV R11, SP
	STR R0, [R11, #m]
	MOV R1, #0
	STR R1, [R11, #count]
	MOV R2, #10

	

	MOV R1, R0
	CMP R1, #1000
	BLO inRange
	;if not in range, output *.**
	MOV R0, #0x2A
	BL SSD1306_OutChar
	MOV R0, #0x2E
	BL SSD1306_OutChar
	MOV R0, #0x2A
	BL SSD1306_OutChar
	MOV R0, #0x2A
	BL SSD1306_OutChar
	B doneF
	
	;if in range, isolate each digit of the number and push onto the stack (up to 3 digits)
inRange	
	LDR R0, [R11, #m]
	MOV R1, R0
	
	UDIV R0, R2
	STR R0, [R11, #m]
	MUL R0, R2
	
	SUB R0, R1, R0
	PUSH{R0, R5}
	
	LDR R3, [R11, #count]
	ADD R3, #1
	STR R3, [R11, #count]
	CMP R3, #3
	BLO inRange
	
	;pop each digit and output to the OLED (with a decimal point after first digit)
	POP{R0, R5}
	ADD R0, #0x30
	BL SSD1306_OutChar
	
	MOV R0, #0x2E
	BL SSD1306_OutChar
	
	POP{R0, R5}
	ADD R0, #0x30
	BL SSD1306_OutChar
	
	POP{R0, R5}
	ADD R0, #0x30
	BL SSD1306_OutChar
	
	;deallocation
doneF
	ADD SP, #8
	POP{R11, LR}
    BX   LR
    ENDP
    ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN          ; make sure the end of this section is aligned
     END            ; end of file
