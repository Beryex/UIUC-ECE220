; simple schedule used as an example in the specification

	.ORIG x5000
		.STRINGZ "one"		; name of event
		.FILL x0015		; bitmap of days (MWF)
		.FILL x0006		; slot 6, or 13:00
		.STRINGZ "2"		; name of event
		.FILL x0002		; bitmap of days (Tu)
		.FILL x000A		; slot 10, or 17:00
		.STRINGZ ""		; blank string marks end of schedule

	.END
