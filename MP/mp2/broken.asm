; broken schedule used to illustrate error messages

	.ORIG x5000

	.STRINGZ "one"		; name of event
	.FILL 21		; bitmap of days (MWF)
	.FILL 6			; slot 6, or 13:00

	.STRINGZ "boring event"	; name of event
	.FILL 26		; bitmap of days (TuThF)
	.FILL 6			; slot 6, or 13:00

	.STRINGZ "silly event"	; name of event
	.FILL 2			; bitmap of days (Tu)
	.FILL 17		; non-existent slot

	.STRINGZ ""		; blank string marks end of schedule

	.END
