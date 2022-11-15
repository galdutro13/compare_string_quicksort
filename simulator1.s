.data
	entrada:	.ascii "8.3\n1.7\n3.1\n3.7\n5.4\n3084.72\n12.1\n"
			.align 2
	free_block_list_head:
		.space 8
		.align 2
	
	p_break.0:
		.word global_mem.1
		.align 2

	global_mem.1:
		.space 1048576
		.align 2
		
	head_orderedstring:
		.word
		.align 2
.text
	main:
	  la $a0, entrada
	  li $v0, 4 
	  syscall
	  li $v0, 0
	  li $a0, 0
	  addiu $sp,$sp,-32
	  sw $ra,28($sp)
	  sw $s0,24($sp)
	  la $a0,entrada
	  jal str_split #la $t9,str_split
	  #jr $t9
	  move $s0,$v0
	  sw $s0, head_orderedstring
	  la $a0,entrada
	  jal findsize
	  
	  move $a1,$v0
	  move $a0,$s0
	  jal quicksort
	  
	  move $v0,$zero
	  lw $ra,28($sp)
	  lw $s0,24($sp)
	  addiu $sp,$sp,32
	  lw $s0, head_orderedstring
	  addi $s2, $zero, 0
	  jal printer
	  li $a0, 0
	  li $v0, 10
	  syscall
	  # jr $ra
	
	printer_header:
	  li $s0, 0
	  jr $ra
	printer:
	  sll $v1, $s2, 2
	  addu $v1, $v1, $s0
	  lw $s1, 0($v1)

	  move $s3, $zero
	  
	printer_inner:
	  addu $t0, $s1, $s3
	  lb $a0, 0($t0)
	  li $v0, 11
	  syscall
	  
	  addiu $s3, $s3, 1
	  bne $a0, $zero, printer_inner
	  
	  addiu $s2, $s2, 1
	  
	  sll $t1, $s2, 2
	  addu $t1, $t1, $s0
	  lw $t1, 0($t1)
	  lw $t1, 0($t1)
	  beq $t1, $zero, printer_header
	  
	  li $a0, 32
	  li $v0, 11
	  syscall
	  
	  la $t9, printer
	  jr $t9
	    
	strcmp:
	  move $v0,$a0
	  li $v1,1 # 0x1
	$L2:
	  addiu $v0,$v0,1
	  sll $a2,$v1,2
	  addu $v1,$a2,$v1
	  sll $v1,$v1,1
	  lb $a3,0($v0)
	  li $a2,46 # 0x2e
	  bne $a3,$a2,$L2
	  move $a3,$a1
	  li $a2,1 # 0x1
	$L3:
	  addiu $a3,$a3,1
	  sll $v0,$a2,2
	  addu $v0,$v0,$a2
	  sll $a2,$v0,1
	  lb $t0,0($a3)
	  li $v0,46 # 0x2e
	  bne $t0,$v0,$L3
	  move $t0,$a0
	  move $v0,$zero
	  b $L4
	$L5:
	  addiu $a3,$a3,-48
	  mul $t1,$v1,$a3
	  addu $v0,$t1,$v0
	  li $a3,1717960704 # 0x66660000
	  addiu $a3,$a3,26215
	  mult $v1,$a3
	  mfhi $a3
	  sra $a3,$a3,2
	  sra $v1,$v1,31
	  subu $v1,$a3,$v1
	  addiu $t0,$t0,1
	$L4:
	  lb $a3,0($t0)
	  li $t1,46 # 0x2e
	  bne $a3,$t1,$L5
	  move $a3,$a1
	  move $t1,$zero
	  b $L6
	$L7:
	  addiu $v1,$v1,-48
	  mul $t2,$a2,$v1
	  addu $t1,$t2,$t1
	  li $v1,1717960704 # 0x66660000
	  addiu $v1,$v1,26215
	  mult $a2,$v1
	  mfhi $v1
	  sra $v1,$v1,2
	  sra $a2,$a2,31
	  subu $a2,$v1,$a2
	  addiu $a3,$a3,1
	$L6:
	  lb $v1,0($a3)
	  li $t2,46 # 0x2e
	  bne $v1,$t2,$L7
	  bne $v0,$t1,$L8
	  addiu $t0,$t0,1
	  bne $a0,$a1,$L11
	$L8:
	  subu $v0,$v0,$t1
	  jr $ra 
	$L12:
	  addiu $v1,$v1,-48
	  addu $v0,$v0,$v1
	  addiu $t0,$t0,1
	$L11:
	  lb $v1,0($t0)
	  bne $v1,$zero,$L12
	  addiu $a3,$a3,1
	  b $L13
	$L14:
	  addiu $v1,$v1,-48
	  addu $t1,$t1,$v1
	  addiu $a3,$a3,1
	$L13:
	  lb $v1,0($a3)
	  bne $v1,$zero,$L14
	  subu $v0,$v0,$t1
	  jr $ra
	swap:
	  lw $v0,0($a0)
	  lw $v1,0($a1)
	  sw $v1,0($a0)
	  sw $v0,0($a1)
	  jr $ra
	quicksort:
	  addiu $sp,$sp,-56
	  sw $ra,52($sp)
	  sw $s5,48($sp)
	  sw $s4,44($sp)
	  sw $s3,40($sp)
	  sw $s2,36($sp)
	  sw $s1,32($sp)
	  sw $s0,28($sp)
	  slti $v0,$a1,2
	  move $s2,$a0
	  move $s3,$a1
	  move $s4,$zero
	  move $s1,$zero
	  bne $v0,$zero,$L19
	  sltu $v0,$s1,$s3
	  beq $v0,$zero,$L25
	$L23:
	  sll $s0,$s1,2
	  addu $s0,$s2,$s0
	  li $v0,1073676288 # 0x3fff0000
	  ori $v0,$v0,0xffff
	  addu $v0,$s3,$v0
	  sll $v0,$v0,2
	  addu $v0,$s2,$v0
	  la $a1,0($v0)
	  la $a0,0($s0)
	  jal strcmp
	  bltz $v0,$L26
	  addiu $s1,$s1,1
	$L27:
	  sltu $v0,$s1,$s3
	  bne $v0,$zero,$L23
	$L25:
	  sll $a0,$s4,2
	  li $a1,1073676288 # 0x3fff0000
	  ori $a1,$a1,0xffff
	  addu $a1,$s3,$a1
	  sll $a1,$a1,2
	  addu $a1,$s2,$a1
	  addu $a0,$s2,$a0
	  jal swap
	  
	  addiu $s1,$s4,1
	  move $a1,$s4
	  move $a0,$s2
	  jal quicksort
	  
	  sll $a0,$s1,2
	  subu $a1,$s3,$s1
	  addu $a0,$s2,$a0
	  jal quicksort
	  
	$L19:
	  lw $ra,52($sp)
	  lw $s5,48($sp)
	  lw $s4,44($sp)
	  lw $s3,40($sp)
	  lw $s2,36($sp)
	  lw $s1,32($sp)
	  lw $s0,28($sp)
	  addiu $sp,$sp,56
	  jr $ra
	$L26:
	  addiu $s5,$s4,1
	  sll $a1,$s4,2
	  addu $a1,$s2,$a1
	  move $a0,$s0
	  jal swap
	  
	  move $s4,$s5
	  addiu $s1,$s1,1
	  b $L27
	findsize:
	  move $v0,$zero
	  lb $v1,0($a0)
	  beq $v1,$zero,$L33
	$L31:
	  li $a1,10 # 0xa
	  beq $v1,$a1,$L34
	  addiu $a0,$a0,1
	$L35:
	  lb $v1,0($a0)
	  bne $v1,$zero,$L31
	$L33:
	  jr $ra
	$L34:
	  addiu $v0,$v0,1
	  addiu $a0,$a0,1
	  b $L35
	str_split:
	  addiu $sp,$sp,-64
	  sw $ra,60($sp)
	  sw $s5,56($sp)
	  sw $s4,52($sp)
	  sw $s3,48($sp)
	  sw $s2,44($sp)
	  sw $s1,40($sp)
	  sw $s0,36($sp)
	  move $s0,$a0
	  move $v1,$a0
	  move $s4,$zero
	  lb $v0,0($v1)
	  beq $v0,$zero,$L52
	$L39:
	  li $a0,10 # 0xa
	  beq $v0,$a0,$L53
	  addiu $v1,$v1,1
	$L56:
	  lb $v0,0($v1)
	  bne $v0,$zero,$L39
	$L52:
	  addiu $s4,$s4,1
	  sll $a0,$s4,2
	  jal malloc
	  move $s5,$v0
	  move $s1,$s0
	  move $s3,$zero
	  move $s2,$zero
	  beq $v0,$zero,$L36
	  slt $v0,$s3,$s4
	  bne $v0,$zero,$L49
	  b $L36
	$L44:
	  li $v1,10 # 0xa
	  beq $v0,$v1,$L54
	  addiu $s2,$s2,1
	  addiu $s1,$s1,1
	$L49:
	  lb $v0,0($s1)
	  bne $v0,$zero,$L44
	$L43:
	  move $a0,$s2
	  add $t8, $zero, $a0
	  jal malloc
	  sll $v1,$s3,2
	  addu $v1,$s5,$v1
	  sw $v0,0($v1)
	  b $L45
	$L48:
	  li $a0,10 # 0xa
	  beq $v1,$a0,$L55
	  sb $v1,0($v0)
	  addiu $s0,$s0,1
	  addiu $v0,$v0,1
	$L45:
	  lb $v1,0($s0)
	  bne $v1,$zero,$L48
	  addiu $s3,$s3,1
	$L57:
	  slt $v0,$s3,$s4
	  bne $v0,$zero,$L49
	$L36:
	  move $v0,$s5
	  lw $ra,60($sp)
	  lw $s5,56($sp)
	  lw $s4,52($sp)
	  lw $s3,48($sp)
	  lw $s2,44($sp)
	  lw $s1,40($sp)
	  lw $s0,36($sp)
	  addiu $sp,$sp,64
	  jr $ra
	$L53:
	  addiu $s4,$s4,1
	  addiu $v1,$v1,1
	  b $L56
	$L55:
	  addiu $s0,$s0,1
	  addiu $s3,$s3,1
	  b $L57
	$L54:
	  addiu $s1,$s1,1
	  b $L43
	malloc:
	  addiu $a0,$a0,23
	  li $v0,-16 # 0xfffffffffffffff0
	  and $a0,$a0,$v0
	  lw $v0,free_block_list_head+4
	  la $a1,free_block_list_head+4
	  b $L61
	$L64:
	  lwl $v1,0($v0)
	  lwr $v1,3($v0)
	  slt $v1,$v1,$a0
	  beq $v1,$zero,$L69
	  addiu $a1,$v0,4
	  lwl $v1,4($v0)
	  lwr $v1,7($v0)
	  move $v0,$v1
	$L61:
	  bne $v0,$zero,$L64
	  lw $v0,p_break.0
	  la $v1,global_mem.1
	  subu $v1,$v1,$v0
	  slt $v1,$a0,$v1
	  bne $v1,$zero,$L66
	  la $v1,p_break.0
	  la $v1,1048576($v1)
	  subu $v1,$v1,$v0
	  slt $v1,$a0,$v1
	  beq $v1,$zero,$L67
	  addu $v1,$v0,$a0
	  sw $v1,p_break.0
	  swl $a0,0($v0)
	  swr $a0,3($v0)
	  addiu $v0,$v0,8
	  jr $ra
	$L69:
	  lwl $v1,4($v0)
	  lwr $v1,7($v0)
	  sw $v1,0($a1)
	  addiu $v0,$v0,8
	  jr $ra
	$L66:
	  li $v0,-1 # 0xffffffffffffffff
	  swl $a0,0($v0)
	  swr $a0,3($v0)
	  addiu $v0,$v0,8
	  jr $ra
	$L67:
	  li $v0,-1 # 0xffffffffffffffff
	  swl $a0,0($v0)
	  swr $a0,3($v0)
	  addiu $v0,$v0,8
	  jr $ra
	
