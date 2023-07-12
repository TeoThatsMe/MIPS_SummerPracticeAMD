main: xor $t0,$t0,$t0
addi $t0,$0,16
sw $t0,0($0)
addi $t0,$t0,16
sw $t0,1($0)
addi $t0,$t0,16
sw $t0,2($0)
addi $t0,$t0,16
sw $t0,3($0)

xor $t0,$t0,$t0
xor $t1,$t1,$t1
addi $t2,$0,3
loop_start:
lw $t3,0($t0)
add $t1,$t1,$t3
beq $t0,$t2,loop_end
addi $t0,$t0,1
j loop_start
loop_end:
addi $t0,$0,100
slt $t2,$t0,$t1
and $t2,$t0,$t1
sw $t2,0($0)
lw $t0,0($0)
j main