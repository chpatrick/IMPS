jmp     start

twoi:   .fill 2
two:    .fill f2.0
three:  .fill f3.0
one:    .fill f1.0
zero:   .fill f0.0

width:  .fill 640
height: .fill 480

iterations: .fill 100

rcenter: .fill f2.0
icenter: .fill f1.0

start:  gpu 0         - Initialise Opengl
        
; set up r3 as x scaling factor
        lw $1 $0 width
        itof $1 $1
        lw $3 $0 three
        fdiv $3 $3 $1  - r3 = 3 / width

; set up r4 as y scaling factor
        lw $1 $0 height
        itof $1 $1
        lw $4 $0 two
        fdiv $4 $4 $1  - r4 = 2 / width

; set up r5 as x translation factor
        lw $5 $0 rcenter

; set up r6 as y translation factor
        lw $6 $0 icenter

loop:   gpu 1         - Start frame

        gpu 3         - 2D Projection

        lw $1 $0 height
drawRow:
        lw $2 $0 width
draw:
        itof $20 $2
        itof $21 $1

        lw $7 $0 zero
        lw $8 $0 zero
        lw $11 $0 iterations
        lw $12 $0 twoi
        itof $13 $11
        lw $18 $0 two

iterate:
; r9 and r10 are temporary
        fmul $14 $7 $8
 
        fmul $9 $7 $7
        fmul $10 $8 $8
        fsub $7 $9 $10

        fmul $8 $14 $18
        
; (r, i) = z^2

; (r9, r10) = c
        fmul $9 $3 $20 
        fsub $9 $9 $5
        
        fmul $10 $4 $21
        fsub $10 $10 $6

        fadd $7 $7 $9
        fadd $8 $8 $10

        ftoi $9 $7
        bgt $9 $12 out
        ftoi $9 $8
        bgt $9 $12 out
        subi $11 $11 1
        bgt $11 $0 iterate
; in
        lw $23 $0 one
        lw $24 $0 one
        lw $25 $0 one
        jmp drawPixel

out:
        lw $23 $0 iterations
        sub $23 $23 $11
        itof $23 $23
        fdiv $23 $23 $13
        addi $24 $23 0
        addi $25 $23 0
drawPixel:

        gpu 6         - Draw pixel

        subi $2 $2 1
        bgt $2 $0 draw
        subi $1 $1 1
        bgt $1 $0 drawRow

        gpu 8         - Present frame
        jmp loop
