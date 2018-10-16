/**
C++ Auxiliar.h
Purpose: Header of Auxiliar functions

@author Alejandro Molina Quesada
@version 1.0
@date 12/10/2018
*/

#ifndef INCLUDE_VECTOR
#include <vector>
#endif
#ifndef FILE_STREAMS
#include <fstream>
#endif

/// <summary>
/// Read data from a file.
/// 
/// 
/// </summary>
static std::vector<char> readFile(const std::string& filename) {
	//Reading file.
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	if(!file.is_open())
		throw std::runtime_error("failed to open file!");
	//Creating buffer.
	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	//Closing file.
	file.close();

	return buffer;
}