#include "precompiled.h"
#pragma hdrstop

#include "strings.h"

#include <sstream>
#include <algorithm>

std::vector<std::string> StringUtils::split(const std::string& str, char delimiter)
{
   std::vector<std::string> entries;
   std::string entry;

   std::istringstream stringStream(str);

   while (std::getline(stringStream, entry, delimiter)) {
      entries.push_back(entry);
   }

   return entries;
}

std::string StringUtils::toLowerCase(const std::string& str)
{
    std::string result;

    for (auto ch : str) {
        result.push_back(static_cast<char>(std::tolower(ch)));
    }

    return result;
}

std::string StringUtils::regexReplace(const std::string& expression, std::string str,
    std::function<std::string (const std::smatch&)> callback)
{
    std::regex regexp(expression);
    std::smatch match;

    std::string result = "";

    while (regex_search(str, match, regexp)) {
        result += std::string(match.prefix()) + callback(match);
        str = match.suffix();
    }

    result += str;

    return result;
}

std::string StringUtils::replace(std::string source,
                                 const std::string& pattern,
                                 const std::string& replacement)
{
    size_t position = source.find(pattern);

    while(position != std::string::npos) {
        source.replace(position, pattern.length(), replacement);
        position = source.find(pattern, position + replacement.length());
    }

    return source;
}
