#pragma once
#include <string>
#include <vector>
class Utilities
{
	public:
        static std::vector<std::string> splitString(
            const std::string& s,
            const std::string& delimiter,
            char glueChar,
            bool removeGlueChar
        );
};

