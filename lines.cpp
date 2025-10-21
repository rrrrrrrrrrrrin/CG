#include <iostream>
#include <string>
#include "rgbimage.h"

int main(int argc, char* argv[]) {
	if (argc < 3) {
		return 1;
	}

	const char* input = argv[1];
	const char* output = argv[2];

	std::ifstream inFile(input);
	if (!inFile) {
		return 2;
	}

	try
	{
		int width = 0;
		int height = 0;

		inFile >> width;
		inFile >> height;

		rgbImg img(width, height);
		RGB white{ 255, 255, 255 };

		int x1 = 0;
		int y1 = 0;
		int x2 = 0;
		int y2 = 0;

		int ch;
		while ((ch = inFile.get()) != EOF)
		{
			inFile >> x1;
			inFile >> y1;
			inFile >> x2;
			inFile >> y2;

			// Lengths
			int dx = x2 - x1;
			int dy = y2 - y1;

			for (int xPix = x1; xPix < x2; ++xPix)
			{
				
			}

			img.save(output);
		}
	}
	catch (std::exception& e) {
		std::cout << "Error. " << e.what() << std::endl;
	}
	catch (...) {
		std::cout << "Unknown error. " << std::endl;
	}
	inFile.close();
	return 0;
}