// Bootstrap code
@256
D=A
@SP
M=D
// call Sys.init 0
@after-finishing-all-tasks
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
@5
D=A
@0
D=D+A
@SP
D=M-D
@ARG
M=D
@SP
D=M
@LCL
M=D
@Sys.init
0;JMP
(after-finishing-all-tasks)// function Sys.init 0
(Sys.init)

// push constant 4
@4
D=A
@SP
A=M
M=D
@SP
M=M+1

// call Main.fibonacci 1
//push return address
@Main.fibonacci.retAddress.0
D=A
@SP
A=M
M=D
@SP
M=M+1
//push LCL
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
//push ARG
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
//push THIS
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
//push THAT
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
//calculate ARG for called function
@5
D=A
@1
D=D+A
@SP
D=M-D
@ARG
M=D
//calculate LCL for called function
@SP
D=M
@LCL
M=D
@Main.fibonacci
0;JMP
(Main.fibonacci.retAddress.0)

// label END
(Main.END)

// goto END
@Main.END
0;JMP

// function Main.fibonacci 0
(Main.fibonacci)

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

// push constant 2
@2
D=A
@SP
A=M
M=D
@SP
M=M+1

// lt
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
@labelLTTrue.Main0
D;JLT
@SP
A=M-1
M=0
@labelLTEnd.Main0
0;JMP
(labelLTTrue.Main0)
@SP
A=M-1
M=-1
(labelLTEnd.Main0)

// if-goto N_LT_2
@SP
AM=M-1
D=M
@Main.N_LT_2
D;JNE

// goto N_GE_2
@Main.N_GE_2
0;JMP

// label N_LT_2
(Main.N_LT_2)

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

// label N_GE_2
(Main.N_GE_2)

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

// push constant 2
@2
D=A
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

// call Main.fibonacci 1
//push return address
@Main.fibonacci.retAddress.1
D=A
@SP
A=M
M=D
@SP
M=M+1
//push LCL
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
//push ARG
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
//push THIS
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
//push THAT
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
//calculate ARG for called function
@5
D=A
@1
D=D+A
@SP
D=M-D
@ARG
M=D
//calculate LCL for called function
@SP
D=M
@LCL
M=D
@Main.fibonacci
0;JMP
(Main.fibonacci.retAddress.1)

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

// push constant 1
@1
D=A
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

// call Main.fibonacci 1
//push return address
@Main.fibonacci.retAddress.2
D=A
@SP
A=M
M=D
@SP
M=M+1
//push LCL
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
//push ARG
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
//push THIS
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
//push THAT
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
//calculate ARG for called function
@5
D=A
@1
D=D+A
@SP
D=M-D
@ARG
M=D
//calculate LCL for called function
@SP
D=M
@LCL
M=D
@Main.fibonacci
0;JMP
(Main.fibonacci.retAddress.2)

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

