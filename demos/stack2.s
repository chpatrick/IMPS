        jmp start
start:  addi $1 $0 1
        push $1
        call a
        pop $2
        push $1
        call b
        pop $3
        halt

a:      addi $4 $0 4
        push $4
        push $4
        mvsp $20
        addi $20 $20 4
        call c
        pop $5
        pop $5
        ret

b:      add $6 $1 $1
        ret

c:      lw   $7 $20 0
        ret
