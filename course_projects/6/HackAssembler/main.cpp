#include "assembler.h"

int main(int argc, char** argv) {

    if(argc < 2) {

        std::cout << "Wrong number of args. Usage:" << std::endl;
        std::cout << "./HackAssembler *.asm" << std::endl;
        return -1;
    }

    Assembler assembler{argv[1]};

    if(!assembler.init()) {
        return -1;
    }

    assembler.translateAndSave();
    assembler.showSymbolTable();

    return 0;
}
