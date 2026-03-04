#ifndef FileManager_h
#define FileManager_h

#include <string>

// throws FileException if it doesn't succeed in any requested action
namespace FileManager {
    std::istream* openInputFile(const std::string& filename);
    std::ostream* openOutputFile(const std::string& filename, const bool& append);
    bool exists(const std::string& filename);
    void touchFile(const std::string& filename);
    void truncateFile(const std::string& filename);
    void removeFile(const std::string& filename);
};

#endif