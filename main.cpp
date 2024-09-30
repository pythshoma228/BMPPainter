#include <iostream>
#include "BMPPainter.h"


int main(int argc, char** argv) {
	if (argc != 2) {
		std::cout << "error: invalid numbers of arguments" << "\n";
		return EXIT_FAILURE;
	}
	
	BMPPainter drawer;
	drawer.openBMP(argv[1]);
	drawer.displayBMP();
	drawer.closeBMP();

	return EXIT_SUCCESS;
}