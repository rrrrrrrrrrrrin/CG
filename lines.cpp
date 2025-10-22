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

		float slopeY = 0;
		float slopeX = 0;

		int ch;
		while ((ch = inFile.get()) != EOF)
		{
			inFile >> x1;
			inFile >> y1;
			inFile >> x2;
			inFile >> y2;

			// Lengths; make float so that the division by int will give a float slope value (if the slope was int, it would just be 0)
			float dx = x2 - x1;
			float dy = y2 - y1;

			int max_length = std::max(std::abs(dx), std::abs(dy));

			if (max_length != 0)
			{
				slopeY = dy / max_length;  // if not 1 (or -1), line is more vertical than horizontal (tg a = k = dy/dx; y = kx )
				slopeX = dx / max_length;  // if not 1 (or -1), line when is more horizontal than vertical (tg a = k = dx/dy; y = kx )

				std::cout << slopeY << " " << slopeX << '\n';

				for (int px = 0; px <= max_length; ++px)
				{
					img.get((x1 + px * slopeX), round(y1 + px * slopeY)) = white;
				}
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