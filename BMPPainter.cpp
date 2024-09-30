#include "BMPPainter.h"
#define BMPFORMAT 0x4d42


void BMPPainter::openBMP(const std::string& filename) {
	if (!_bmpFile.is_open()) {

		_bmpFile.open(filename, std::ios_base::in);
		if (_bmpFile.fail()) {
			std::cout << "error: file is not found" << "\n";
		}

		WORD& bfType = this->_bfh.bfType;
		_bmpFile.read(reinterpret_cast<char*>(&bfType),sizeof(bfType));

		if(bfType == BMPFORMAT) {
			auto initField = [this](auto& field) { _bmpFile.read(reinterpret_cast<char*>(&field), sizeof(field)); };
			
			// initialize fields of BITFILEHEADER
			initField(_bfh.bfSize);
			initField(_bfh.bfReserved1);
			initField(_bfh.bfReserved2);
			initField(_bfh.bfOffBits);

			//initialize fields of BITINFOHEADER
			initField(_bih.biSize);
			initField(_bih.biWidth);
			initField(_bih.biHeight);
			initField(_bih.biPlanes);
			initField(_bih.biBitCount);
			initField(_bih.biCompression);
			initField(_bih.biSizeImage);
			initField(_bih.biXPelsPerMeter);
			initField(_bih.biYPelsPerMeter);
			initField(_bih.biClrUsed);
			initField(_bih.biClrImportant);
			

			_bmpFile.seekg(_bfh.bfOffBits);
			if(_bih.biBitCount >= 24) {


				uint32_t white = _bih.biBitCount == 24 ? 0x00FFFFFF : 0xFFFFFFFF;
				uint32_t black = _bih.biBitCount == 24 ? 0x00000000 : 0xFF000000; 
				int8_t shift = _bih.biBitCount == 24 ? 3 : 4;
				uint32_t currentRgba = 0;

				_data.resize(_bih.biHeight);
				for(auto& w : _data) w.resize(_bih.biWidth);

				if( ( _bih.biWidth % 4 == 0 ) || _bih.biBitCount == 32) {

					for(std::size_t h = 0; h != _bih.biHeight; h++) {
						for(std::size_t w = 0;w != _bih.biWidth; w++) {
							_bmpFile.read(reinterpret_cast<char*>(&currentRgba),shift);

							if(currentRgba == white) _data[h][w] = 1;
							else if(currentRgba == black) _data[h][w] = 0;
							else {
								std::cout << "error: image must contain only black and white colors" << "\n";
								this->closeBMP();
								std::abort();
							}
							
						}
						
					}
				}
				else {
					//Calculate extra bytes numbers for not dividable width
					std::size_t trueSizeImage =  _bih.biWidth * _bih.biHeight * 3;
					int8_t extraBytesNum = 1;
					auto tempSize = trueSizeImage;

					tempSize += _bih.biHeight * extraBytesNum;
					while( _bih.biSizeImage - tempSize != 0 ) {
						extraBytesNum++;
						tempSize = trueSizeImage;
						tempSize += _bih.biHeight * extraBytesNum;
					}

					for(std::size_t h = 0; h != _bih.biHeight; h++) {
						for(std::size_t w = 0;w != _bih.biWidth; w++) {
							_bmpFile.read(reinterpret_cast<char*>(&currentRgba),shift);

							if(currentRgba == white) _data[h][w] = 1;
							else if(currentRgba == black) _data[h][w] = 0;
							else {
								std::cout << "error: image must contain only black and white colors" << "\n";
								this->closeBMP();
								std::abort();
							}
						}
						_bmpFile.seekg(_bmpFile.tellg() + std::streampos(extraBytesNum));
					}
				}
				
			}
			else {
				std::cout << "error: bitCount not supported" << "\n";
			}
		}
		else {
			std::cout << "error: wrong type format of file" << "\n";
		}
	}
	else {
		std::cout << "error: trying to open new file while current is not closed" << "\n";
	}
}


void BMPPainter::displayBMP() {
	if(_data.size() != 0) {
		std::size_t h = _data.size()-1;
		
		while(h > 0) {
			for(std::size_t pixel : _data[h]) {
				//if pixel == 0 then we print black
				if(pixel == 0) std::cout << "\033[40;30m  \033[0m";
				else std::cout << "\033[47;30m  \033[0m";
			}
			std::cout << "\n";
			h--;
		}
	}
	else {
		std::cout << "nothing to display" << "\n";
	}
}

void BMPPainter::closeBMP() {
	_bmpFile.close();
	this->_data.clear();
	this->_data.shrink_to_fit();
	_bfh = {};
	_bih = {};
}
