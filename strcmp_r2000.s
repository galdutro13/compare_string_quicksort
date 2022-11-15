	strcmp:
	  move $v0,$a0
	  li $a2,1 # 0x1
	$L2:
	  addiu $v0,$v0,1
	  sll $v1,$a2,2
	  addu $a2,$v1,$a2
	  sll $a2,$a2,1
	  lb $a3,0($v0)
	  li $v1,46 # 0x2e
	  bne $a3,$v1,$L2
	  move $v0,$a1
	  li $v1,1 # 0x1
	$L3:
	  addiu $v0,$v0,1
	  sll $a3,$v1,2
	  addu $v1,$a3,$v1
	  sll $v1,$v1,1
	  lb $t0,0($v0)
	  li $a3,46 # 0x2e
	  bne $t0,$a3,$L3
	  move $t0,$a0
	  move $v0,$zero
	  b $L4
	$L5:
	  addiu $a3,$a3,-48
	  mult $a2,$a3
	  mflo $a3
	  addu $v0,$v0,$a3
	  li $a3,10 # 0xa
	  div $zero,$a2,$a3
	  bne $a3,$zero,1f
	  nop
	  break 7
	  mflo $a2
	  addiu $t0,$t0,1
	$L4:
	  lb $a3,0($t0)
	  li $t1,46 # 0x2e
	  bne $a3,$t1,$L5
	  move $a3,$a1
	  move $t1,$zero
	  b $L6
	$L7:
	  addiu $a2,$a2,-48
	  mult $v1,$a2
	  mflo $a2
	  addu $t1,$t1,$a2
	  li $a2,10 # 0xa
	  div $zero,$v1,$a2
	  bne $a2,$zero,1f
	  nop
	  break 7
	  mflo $v1
	  addiu $a3,$a3,1
	$L6:
	  lb $a2,0($a3)
	  li $t2,46 # 0x2e
	  bne $a2,$t2,$L7
	  bne $v0,$t1,$L8
	  addiu $t0,$t0,1
	  bne $a0,$a1,$L11
	$L8:
	  subu $v0,$v0,$t1
	  j $ra
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