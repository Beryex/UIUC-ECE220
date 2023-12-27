; tests for C compiler and library functions...

	.ORIG x3000

	; initialize registers in odd ways...
	LD R2,ODDBITS
	ADD R3,R2,R2
	ADD R4,R3,R3
	ADD R5,R4,R4
	LD R6,ST_PTR

;   tests for MULTIPLY (R0 <- R0 * R1)

	LD R2,MPTR
	NOT R2,R2
	ADD R2,R2,#1
	ST R2,MINV

	AND R0,R0,#0
	ADD R1,R0,#0
	LD R2,MPTR
	JSRR R2 ; 0 x 0
	ADD R0,R0,#0
	BRnp MULTFAIL
	ADD R1,R1,#0
	BRnp MULTFAIL
	LD R1,MINV
	ADD R1,R1,R2
	BRnp MULTFAIL
	LD R2,ODDBITS
	JSR CHECKREGS

	ADD R1,R1,#-10
	LD R2,MPTR
	JSRR R2 ; 0 x -10
	ADD R0,R0,#0
	BRnp MULTFAIL
	ADD R0,R1,#10
	BRnp MULTFAIL
	LD R0,MINV
	ADD R0,R0,R2
	BRnp MULTFAIL
	LD R2,ODDBITS
	JSR CHECKREGS

	ADD R1,R0,#10
	LD R2,MPTR
	JSRR R2 ; 0 x 10
	ADD R0,R0,#0
	BRnp MULTFAIL
	ADD R0,R1,#-10
	BRnp MULTFAIL
	LD R0,MINV
	ADD R0,R0,R2
	BRnp MULTFAIL
	LD R2,ODDBITS
	JSR CHECKREGS

	ADD R1,R0,#-3
	ADD R0,R0,#-3
	LD R2,MPTR
	JSRR R2 ; -3 x -3
	ADD R1,R1,#3
	BRnp MULTFAIL
	ADD R1,R0,#-9
	BRnp MULTFAIL
	LD R1,MINV
	ADD R1,R1,R2
	BRnp MULTFAIL
	LD R2,ODDBITS
	JSR CHECKREGS

	ADD R0,R1,#-3
	LD R2,MPTR
	JSRR R2 ; -3 x 0
	ADD R0,R0,#0
	BRnp MULTFAIL
	ADD R1,R1,#0
	BRnp MULTFAIL
	LD R0,MINV
	ADD R0,R0,R2
	BRnp MULTFAIL
	LD R2,ODDBITS
	JSR CHECKREGS

	ADD R0,R0,#-3
	ADD R1,R1,#3
	LD R2,MPTR
	JSRR R2 ; -3 x 3
	ADD R0,R0,#9
	BRnp MULTFAIL
	ADD R0,R1,#-3
	BRnp MULTFAIL
	LD R0,MINV
	ADD R0,R0,R2
	BRnp MULTFAIL
	LD R2,ODDBITS
	JSR CHECKREGS

	ADD R0,R0,#3
	LD R2,MPTR
	JSRR R2 ; 3 x 3
	ADD R1,R1,#-3
	BRnp MULTFAIL
	ADD R1,R0,#-9
	BRnp MULTFAIL
	LD R1,MINV
	ADD R1,R1,R2
	BRnp MULTFAIL
	LD R2,ODDBITS
	JSR CHECKREGS

	LD R2,MPTR
	JSRR R2 ; 3 x 0
	ADD R0,R0,#0
	BRnp MULTFAIL
	ADD R1,R1,#0
	BRnp MULTFAIL
	LD R0,MINV
	ADD R0,R0,R2
	BRnp MULTFAIL
	LD R2,ODDBITS
	JSR CHECKREGS

	ADD R0,R0,#3
	ADD R1,R1,#-3
	LD R2,MPTR
	JSRR R2 ; 3 x -3
	ADD R0,R0,#9
	BRnp MULTFAIL
	ADD R1,R1,#3
	BRnp MULTFAIL
	LD R0,MINV
	ADD R0,R0,R2
	BRnp MULTFAIL
	LD R2,ODDBITS
	JSR CHECKREGS

	BRnzp DIV_TEST

