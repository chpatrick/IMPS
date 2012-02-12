        jmp start       ; IMPS compatible version

start:  addi $30 $30 0x0fff ; stack pointer
        muli $30 $30 16
        addi $30 $30 0xb
        addi $2 $0 3    ; first argument m
        addi $3 $0 9    ; second argument n
        jal ack         ; $31 = pc + 4, pc = ack
        halt            ; result on $1

ack:    sw $31 $30 0    ; push return address
        subi $30 $30 4  ; sp += 4

tail:   addi $20 $20 1  ; $20++, call counter
        beq $2 $0 case1 ; if m = 0, case 1
        beq $3 $0 case2 ; if n = 0 (and m > 0), case 2, otherwise m,n>0 so case3

case3:  subi $3 $3 1    ; n = n - 1
        sw $2 $30 0     ; push m
        subi $30 $30 4
        jal ack         ; $1 = A(m, n-1)
        addi $30 $30 4  ; pop m
        lw $2 $30 0
        addi $3 $1 0    ; n = A(m, n-1)
        subi $2 $2 1    ; m = m - 1
        jmp tail        ; $1 = A(m-1, A(m, n-1))

case1:  addi $1 $3 1    ; $1 = n + 1
        addi $30 $30 4
        lw $31 $30 0
        jr $31

case2:  subi $2 $2 1    ; m = m - 1
        addi $3 $0 1    ; n = 1
        jmp tail        ; $1 = A(m-1, 1)
