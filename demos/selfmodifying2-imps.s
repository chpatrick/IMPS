        jmp start       ; fills $2 - $31 with the first 29 fibonacci numbers

start:  addi $0 $0 0x4210       ; 100001000010000
        muli $0 $0 0x80         ; arithmatic left shift  7
        addi $2 $2 0
        addi $3 $3 1
loop:   add $4 $3 $2
        lw $1 $2 loop           ; load 'loop' instruction
        add $1 $1 $0            ; increment all 3 register operands
        sw $1 $2 loop           ; save instruction at 'loop'
        beq $31 $2 loop         ; do while $31 == 0
        halt