ST_PTR	.FILL STACK
ODDBITS .FILL x8765
MPTR	.FILL MULTIPLY
MINV	.BLKW 1

MULTFAIL
	LEA R0,BADMULT
	PUTS
	HALT
BADMULT .STRINGZ "Multiply failed a test!\n"

;   tests for DIVIDE   (R0 <- R0 / R1, rounded toward 0)
DIV_TEST
	AND R0,R0,#0
	ADD R1,R0,#4
	ADD R0,R0,#-10
DTLOOP1	ST R0,DLVAR
	JSR DIVIDE
	JSR CHECKREGS
	ADD R2,R1,#-4
	BRnp DIVFAIL
	LEA R2,DLANS
	LD R1,DLVAR
	ADD R2,R2,R1
	LDR R2,R2,#0
	ADD R2,R0,R2
	BRnp DIVFAIL
	LD R0,DLVAR
	AND R1,R1,#0
	ADD R1,R1,#4
	ADD R0,R0,#1
	ADD R2,R0,#-10
	BRp DTL1DONE
	LD R2,ODDBITS
	BRnzp DTLOOP1
DTL1DONE
	LD R2,ODDBITS

	AND R0,R0,#0
	ADD R1,R0,#-4
	ADD R0,R0,#-10
DTLOOP2	ST R0,DLVAR
	JSR DIVIDE
	JSR CHECKREGS
	ADD R2,R1,#4
	BRnp DIVFAIL
	LEA R2,DLANS
	LD R1,DLVAR
	NOT R1,R1
	ADD R1,R1,#1
	ADD R2,R2,R1
	LDR R2,R2,#0
	ADD R2,R0,R2
	BRnp DIVFAIL
	LD R0,DLVAR
	AND R1,R1,#0
	ADD R1,R1,#-4
	ADD R0,R0,#1
	ADD R2,R0,#-10
	BRp DTL2DONE
	LD R2,ODDBITS
	BRnzp DTLOOP2
DTL2DONE
	LD R2,ODDBITS

	BRnzp MOD_TEST

DIVFAIL
	LEA R0,BADDIV
	PUTS
	HALT
DLVAR	.BLKW 1
	.FILL 2
	.FILL 2
	.FILL 2
	.FILL 1
	.FILL 1
	.FILL 1
	.FILL 1
	.FILL 0
	.FILL 0
	.FILL 0
DLANS	.FILL 0
	.FILL 0
	.FILL 0
	.FILL 0
	.FILL -1
	.FILL -1
	.FILL -1
	.FILL -1
	.FILL -2
	.FILL -2
	.FILL -2
BADDIV	.STRINGZ "Divide failed a test!\n"

