        jmp start       ; OPTIMISED VERSION, lets see what this bad boy can do

start:  addi $2 $0 3    ; first argument m
        addi $3 $0 10   ; second argument n
        call ack
        halt            ; result on $1

ack:    addi $20 $20 1  ; $20++, to track the number of calls, for the lols
        beq $2 $0 case1 ; if m = 0, case 1
        beq $3 $4 case2 ; if n = 0 (and m > 0), case 2, otherwise m,n>0 so case3

case3:  subi $3 $3 1    ; n = n - 1
        push $2
        call ack        ; $1 = A(m, n-1)
        pop $2
        addi $3 $1 0    ; n = A(m, n-1)
        subi $2 $2 1    ; m = m - 1
        jmp ack         ; $1 = A(m-1, A(m, n-1))

case1:  addi $1 $3 1    ; $1 = n + 1
        ret

case2:  subi $2 $2 1    ; m = m - 1
        addi $3 $0 1    ; n = 1
        jmp ack         ; $1 = A(m-1, 1)
