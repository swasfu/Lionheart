#pragma once

#include <string>
#include <fstream>

class File
{
public:
	static std::string ReadFile(std::string);
	static std::ifstream GetInputStream(std::string);
};