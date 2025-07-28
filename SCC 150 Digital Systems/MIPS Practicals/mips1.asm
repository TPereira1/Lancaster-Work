addi $s0,$zero,0
lui $s0,0x1001
addi $s1,$s0,512
addi $a0,$zero,0x00ff
addi $a2,$zero,0x00ff00
loop: 
	beq $s0, $s1, exit
	sw $a0,0($s0)
	addi $s0, $s0, 4
	b loop
exit:
	lui $s0,0x1001
	addi $s0, $s0,128
	addi $s1,$s0,64
	addi $a0,$zero,0x00ff0000
	b loop2
loop2:
	beq $s0, $s1, exit2
	sw $a0,0($s0)
	addi $s0, $s0, 4
	b loop2
exit2: 
	lui $s0,0x1001
	addi $s0, $s0,20
	addi $s1,$s0,576
	b loop3
loop3:
	beq $s0, $s1, exit3
	sw $a2,0($s0)
	addi $s0, $s0,64
	b loop3
exit3: