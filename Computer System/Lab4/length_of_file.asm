length_of_file:
#function to find length of data read from file
#arguments: a1=bufferAdress holding file data
#return file length in a0
	
#Start your coding here
li a0, 0
li t1, 0x00
LengthLoop:
#load a1 data to a register
	lb t0 (a1)
#Check if it's null, if yes then end
	beq t0, t1, LengthEnd
#increase byte count by 1
	addi a0, a0, 1
#increase data position by 1
	addi a1, a1, 1
	j LengthLoop
LengthEnd:

#if no student code provided, this function just returns 0 in a0

#End your coding here
	
	ret
#######################end of length_of_file###############################################	
