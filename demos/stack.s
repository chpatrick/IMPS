        jmp start
start:  addi $1 $0 1
        push $1
        pop  $2
        addi $3 $0 2
        push $3
        addi $5 $0 3
        push $5
        pop  $6
        pop  $4
        halt
