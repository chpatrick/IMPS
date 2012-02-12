        jmp start       ; calculates the 2 argument Ackermann-Péter function, $1 = A(m, n) where m = $2 and n = $3

start:  addi $2 $0 3    ; first argument m
        addi $3 $0 9    ; second argument n
        push $3         ; push second argument n
        push $2         ; push first argument m
        call ack
        pop $2
        pop $3
        halt            ; result on $1

ack:    addi $20 $20 1  ; calls to ack ;)
        push $7         ; conserve the caller's bp
        mvsp $7         ; create base pointer
        lw $2 $7 10     ; 1st argument m at bp+10
        lw $3 $7 14     ; 2nd argument n at bp+14

        beq $2 $0 case1 ; if m = 0, case 1
        beq $3 $4 case2 ; if n = 0 (and m > 0), case 2, otherwise m,n>0 so case3

case3:  subi $3 $3 1    ; n = n - 1
        push $3
        push $2
        call ack        ; $1 = A(m, n-1)
        pop $2
        pop $3
        addi $3 $1 0    ; n = A(m, n-1)
        subi $2 $2 1    ; m = m - 1
        push $3
        push $2
        call ack        ; $1 = A(m-1, A(m, n-1))
        pop $2
        pop $3
        jmp return

case1:  addi $1 $3 1    ; $1 = n + 1
        jmp return

case2:  subi $2 $2 1    ; m = m - 1
        addi $3 $0 1    ; n = 1
        push $3
        push $2
        call ack        ; $1 = A(m-1, 1)
        pop $2
        pop $3
        jmp return

return: pop $7
        ret             ; result on $1
