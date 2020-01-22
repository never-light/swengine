#include <string>

class FileUtills {
public:
    static bool isFileExists(const std::string& path);
    static std::string readFile(const std::string& path);
};
