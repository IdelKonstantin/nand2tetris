#include "VM_translator.h"
#include "pathcontentanalyzer.h"

namespace fs = std::experimental::filesystem;

int main(int argc, char** argv) {

    fs::path path = fs::current_path();

    if (argc > 1) {
        path = argv[1];
    }

    pathContentAnalyzer pca{path};

    auto files = pca.getFilesInDirectory();
    auto outputFilename = pca.getOutputFilename();

    if(files.empty()) {

        std::cerr << "No *.vm files found..." << std::endl;
        return -1;
    }

    VMtranslator vmt{outputFilename, files};

    if(!vmt.init()) {
        return -1;
    }

    vmt.translateAndSaveAll();

    return 0;
}
