#include "pathcontentanalyzer.h"

#include <algorithm>

pathContentAnalyzer::pathContentAnalyzer(fs::path path) : m_path(path) {}

std::vector<std::string>& pathContentAnalyzer::getFilesInDirectory() {

    if (fs::exists(m_path)) {

        m_outputFilename = m_path.stem().string() + std::string(".asm");

        if(fs::is_directory(m_path)) {

            for (const auto& entry : fs::recursive_directory_iterator(m_path)) {

                if (fs::is_regular_file(entry) && entry.path().extension() == VM_EXTENSION) {
                    m_files.push_back(entry.path().string());
                }
            }
        }
        else if(fs::is_regular_file(m_path)) {

            if(m_path.extension() == VM_EXTENSION) {
                m_files.push_back(m_path.string());
            }
        }
    }

    int index{0};

    for(const auto& path : m_files) {
        if(path.find("Sys.vm") != std::string::npos) {
            break;
        }
        index++;
    }

    if(index != 0) {
        std::swap(m_files[0], m_files[index]);
    }

    return m_files;
}

std::string pathContentAnalyzer::getOutputFilename() const {

    return m_outputFilename;
}
