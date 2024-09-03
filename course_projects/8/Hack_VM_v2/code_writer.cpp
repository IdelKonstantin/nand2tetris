#include "code_writer.h"

CodeWriter::CodeWriter() {}

std::string CodeWriter::writeBootStrap() const {

    const std::string bootCommand = R"boot(// Bootstrap code
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
(after-finishing-all-tasks))boot";

    return bootCommand;
}

std::string CodeWriter::writeArithmetic(const std::string& opName, const std::string& currentWorkingModule) {

    m_ss.str(std::string());

    if(opName == "add" || opName == "sub") {

        m_ss << "@SP" << '\n';
        m_ss << "A=M-1" << '\n';
        m_ss << "D=M" << '\n';
        m_ss << "@R13" << '\n';
        m_ss << "M=D" << '\n';
        m_ss << "@SP" << '\n';
        m_ss << "M=M-1" << '\n';
        m_ss << "@SP" << '\n';
        m_ss << "A=M-1" << '\n';
        m_ss << "D=M" << '\n';
        m_ss << "@R13" << '\n';
        m_ss << "D=D" << (opName == "add" ? '+' : '-') << "M" << '\n';
        m_ss << "@SP" << '\n';
        m_ss << "A=M-1" << '\n';
        m_ss << "M=D" << '\n';
    }

    else if(opName == "neg") {

        m_ss << "@SP" << '\n';
        m_ss << "A=M-1" << '\n';
        m_ss << "M=-M" << '\n';
    }
    else if(opName == "eq" || opName == "gt" || opName == "lt") {

        std::string labelNameTrue;
        std::string labelNameEnd;

        m_ss << "@SP" << '\n';
        m_ss << "A=M-1" << '\n';
        m_ss << "D=M" << '\n';
        m_ss << "@R13" << '\n';
        m_ss << "M=D" << '\n';
        m_ss << "@SP" << '\n';
        m_ss << "M=M-1" << '\n';
        m_ss << "@SP" << '\n';
        m_ss << "A=M-1" << '\n';
        m_ss << "D=M" << '\n';
        m_ss << "@R13" << '\n';
        m_ss << "D=D-M" << '\n';

        if(opName == "eq") {

            labelNameTrue = {"labelEQTrue." + currentWorkingModule + std::to_string(m_eqCounter)};
            labelNameEnd = {"labelEQEnd." + currentWorkingModule + std::to_string(m_eqCounter)};

            m_ss << '@' << labelNameTrue << '\n';
            m_ss << "D;JEQ" << '\n';
            m_ss << "@SP" << '\n';
            m_ss << "A=M-1" << '\n';
            m_ss << "M=0" << '\n';
            m_ss << '@' << labelNameEnd << '\n';
            m_ss << "0;JMP" << '\n';
            m_ss << '(' << labelNameTrue << ')' << '\n';
            m_ss << "@SP" << '\n';
            m_ss << "A=M-1" << '\n';
            m_ss << "M=-1" << '\n';
            m_ss << '(' << labelNameEnd << ')' << '\n';
            m_eqCounter++;
        }
        else if(opName == "gt") {

            labelNameTrue = {"labelGTTrue." + currentWorkingModule + std::to_string(m_gtCounter)};
            labelNameEnd = {"labelGTEnd." + currentWorkingModule + std::to_string(m_gtCounter)};

            m_ss << '@' << labelNameTrue << '\n';
            m_ss << "D;JGT" << '\n';
            m_ss << "@SP" << '\n';
            m_ss << "A=M-1" << '\n';
            m_ss << "M=0" << '\n';
            m_ss << '@' << labelNameEnd << '\n';
            m_ss << "0;JMP" << '\n';
            m_ss << '(' << labelNameTrue << ')' << '\n';
            m_ss << "@SP" << '\n';
            m_ss << "A=M-1" << '\n';
            m_ss << "M=-1" << '\n';
            m_ss << '(' << labelNameEnd << ')' << '\n';
            m_gtCounter++;
        }
        else if(opName == "lt") {

            labelNameTrue = {"labelLTTrue." + currentWorkingModule + std::to_string(m_ltCounter)};
            labelNameEnd = {"labelLTEnd." + currentWorkingModule + std::to_string(m_ltCounter)};

            m_ss << '@' << labelNameTrue << '\n';
            m_ss << "D;JLT" << '\n';
            m_ss << "@SP" << '\n';
            m_ss << "A=M-1" << '\n';
            m_ss << "M=0" << '\n';
            m_ss << '@' << labelNameEnd << '\n';
            m_ss << "0;JMP" << '\n';
            m_ss << '(' << labelNameTrue << ')' << '\n';
            m_ss << "@SP" << '\n';
            m_ss << "A=M-1" << '\n';
            m_ss << "M=-1" << '\n';
            m_ss << '(' << labelNameEnd << ')' << '\n';
            m_ltCounter++;
        }
    }
    else if(opName == "and" || opName == "or") {

        m_ss << "@SP" << '\n';
        m_ss << "A=M-1" << '\n';
        m_ss << "D=M" << '\n';
        m_ss << "@R13" << '\n';
        m_ss << "M=D" << '\n';
        m_ss << "@SP" << '\n';
        m_ss << "M=M-1" << '\n';
        m_ss << "@SP" << '\n';
        m_ss << "A=M-1" << '\n';
        m_ss << "D=M" << '\n';
        m_ss << "@R13" << '\n';
        m_ss << "D=D" << (opName == "and" ? '&' : '|') << "M" << '\n';
        m_ss << "@SP" << '\n';
        m_ss << "A=M-1" << '\n';
        m_ss << "M=D" << '\n';
    }
    else if(opName == "not") {

        m_ss << "@SP" << '\n';
        m_ss << "A=M-1" << '\n';
        m_ss << "M=!M" << '\n';
    }

    return m_ss.str();
}

