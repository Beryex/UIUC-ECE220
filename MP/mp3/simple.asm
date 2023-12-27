; simple schedule used as an example in the specification

	.ORIG x5000

	.STRINGZ "one"		; name of event
	.FILL 21		; bitmap of days (MWF)
	.FILL 6			; slot 6, or 13:00

	.STRINGZ "2"		; name of event
	.FILL 2			; bitmap of days (Tu)
	.FILL 10		; slot 10, or 17:00

	.STRINGZ ""		; blank string marks end of schedule

	.END
