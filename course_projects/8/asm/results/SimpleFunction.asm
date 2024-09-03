// function SimpleFunction.test 2
(SimpleFunction.test)
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@0
D=A
@SP
A=M
M=D
@SP
M=M+1

// push local 0
@LCL
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// push local 1
@LCL
D=M
@1
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// add
@SP
A=M-1
D=M
@R13
M=D
@SP
M=M-1
@SP
A=M-1
D=M
@R13
D=D+M
@SP
A=M-1
M=D

// not
@SP
A=M-1
M=!M

// push argument 0
@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// add
@SP
A=M-1
D=M
@R13
M=D
@SP
M=M-1
@SP
A=M-1
D=M
@R13
D=D+M
@SP
A=M-1
M=D

// push argument 1
@ARG
D=M
@1
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

// sub
@SP
A=M-1
D=M
@R13
M=D
@SP
M=M-1
@SP
A=M-1
D=M
@R13
D=D-M
@SP
A=M-1
M=D

// return
@LCL
D=M
@R13
M=D

//R14 <--- RET = *(FRAME - 5)
@R13
D=M
@5
A=D-A
D=M
@R14
M=D

//*ARG = pop()
@SP
A=M-1
D=M
@ARG
A=M
M=D

//SP = ARG + 1
@ARG
D=M+1
@SP
M=D

//THAT = *(FRAME - 1)
@R13
A=M-1
D=M
@THAT
M=D

//THIS = *(FRAME - 2)
@R13
D=M
@2
A=D-A
D=M
@THIS
M=D

//ARG = *(FRAME - 3)
@R13
D=M
@3
A=D-A
D=M
@ARG
M=D

//LCL = *(FRAME - 4)
@R13
D=M
@4
A=D-A
D=M
@LCL
M=D

//goto RET
@R14
A=M
0;JMP

