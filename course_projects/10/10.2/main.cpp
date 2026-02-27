#include "JackAnalyzer.h"
#include <iostream>
#include <experimental/filesystem>
#include <vector>
#include <thread>

namespace fs = std::experimental::filesystem;

void processFile(const fs::path& filePath) {
    try {
        JackAnalyzer analyzer(filePath.string());
        analyzer.writeXML();
        std::cout << "Analyzed: " << filePath.filename() << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error processing " << filePath << ": " << e.what() << "\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: analyzer <file.jack | directory>\n";
        return 1;
    }

    fs::path inputPath(argv[1]);

    if (!fs::exists(inputPath)) {
        std::cerr << "Path does not exist: " << inputPath << "\n";
        return 1;
    }

    std::vector<fs::path> jackFiles;

    if (fs::is_regular_file(inputPath) && inputPath.extension() == ".jack") {
        processFile(inputPath);
        return 0;
    }

    if (fs::is_directory(inputPath)) {
        for (const auto& entry : fs::directory_iterator(inputPath)) {
            if (fs::is_regular_file(entry.path()) && entry.path().extension() == ".jack")
                jackFiles.push_back(entry.path());
        }
    }

    if (jackFiles.empty()) {
        std::cerr << "No .jack files found.\n";
        return 1;
    }

    std::vector<std::thread> threads;
    for (const auto& file : jackFiles)
        threads.emplace_back(processFile, file);
    
    for (auto& t : threads)
        if (t.joinable()) t.join();

    std::cout << "Done. Processed " << jackFiles.size() << " file(s).\n";
    return 0;
}