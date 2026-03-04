#include <fstream>
#include <sstream>

#include "FileManager.h"
#include "../Exceptions/AllExceptions.h"

std::istream* FileManager::openInputFile(const std::string& filename) {
    std::ifstream* input = new std::ifstream(filename);
    if (!*input) {
        delete input;
        throw FileException(filename + " - can't open file", -1);
    }
    return input;
}

std::ostream* FileManager::openOutputFile(const std::string& filename, const bool& append) {
    std::ofstream* output = new std::ofstream(filename, (append ? std::ios::app : std::ios::trunc) | std::ios::out);
    if (!*output) {
        delete output;
        throw FileException(filename + " - can't open file", -1);
    }
    return output;
}

bool FileManager::exists(const std::string& filename) {
    std::ifstream ifile(filename);
    return ifile.good();
}

void FileManager::touchFile(const std::string& filename) {
    if (exists(filename)) {
        throw FileException(filename + " - already exists", -1);
    }
    std::ofstream ofile(filename);
    if (!ofile.good()) {
        throw FileException("couldn't touch - " + filename, -1);
    }
}

void FileManager::truncateFile(const std::string& filename) {
    if (!exists(filename)) {
        throw FileException(filename + " - doesn't exist", -1);
    }
    std::ofstream ofile(filename, std::ios::trunc);
    if (!ofile.good()) {
        throw FileException("couldn't truncate - " + filename, -1);
    }
}

void FileManager::removeFile(const std::string& filename) {
    if (!exists(filename)) {
        throw FileException(filename + " - doesn't exist", -1);
    }
    if (std::remove(filename.c_str())) {
        throw FileException("couldn't remove - " + filename, -1);
    }
}