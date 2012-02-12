jmp	    start

float:    .fill f10.0 ;x
          .fill f10.0 ;y
          .fill f0.5 ;r
          .fill f1.0 ;g
          .fill f1.0 ;b

start:  gpu 0         - Initialise Opengl

loop:   gpu 1         - Start frame

        gpu 3         - 2D Projection

        addi $1 $0 float

        lw $20 $1 0   - load x
        lw $21 $1 4   - load y

        lw $23 $1 8   - load rgb
        lw $24 $1 12   - load rgb
        lw $25 $1 16   - load rgb

        gpu 6         - Draw pixel

        gpu 8         - Present frame
        jmp loop
