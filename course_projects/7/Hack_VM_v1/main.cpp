#include "VM_translator.h"

int main(int argc, char** argv) {

    if(argc < 2) {

        std::cout << "Wrong number of args. Usage:" << std::endl;
        std::cout << "./VMtranslator_v1 *.vm" << std::endl;
        return -1;
    }

    VMtranslator vmt{argv[1]};

    if(!vmt.init()) {
        return -1;
    }

    vmt.translateAndSave();

    return 0;
}
