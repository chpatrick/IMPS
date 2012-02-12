jmp	    start

vertex1:  .fill f1.0
          .fill f1.0
          .fill f1.0

vertex2:  .fill f1.0
          .fill f0.0
          .fill f1.0

vertex3:  .fill f0.0
          .fill f1.0
          .fill f1.0

colour1:  .fill f0.7
          .fill f0.2
          .fill f0.3

colour2:  .fill f0.5
          .fill f0.5
          .fill f0.5

colour3:  .fill f0.1
          .fill f0.5
          .fill f0.0

start:  gpu 0         - Initialise Opengl

        addi $1 $0 vertex1 

        addi $2 $0 vertex2 

        addi $3 $0 vertex3 

        addi $4 $0 colour1
				addi $5 $0 colour2   
				addi $6 $0 colour3   

loop:   gpu 1         - Start frame

        gpu 2         - 3D Projection

        gpu 4         - Start poly

        lw $20 $1 0   - Set x
        lw $21 $1 4   - Set y
        lw $22 $1 8   - Set z

        lw $23 $4 0   - Set r
        lw $24 $4 4   - Set g
        lw $25 $4 8   - Set b

        gpu 5         - Draw vertex

        lw $20 $2 0   - Set x
        lw $21 $2 4   - Set y
        lw $22 $2 8   - Set z

        lw $23 $5 0   - Set r
        lw $24 $5 4   - Set g
        lw $25 $5 8   - Set b
        gpu 5         - Draw vertex

        lw $20 $3 0   - Set x
        lw $21 $3 4   - Set y
        lw $22 $3 8   - Set z

        lw $23 $6 0   - Set r
        lw $24 $6 4   - Set g
        lw $25 $6 8   - Set b
        gpu 5         - Draw vertex

        gpu 7         - End polygon

        gpu 8         - Present frame
        jmp loop