std::string CodeWriter::writePushPop(Parser::command_t commandType, const std::string& segment, const std::string& index, const std::string& currentWorkingModule) {

    m_ss.str(std::string());

    switch(commandType) {

        case Parser::C_PUSH:
            if(segment == "constant") {

                m_ss << "@" << index << '\n';
                m_ss << "D=A" << '\n';
                m_ss << "@SP" << '\n';
                m_ss << "A=M" << '\n';
                m_ss << "M=D" << '\n';
                m_ss << "@SP" << '\n';
                m_ss << "M=M+1" << '\n';
            }
            else if (segment == "static") {

                m_ss << "@" << currentWorkingModule << '.' << index << '\n';
                m_ss << "D=M" << '\n';
                m_ss << "@SP" << '\n';
                m_ss << "A=M" << '\n';
                m_ss << "M=D" << '\n';
                m_ss << "@SP" << '\n';
                m_ss << "M=M+1" << '\n';
            }
            else if (segment == "temp") {

                m_ss << "@5" << '\n';
                m_ss << "D=A" << '\n';
                m_ss << "@" << index << '\n';
                m_ss << "A=D+A" << '\n';
                m_ss << "D=M" << '\n';
                m_ss << "@SP" << '\n';
                m_ss << "A=M" << '\n';
                m_ss << "M=D" << '\n';
                m_ss << "@SP" << '\n';
                m_ss << "M=M+1" << '\n';
            }
            else if (segment == "pointer") {

                m_ss << "@3" << '\n';
                m_ss << "D=A" << '\n';
                m_ss << "@" << index << '\n';
                m_ss << "A=D+A" << '\n';
                m_ss << "D=M" << '\n';
                m_ss << "@SP" << '\n';
                m_ss << "A=M" << '\n';
                m_ss << "M=D" << '\n';
                m_ss << "@SP" << '\n';
                m_ss << "M=M+1" << '\n';
            }
            else { //local, argument, this and that

                m_ss << "@" << m_segNicknames.at(segment) << '\n';
                m_ss << "D=M" << '\n';
                m_ss << "@" << index << '\n';
                m_ss << "A=D+A" << '\n';
                m_ss << "D=M" << '\n';
                m_ss << "@SP" << '\n';
                m_ss << "A=M" << '\n';
                m_ss << "M=D" << '\n';
                m_ss << "@SP" << '\n';
                m_ss << "M=M+1" << '\n';
            }
        break;

        case Parser::C_POP:

            if(segment == "pointer" || segment == "temp") {

                m_ss << "@" << (segment == "pointer" ? '3' : '5') << '\n';
                m_ss << "D=A" << '\n';
                m_ss << "@" << index << '\n';
                m_ss << "D=D+A" << '\n';
                m_ss << "@R13" << '\n';
                m_ss << "M=D" << '\n';
                m_ss << "@SP" << '\n';
                m_ss << "A=M-1" << '\n';
                m_ss << "D=M" << '\n';
                m_ss << "@R13" << '\n';
                m_ss << "A=M" << '\n';
                m_ss << "M=D" << '\n';
                m_ss << "@SP" << '\n';
                m_ss << "M=M-1" << '\n';
            }
            else if(segment == "static") {

                m_ss << "@SP" << '\n';
                m_ss << "A=M-1" << '\n';
                m_ss << "D=M" << '\n';
                m_ss << "@" << currentWorkingModule << '.' << index << '\n';
                m_ss << "M=D" << '\n';
                m_ss << "@SP" << '\n';
                m_ss << "M=M-1" << '\n';
            }
            else { //local, argument, this and that

                m_ss << "@" << m_segNicknames.at(segment) << '\n';
                m_ss << "D=M" << '\n';
                m_ss << "@" << index << '\n';
                m_ss << "D=D+A" << '\n';
                m_ss << "@R13" << '\n';
                m_ss << "M=D" << '\n';
                m_ss << "@SP" << '\n';
                m_ss << "A=M-1" << '\n';
                m_ss << "D=M" << '\n';
                m_ss << "@R13" << '\n';
                m_ss << "A=M" << '\n';
                m_ss << "M=D" << '\n';
                m_ss << "@SP" << '\n';
                m_ss << "M=M-1" << '\n';
            }

        break;
    }

    return m_ss.str();
}

