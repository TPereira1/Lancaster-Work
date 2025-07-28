addi $s0, $zero,0
lui $s0, 0x1004
addi $a0, $zero, 0x00ff
addi $s1, $s1, 1
sll $s1,$s1,20
add $s1, $s0, $s1
loop:
beq $s0,$s1, exit
sw $a0, 0($s0)
addi $s0, $s0, 4
j loop
exit:
addi $a0, $zero, 0x00ff00
lui $s0, 0x1004
loop3:
addi $s1, $s0,512
loop2:
beq $s0,$s1, exit2
sw $a0, 0($s0)
addi $s0, $s0, 4
j loop2
exit2:
addi $s0, $s0,1024
j loop3
