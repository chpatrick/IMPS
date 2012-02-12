#define temp1           $8
#define temp2           $9

#define floats          $3 

#define minusOneOffset  0
#define pointOneOffset  4
#define halfOffset      8
#define oneOffset       12
#define twoOffset       16
#define fiveOffset      20
#define tenOffset       24
#define fiftyOffset     28
#define hundredOffset   32
#define thousandOffset  36

jmp	    start

float:    .fill f-1.0
          .fill f0.1 
          .fill f0.5   
          .fill f1.0
          .fill f2.0
          .fill f5.0
          .fill f10.0
          .fill f50.0
          .fill f100.0
          .fill f1000.0

; $0  = 0
; $1  = xi
; $2  = yi
; $3  = floats

; $6  = xf
; $7  = yf
; $9 = temp1
; $9 = temp2
; $10 = xdiff
; $11 = ydiff


; $12 = lastTics
; $13 = currentTics
; $14 = rate

start:    gpu 0               - Initialise Opengl
          
          addi floats $0 float - Set up float list

          tics $12             - $13 = pastTics
          subi $12 $12 10
          lw temp1 floats thousandOffset
          fdiv $12 $12 temp1
          
loop:     tics $13            - $13 = currentTics     
          sub $14 $13 $12     - rate = new - old
          itof $14 $14        - rate = (float) rate
          add $12 $0 $13      - lastTics = tics               

          gpu 1               - Start frame
          gpu 2               - 3D Projection

          addi $1 $0 20       - xi = 100
          lw temp1 floats halfOffset
          fadd $10 $0 temp1    - xdiff = 0.5
          ;addi $10 $0 1       - xdiff = 0.5
          ;itof $10 $10        - xdiff = (float) xdiff

forx:     itof $6 $1          - xf = (float) xi
          fmul $6 $6 $10      - xf *= xdiff      

          addi $2 $0 20       - yi = 100
          lw temp1 floats halfOffset
          fadd $11 $0 temp1    - ydiff = 0.5
          ;itof $11 $11        - ydiff = (float) ydiff

fory:     itof $7 $2          - yf = (float) yi
          fmul $7 $7 $11      - yf *= ydiff    
          
          gpu 4               - Start poly
          
          lw temp1 floats halfOffset
          fadd $20 temp1 $0
          fadd $21 temp1 $0
          fadd $22 $0 $0

          fadd $20 $20 $6 - Add x offsets
          fadd $21 $21 $7 - Add y offsets

          lw temp1 floats fiveOffset
          fsub $20 $20 temp1  - x -= 5.0f
          fsub $21 $21 temp1  - y -= 5.0f

          lw temp1 floats oneOffset
      
          fadd $23 temp1 $0  - v.r
          fadd $24 temp1 $0   - v.g
          fadd $25 temp1 $0   - v.b

          ;colour work

          fadd $23 $20 $0
          lw temp1 floats halfOffset
          fadd $23 temp1 $23
          lw temp1 floats twoOffset
          fmul $23 $23 temp1 

          add temp1 $0 $13
          itof temp1 temp1
          lw temp2 floats hundredOffset
          fdiv temp1 temp1 temp2
          fadd $23 $23 temp1

          fsin $23 $23

          fadd $22 $23 $0 - Do z stuff

          gpu 5               - Draw vertex

          lw temp1 floats halfOffset

          fadd $20 temp1 $0
          fadd $21 $0 $0
          fadd $22 $0 $0
      
          fadd $20 $20 $6 - Add x offsets
          fadd $21 $21 $7 - Add y offsets

          lw temp1 floats fiveOffset
          fsub $20 $20 temp1  - x -= 5.0f
          fsub $21 $21 temp1  - y -= 5.0f

          lw temp1 floats oneOffset

          fadd $23 temp1 $0   - v.r
          fadd $24 temp1 $0   - v.g
          fadd $25 temp1 $0   - v.b

          ;colour work
  
          fadd $23 $20 $0
          ;;lw temp1 floats halfOffset
          ;;fsub $23 $23 temp1
          lw temp1 floats twoOffset
          fmul $23 $23 temp1 

          add temp1 $0 $13
          itof temp1 temp1
          lw temp2 floats hundredOffset
          fdiv temp1 temp1 temp2
          fadd $23 $23 temp1

          fsin $23 $23

          fadd $22 $23 $0 - Do z stuff

          gpu 5               - Draw vertex

          lw temp1 floats halfOffset

          fadd $20 $0 $0
          fadd $21 $0 $0
          fadd $22 $0 $0
    
          fadd $20 $20 $6 - Add x offsets
          fadd $21 $21 $7 - Add y offsets

          lw temp1 floats fiveOffset
          fsub $20 $20 temp1  - x -= 5.0f
          fsub $21 $21 temp1  - y -= 5.0f

          lw temp1 floats oneOffset

          fadd $23 temp1 $0   - v.r
          fadd $24 temp1 $0   - v.g
          fadd $25 temp1 $0   - v.b

          ;colour work

          fadd $23 $20 $0
          lw temp1 floats halfOffset
          fadd $23 $23 temp1
          lw temp1 floats twoOffset
          fmul $23 $23 temp1 

          ;time
          add temp1 $0 $13
          itof temp1 temp1
          lw temp2 floats hundredOffset
          fdiv temp1 temp1 temp2
          fadd $23 $23 temp1

          fsin $23 $23

          fadd $22 $23 $0 - Do z stuff

          gpu 5               - Draw vertex

          lw temp1 floats halfOffset

          fadd $20 $0 $0
          fadd $21 temp1 $0
          fadd $22 $0 $0

          fadd $20 $20 $6 - Add x offsets
          fadd $21 $21 $7 - Add y offsets

          lw temp1 floats fiveOffset
          fsub $20 $20 temp1  - x -= 5.0f
          fsub $21 $21 temp1  - y -= 5.0f

          lw temp1 floats oneOffset

          fadd $23 temp1 $0   - v.r
          fadd $24 temp1 $0   - v.g
          fadd $25 temp1 $0   - v.b

          ;colour work

          fadd $23 $20 $0
          lw temp1 floats halfOffset
          fadd $23 $23 temp1
          fadd $23 $23 temp1
          lw temp1 floats twoOffset
          fmul $23 $23 temp1 

          add temp1 $0 $13
          itof temp1 temp1
          lw temp2 floats hundredOffset
          fdiv temp1 temp1 temp2
          fadd $23 $23 temp1

          fsin $23 $23

          fadd $22 $23 $0 - Do z stuff
          
          gpu 5               - Draw vertex

          gpu 7               - End polygon
        

          subi $2 $2 1        - xi -= 1
          blt $2 $0 endloopy  - if(x < 0) break;
          jmp fory    

endloopy:

          subi $1 $1 1        - xi -= 1
          blt $1 $0 endloopx  - if(x < 0) break;
          jmp forx          

endloopx: gpu 8               - Present frame
          jmp loop 
