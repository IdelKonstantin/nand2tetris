#include "code_writer.h"

CodeWriter::CodeWriter() {}

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
