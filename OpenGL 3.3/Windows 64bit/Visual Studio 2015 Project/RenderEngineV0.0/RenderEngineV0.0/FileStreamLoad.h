#pragma once
/**
C++ FileStreamLoad.h
Purpose: Header of Model class

@author URJC (Universidad Rey Juan Carlos)
@version 1.0
@date 04/03/2018
*/

#include <windows.h>
#include <fstream>
//#include <FreeImage.h>
#define _CRT_SECURE_DEPRECATE_MEMORY
#include <memory.h>

/// <summary>
/// This function convert the content of a file into an unsigned char array.
/// <returns>The unsigned char array.</returns> 
/// </summary>
char *loadStringFromFile(const char *fileName, unsigned int &fileLen);
/// <summary>
/// This function convert the content of an image into an unsigned char array.
/// <returns>The unsigned char array.</returns> 
/// </summary>
//unsigned char *loadTexture(const char* fileName, unsigned int &w, unsigned int &h);