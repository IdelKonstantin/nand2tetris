#ifndef PATHCONTENTANALYZER_H
#define PATHCONTENTANALYZER_H

#include <experimental/filesystem>
#include <vector>

#define VM_EXTENSION ".vm"

#include <iostream>

namespace fs = std::experimental::filesystem;

class pathContentAnalyzer {

private:

    fs::path m_path;
    std::vector<std::string> m_files;
    std::string m_outputFilename;

public:
    pathContentAnalyzer(fs::path path);

    std::vector<std::string>& getFilesInDirectory();
    std::string getOutputFilename() const;
};

#endif // PATHCONTENTANALYZER_H
