  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #

hexasc:
	andi	$v0, $a0, 0xF	# We start with removing all the bits from the incoming variable, except the 4 least significant bits
	
	blt     $v0, 0xA, convert # If the four least significant bits of $v0 are together less than 10, then run "convert"
	nop
	addi    $v0, $v0, 0x37 # if the four least significant bits are more or equal to 10, then find the fitting ASCII address by adding 0x37
	
	convert: 
		addi $v0, $v0, 0x30 # We find the fitting ASCII address by adding 0x30, meaning we will land on the address for 0+x where x is the value that the 4 lsb sum to. 
		jr $ra
		nop
				
.global delay
delay: # takes ms ( $a0 ) as argument                        
       move $t0, $a0
       whileLoop:  
            ble  $t0, $0, endLoop     # Check if $a0 == 0, and if true -> end loop, else continue loop. 
            addi $t1, $0, 4450        # Create a comparable variable used to determine when the for-loop should end.
            addi $t0, $t0, -1         # ms = ms - 1
        forLoop:
                beq  $t1, $0 , whileLoop  
                nop
                addi $t1, $t1, -1        # Istead of writing i = i + 1, we can  
                                         # instead decrease the 
                                         # value for the constant and continue the loop until its 0. 
            j forLoop                    # Rerun the for-loop 
            nop                 
endLoop:
    jr $ra 
    nop   	  				
    
    	
.global time2string
time2string:
    # save caller-based values / callee based
    PUSH $ra
    PUSH $s0
    PUSH $s1
    PUSH $s2
# save address and argument
    move $s0, $a0 # address
    move $s1, $a1 # digits
# Remove the non-specific bytes (only need 4 least significant) 
    andi $s1, $s1, 0xffff 
# save nullbyte
    li $s2, 0x00
    sb $s2, 5($s0)
    move $a0, $s1    
# run hexacs on 4th digit
    jal hexasc 
    nop
    sb $v0, 4($s0)    
 # 3rd digit
    srl $a0, $s1, 4
    jal hexasc 
    nop
    sb $v0, 3($s0)
    
# colon
    li $s2, 0x3A
    sb $s2, 2($s0)
    
# 2nd digit
    srl $a0, $s1, 8
    jal hexasc 
    nop
    sb $v0, 1($s0)
    
# 1st digit
    srl $a0, $s1, 12
    jal hexasc 
    nop
    sb $v0, 0($s0)
    j endtwo 
	nop
endtwo: 
# pop from stack
    POP $s2
    POP $s1
    POP $s0
    POP $ra
    jr $ra
    nop
	
.global enable_interrupt
enable_interrupt:
  ei	