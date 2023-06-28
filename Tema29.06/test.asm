main:
    lui $s0,0x1000

    addi $t0, $0,50
    sw $t0,0($s0)
    addi $t0,$0,16
    sw $t0,4($s0)
    addi $t0,$0,-10
    sw $t0,8($s0)
    addi $t0,$0,5
    sw $t0,12($s0)

    xor $t0,$t0,$t0
    lw $t1,0($s0)
    add $t0,$t0,$t1
    lw $t1,4($s0)
    add $t0,$t0,$t1
    lw $t1,8($s0)
    add $t0,$t0,$t1
    lw $t1,12($s0)
    add $t0,$t0,$t1
    
    jr $ra
    