totalIncome:
#finds the total income from the file
#arguments:	a0 contains the file record pointer array location (0x10040000 in our example) But your code MUST handle any address value
#		a1:the number of records in the file
#return value a0:the total income (add up all the record incomes)

	#if empty file, return 0 for  a0
	bnez a1, totalIncome_fileNotEmpty
	li a0, 0
	ret
	

totalIncome_fileNotEmpty:
	# Start your coding from here!
	
	# Start your coding from here!
	addi sp, sp, -4
	sw ra, 0(sp)
	addi sp sp, -16
	
	mv t3 a1 #length of file
	li t2, 0 #counter
	#li t4, 0 #storage
	li t6, 0 #sum
	
	addi t1 a0 4 # current address
	
TotalLoop:
	beq t3 t2 TotalEnd # if loop amount equals file amount , return loop
	
	lwu a0, 0(t1) #writes the location of the number
	#store a0??
	 
	#saves the temp, so it doesn't override after loop
	sw t1, 0(sp)
	sw t2, 4(sp)
	sw t3, 8(sp)
	sw t6, 12(sp)
	 
	#gets passed onto income from record
	jal income_from_record
	
	#restores the variables 
	lwu t1, 0(sp)
	lwu t2, 4(sp)
	lwu t3, 8(sp)
	lwu t6, 12(sp)
	
	
	add t6 t6 a0
	addi t2 t2 1 #counter, amount of files plus 1 every loop
	addi t1 t1 8 # shift to next location to read next info 
	#restore a0??
	
	j TotalLoop
	
	
TotalEnd:
	addi sp, sp, 16 #restoring the ra and temps back
	lw ra, 0(sp) #loading the return address back
	addi sp, sp, 4 
	
	mv a0, t6
	#if no student code entered, a0 just returns 0 always :(
	
# End your  coding  here!
	
	ret
#######################end of nameOfMaxIncome_totalIncome###############################################