std::string CodeWriter::writeLable(const std::string& lable, const std::string& currentWorkingModule) {

    m_ss.str(std::string());
    m_ss << '(' << currentWorkingModule << '.' << lable << ")\n";
    return m_ss.str();
}

std::string CodeWriter::writeGoto(const std::string& lable, const std::string& currentWorkingModule) {

    m_ss.str(std::string());
    m_ss << '@' << currentWorkingModule << '.' << lable << '\n';
    m_ss << "0;JMP\n";
    return m_ss.str();
}

std::string CodeWriter::writeIf(const std::string& lable, const std::string& currentWorkingModule) {

    m_ss.str(std::string());

    m_ss << "@SP\n";
    m_ss << "AM=M-1\n";
    m_ss << "D=M\n";
    m_ss << '@' << currentWorkingModule << '.' << lable << '\n';
    m_ss << "D;JNE\n";

    return m_ss.str();
}

std::string CodeWriter::writeFunction(const std::string& funcName, const std::string& nArgs, const std::string& currentWorkingModule) {

    std::string funcCode{'(' + funcName + ")\n"};

    for(int i =0; i < std::stoi(nArgs); i++) {
        funcCode += writePushPop(Parser::C_PUSH, "constant", "0", currentWorkingModule);
    }

    return funcCode;
}

std::string CodeWriter::writeCall(const std::string& funcName, const std::string& nArgs) {

    m_ss.str(std::string());

    m_ss << "//push return address\n";
    m_ss << '@' << funcName << '.' << "retAddress" << '.' << m_retAddrCounter << '\n';
    m_ss << "D=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
    m_ss << "//push LCL\n";
    m_ss << "@LCL\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
    m_ss << "//push ARG\n";
    m_ss << "@ARG\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
    m_ss << "//push THIS\n";
    m_ss << "@THIS\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
    m_ss << "//push THAT\n";
    m_ss << "@THAT\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
    m_ss << "//calculate ARG for called function\n";
    m_ss << "@5\nD=A\n@" << nArgs << "\nD=D+A\n@SP\nD=M-D\n@ARG\nM=D\n";
    m_ss << "//calculate LCL for called function\n";
    m_ss << "@SP\nD=M\n@LCL\nM=D\n";
    m_ss << '@' << funcName << '\n';
    m_ss << "0;JMP\n";
    m_ss << '(' << funcName << '.' << "retAddress" << '.' << m_retAddrCounter << ")\n";
    m_retAddrCounter++;
    return m_ss.str();
}

std::string CodeWriter::writeReturn() {

    m_ss.str(std::string());

    const std::string retCommand = R"ret(@LCL
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
0;JMP)ret";

    m_ss << retCommand << '\n';
    return m_ss.str();
}
