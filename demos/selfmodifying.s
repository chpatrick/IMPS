        jmp start

start:  jmp modify

modify: lw $1 $0 start  ; load the instruction at 'start' into $1
        addi $1 $1 16   ; add 16 (to it's immediate operand)
        sw $1 $0 start  ; write this new instruction to 'start'
        jmp start       ; jump there and execute it!

hax:    addi $7 $0 69   ; a good number
        halt