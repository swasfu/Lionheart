#include "File.h"

#include <fstream>
#include <iostream>

std::string File::ReadFile(std::string filename)
{
	std::ifstream fileStream = GetInputStream(filename);
	fileStream.seekg(0, std::ios::end);
	size_t size = fileStream.tellg();
	std::string buffer(size, ' ');
	fileStream.seekg(0);
	fileStream.read(&buffer[0], size);
	fileStream.close();
	return buffer;
}

std::ifstream File::GetInputStream(std::string filename)
{
	std::ifstream stream(filename);

	if (stream.fail())
	{
		std::cout << filename << ": Failed to open file" << std::endl;
		return std::ifstream();
	}

	return stream;
}
