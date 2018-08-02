#include "files.h"

#include <filesystem>

namespace fs = std::experimental::filesystem;

bool FilesUtils::isExists(const std::string& filename)
{
	return fs::exists(filename);
}
