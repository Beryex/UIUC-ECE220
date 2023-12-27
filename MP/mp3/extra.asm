	.ORIG x6000

	.FILL NAME1	; "A"
	.FILL x15	; M W F
	.FILL x0148	; 10, 13, 15

	.FILL NAME2	; "B"
	.FILL x10	; F
	.FILL x008A	; 8, 10, 14

	.FILL NAME3	; "C"
	.FILL x1F	; M Tu W Th F
	.FILL x0212	; 8, 11, 16

	.FILL NAME4	; "D"
	.FILL x1A	; Tu Th F
	.FILL x0094	; 9, 11, 14

	.FILL NAME5	; "E"
	.FILL x07	; M Tu W
	.FILL x0504	; 9, 15, 17

	.FILL NAME6	; "F"
	.FILL x09	; M Th
	.FILL x0240	; 13, 16

	.FILL x0000	; end of list

NAME1	.STRINGZ "A"
NAME2	.STRINGZ "B"
NAME3	.STRINGZ "C"
NAME4	.STRINGZ "D"
NAME5	.STRINGZ "E"
NAME6	.STRINGZ "F"

	.END

