li $s0, 0x10045000
addi $s1,$s0,2048
addi $t0, $zero, 5
addi $t1, $zero, 0
addi $a0, $zero, 0x00ff

stave:
beq $t1, $t0, exit2
beq $s0,$s1, exit
sw $a0, 0($s0)
addi $s0, $s0, 4
j stave

exit:
addi $s0, $s0, 20480
addi $s1,$s0,2048
addi $t1, $t1, 1
j stave
exit2:
