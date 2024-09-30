#pragma once
#include <iostream>
#include <fstream>
#include "BMPStructures.h"
#include <vector>

class BMPPainter {
public:
	BMPPainter() = default;

	void openBMP(const std::string& filename);

	void displayBMP();

	void closeBMP();



private:
	std::vector<std::vector<int8_t>>		_data;
	std::ifstream			_bmpFile;
	BITMAPFILEHEADER 		_bfh;
	BITMAPINFOHEADER 		_bih;

};