;   tests for MODULUS  (R0 <- R0 MOD R1, using C's definition)
MOD_TEST
	AND R0,R0,#0
	ADD R1,R0,#4
	ADD R0,R0,#-10
MDLOOP1	ST R0,MDLVAR
	JSR MODULUS
	JSR CHECKREGS
	ADD R2,R1,#-4
	BRnp MODFAIL
	LD R2,MDLVAR
	BRn MDL1NEG
	AND R2,R2,#3
	NOT R2,R2
	ADD R2,R2,#1
	BRnzp MDL1TEST
MDL1NEG	AND R2,R2,#3
	NOT R2,R2
	AND R2,R2,#3
	ADD R2,R2,#1
	AND R2,R2,#3
MDL1TEST
	ADD R2,R0,R2
	BRnp MODFAIL
	LD R0,MDLVAR
	AND R1,R1,#0
	ADD R1,R1,#4
	ADD R0,R0,#1
	ADD R2,R0,#-10
	BRp MDL1DONE
	LD R2,ODDBITS
	BRnzp MDLOOP1
MDL1DONE
	LD R2,ODDBITS

	AND R0,R0,#0
	ADD R1,R0,#-4
	ADD R0,R0,#-10
MDLOOP2	ST R0,MDLVAR
	JSR MODULUS
	JSR CHECKREGS
	ADD R2,R1,#4
	BRnp MODFAIL

	LD R2,MDLVAR
	BRn MDL2NEG
	AND R2,R2,#3
	NOT R2,R2
	ADD R2,R2,#1
	BRnzp MDL2TEST
MDL2NEG	AND R2,R2,#3
	NOT R2,R2
	AND R2,R2,#3
	ADD R2,R2,#1
	AND R2,R2,#3
MDL2TEST
	ADD R2,R0,R2
	BRnp MODFAIL
	LD R0,MDLVAR
	AND R1,R1,#0
	ADD R1,R1,#-4
	ADD R0,R0,#1
	ADD R2,R0,#-10
	BRp MDL2DONE
	LD R2,ODDBITS
	BRnzp MDLOOP2
MDL2DONE
	LD R2,ODDBITS

	BRnzp PRINT_TEST

MODFAIL
	LEA R0,BADMOD
	PUTS
	HALT
MDLVAR	.BLKW 1
BADMOD	.STRINGZ "Modulus failed a test!\n"

;   tests for int PRINTF (const char* fmt, ...);
PRINT_TEST
	LD R1,NEGBIZ
	LEA R0,PTESTS1
	ADD R6,R6,#-1
	STR R0,R6,#0
	LD R0,POSBIZ
	JSR PRINTF
	ADD R0,R0,R1
	BRnp PRINT_FAIL
	LD R0,POSBIZ
	ADD R0,R0,R1
	BRnp PRINT_FAIL
	LDR R0,R6,#0
	ADD R6,R6,#2
	LD R1,PTS1LEN
	ADD R0,R0,R1
	BRnp PRINT_FAIL
	JSR CHECKREGS

	LD R1,NEGBIZ
	LEA R0,PTESTS2
	ADD R6,R6,#-3
	STR R0,R6,#0
	STR R1,R6,#1
	LD R0,POSBIZ
	STR R0,R6,#2
	JSR PRINTF
	ADD R0,R0,R1
	BRnp PRINT_FAIL
	LD R0,POSBIZ
	ADD R0,R0,R1
	BRnp PRINT_FAIL
	LDR R0,R6,#0
	ADD R6,R6,#4
	LD R1,PTS2LEN
	ADD R0,R0,R1
	BRnp PRINT_FAIL
	JSR CHECKREGS

	BRnzp RAND_TEST

POSBIZ	.FILL xABCD
NEGBIZ	.FILL x5433

PRINT_FAIL
	LEA R0,BADPRINT
	PUTS
	HALT
BADPRINT .STRINGZ "PRINTF failed a test!\n"

; the assembler will convert the \n in the string below
PTESTS1 .STRINGZ "A simple example of a string.\n"
PTS1LEN	.FILL #-30
; the assembler will convert the \n and \\ in the string below
; string length is 30 + length of #'s
PTESTS2 .STRINGZ "Conversions: %d %d %% %x %` \\\\ \\n \\q\n"
PTS2LEN	.FILL #-41

;   tests for void SRAND (int new_seed);
;         and int RAND ();
RAND_TEST
	LEA R0,SAY_MATCH
	PUTS

	LD R0,POSBIZ
	LD R1,NEGBIZ
	ADD R6,R6,#-1
	STR R0,R6,#0
	JSR SRAND
	ADD R0,R0,R1
	BRnp PRINT_FAIL
	LD R0,POSBIZ
	ADD R0,R0,R1
	BRnp PRINT_FAIL
	ADD R6,R6,#2
	JSR CHECKREGS

	AND R0,R0,#0
RTLP1	ST R0,RTVAR
	LD R0,POSBIZ
	LD R1,NEGBIZ
	JSR RAND
	ADD R0,R0,R1
	BRnp PRINT_FAIL
	LD R0,POSBIZ
	ADD R0,R0,R1
	BRnp PRINT_FAIL
	LDR R0,R6,#0
	ADD R6,R6,#1
	JSR PRINT_NUM
	AND R0,R0,#0
	ADD R0,R0,#10
	OUT
	JSR CHECKREGS
	LD R0,RTVAR
	ADD R0,R0,#1
	ADD R1,R0,#-8
	BRn RTLP1
	
	LEA R0,SAY_SEQ2
	PUTS

	LD R0,POSBIZ
	LD R1,NEGBIZ
	ADD R6,R6,#-1
	STR R0,R6,#0
	JSR SRAND
	ADD R0,R0,R1
	BRnp PRINT_FAIL
	LD R0,POSBIZ
	ADD R0,R0,R1
	BRnp PRINT_FAIL
	ADD R6,R6,#2
	JSR CHECKREGS

	AND R0,R0,#0
RTLP2	ST R0,RTVAR
	LD R0,POSBIZ
	LD R1,NEGBIZ
	JSR RAND
	ADD R0,R0,R1
	BRnp PRINT_FAIL
	LD R0,POSBIZ
	ADD R0,R0,R1
	BRnp PRINT_FAIL
	LDR R0,R6,#0
	ADD R6,R6,#1
	JSR PRINT_NUM
	AND R0,R0,#0
	ADD R0,R0,#10
	OUT
	JSR CHECKREGS
	LD R0,RTVAR
	ADD R0,R0,#1
	ADD R1,R0,#-8
	BRn RTLP2

	BRnzp SCANF_TEST

RTVAR .BLKW 1
SAY_MATCH .STRINGZ "\n\nThe following sequences should match...\n"
SAY_SEQ2  .STRINGZ "Second sequence...\n"


POSB2	.FILL xABCD
NEGB2	.FILL x5433
SCAN1 .STRINGZ "%d/%d:%d%d"
SCAN2 .STRINGZ "--%d%d"

;   tests for int SCANF (const char* fmt, ...);
SCANF_TEST
	LEA R0,SAY_TYPE
	PUTS

	ST R0,SC_V1
	ST R0,SC_V2
	ST R0,SC_V3
	ST R0,SC_V4

	ADD R6,R6,#-5
	LEA R0,SCAN1
	STR R0,R6,#0
	LEA R0,SC_V1
	STR R0,R6,#1
	ADD R0,R0,#1
	STR R0,R6,#2
	ADD R0,R0,#1
	STR R0,R6,#3
	ADD R0,R0,#1
	STR R0,R6,#4
	LD R1,NEGB2
	LD R0,POSB2
	JSR SCANF
	ADD R0,R0,R1
	BRnp SCAN_FAIL
	LD R0,POSB2
	ADD R0,R0,R1
	BRnp SCAN_FAIL
	LDR R0,R6,#0
	ADD R6,R6,#6
	ADD R0,R0,#-4
	BRnp SCAN_FAIL
	JSR CHECKREGS

	LD R0,SC_V1
	BRnp SCAN_FAIL
	LD R1,SC_V2
	ADD R1,R1,#-1
	BRnp SCAN_FAIL
	LD R1,SC_V3
	ADD R1,R1,#-2
	BRnp SCAN_FAIL
	LD R1,SC_V4
	ADD R1,R1,#-3
	BRnp SCAN_FAIL

	ADD R6,R6,#-3
	LEA R0,SCAN2
	STR R0,R6,#0
	LEA R0,SC_V2
	STR R0,R6,#1
	ADD R0,R0,#1
	STR R0,R6,#2
	LD R1,NEGB2
	LD R0,POSB2
	JSR SCANF
	ADD R0,R0,R1
	BRnp SCAN_FAIL
	LD R0,POSB2
	ADD R0,R0,R1
	BRnp SCAN_FAIL
	LDR R0,R6,#0
	ADD R6,R6,#4
	ADD R0,R0,#-2
	BRnp SCAN_FAIL
	JSR CHECKREGS

	LD R0,SC_V1
	BRnp SCAN_FAIL
	LD R1,SC_V2
	ADD R1,R1,#4
	BRnp SCAN_FAIL
	LD R1,SC_V3
	ADD R1,R1,#-5
	BRnp SCAN_FAIL
	LD R1,SC_V4
	ADD R1,R1,#-3
	BRnp SCAN_FAIL

	ADD R6,R6,#-2
	LEA R0,SCAN3
	STR R0,R6,#0
	LEA R0,SC_V4
	STR R0,R6,#1
	LD R1,NEGB2
	LD R0,POSB2
	JSR SCANF
	ADD R0,R0,R1
	BRnp SCAN_FAIL
	LD R0,POSB2
	ADD R0,R0,R1
	BRnp SCAN_FAIL
	LDR R0,R6,#0
	ADD R6,R6,#3
	ADD R0,R0,#0
	BRnp SCAN_FAIL
	JSR CHECKREGS

	LD R0,SC_V1
	BRnp SCAN_FAIL
	LD R1,SC_V2
	ADD R1,R1,#4
	BRnp SCAN_FAIL
	LD R1,SC_V3
	ADD R1,R1,#-5
	BRnp SCAN_FAIL
	LD R1,SC_V4
	ADD R1,R1,#-3
	BRnp SCAN_FAIL

	ADD R6,R6,#-2
	LEA R0,SCAN4
	STR R0,R6,#0
	LEA R0,SC_V2
	STR R0,R6,#1
	LD R1,NEGB2
	LD R0,POSB2
	JSR SCANF
	ADD R0,R0,R1
	BRnp SCAN_FAIL
	LD R0,POSB2
	ADD R0,R0,R1
	BRnp SCAN_FAIL
	LDR R0,R6,#0
	ADD R6,R6,#3
	ADD R0,R0,#0
	BRnp SCAN_FAIL
	JSR CHECKREGS

	LD R0,SC_V1
	BRnp SCAN_FAIL
	LD R1,SC_V2
	ADD R1,R1,#4
	BRnp SCAN_FAIL
	LD R1,SC_V3
	ADD R1,R1,#-5
	BRnp SCAN_FAIL
	LD R1,SC_V4
	ADD R1,R1,#-3
	BRnp SCAN_FAIL

	ADD R6,R6,#-2
	LEA R0,SCAN5
	STR R0,R6,#0
	LEA R0,SC_V3
	STR R0,R6,#1
	LD R1,NEGB2
	LD R0,POSB2
	JSR SCANF
	ADD R0,R0,R1
	BRnp SCAN_FAIL
	LD R0,POSB2
	ADD R0,R0,R1
	BRnp SCAN_FAIL
	LDR R0,R6,#0
	ADD R6,R6,#3
	ADD R0,R0,#-1
	BRnp SCAN_FAIL
	JSR CHECKREGS

	LD R0,SC_V1
	BRnp SCAN_FAIL
	LD R1,SC_V2
	ADD R1,R1,#4
	BRnp SCAN_FAIL
	LD R1,SC_V3
	ADD R1,R1,#6
	BRnp SCAN_FAIL
	LD R1,SC_V4
	ADD R1,R1,#-3
	BRnp SCAN_FAIL

	HALT

SCAN_FAIL
	LEA R0,BADSCAN
	PUTS
	HALT
BADSCAN .STRINGZ "SCANF failed a test!\n"

SC_V1 .BLKW 1 ; variables MUST be contiguous
SC_V2 .BLKW 1
SC_V3 .BLKW 1
SC_V4 .BLKW 1
SAY_TYPE .STRINGZ "\n\nType \"  0/1:2\", press ENTER/CTRL-M a few times,\nthen type \"3---4 5+-6\" and press ENTER again\n>> "
SCAN3 .STRINGZ "%d"
SCAN4 .STRINGZ "0%d"
SCAN5 .STRINGZ "+%d"

NEGODD  .FILL x789B

CHECKREGS
	ADD R6,R6,#-2
	STR R0,R6,#0
	STR R1,R6,#1
	LD R0,NEGODD
	ADD R1,R0,R2
	BRnp CHECKFAIL 
	ADD R0,R0,R0
	ADD R1,R0,R3
	BRnp CHECKFAIL 
	ADD R0,R0,R0
	ADD R1,R0,R4
	BRnp CHECKFAIL 
	ADD R0,R0,R0
	ADD R1,R0,R5
	BRnp CHECKFAIL 
	LEA R0,STACK
	NOT R0,R0
	ADD R0,R0,#3
	ADD R1,R0,R6
	BRnp CHECKFAIL
	LDR R0,R6,#0
	LDR R1,R6,#1
	ADD R6,R6,#2
	RET
CHECKFAIL
	LEA R0,BADREGS
	PUTS
	HALT
BADREGS .STRINGZ "Callee-saved register has changed!\n"

	.BLKW #100
STACK

; LC-3 should halt without any message about failures;
; copy part3.asm into this place and then run to test...

	.END
