income_from_record:
#function to return numerical income from a specific record
#e.g. for record "Microsoft,34\r\n", income to return is 34(for which name is Microsoft)

#arguments:	a0 contains pointer to start of numerical income in record 

#function RETURNS income numerical value of the asci income in a0 (34 in our example)
	
# Start your coding from here!

	li t3, 0xd
	li t1, 0
	li t2, 10

RecordLoop:
	lbu t0, 0(a0)
	beq t0, t3, RecordEnd
	#convert binary into integer
	addi t0, t0, -48
	mul t1, t1, t2
	add t1, t1, t0

	addi a0, a0, 1
	j RecordLoop
RecordEnd:
	mv a0, t1
	#if no student code entered, a0 just returns 0 always :(
	
# End your  coding  here!
	ret
	
#######################end of income_from_record###############################################	
