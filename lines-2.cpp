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

		float x1 = 0;
		float y1 = 0;
		float x2 = 0;
		float y2 = 0;

		while (inFile.get() != EOF)
		{
			inFile >> x1;
			inFile >> y1;
			inFile >> x2;
			inFile >> y2;

			// Lengths; make values float so that the division by int later on, will give a float slope value
			float dx = x2 - x1;
			float dy = y2 - y1;

			if (std::abs(dy) >= std::abs(dx))
			{
				float slope = dy / dx;

				int change = slope < 0 ? -1 : 1;

				if (dx < 0)
				{
					std::swap(x1, x2);
					std::swap(y1, y2);
				}

				int y = y1;

				for (int px = 0; px < std::abs(dx); ++px)
				{
					float p = round(y + px * slope);  // current float pixel on the line

					// Determine where to color in an int pixel by checking distance from float pixel to y and y+1
					int d = y + 1 - p;  // y+1 > p
					int d1 = p - y;  // y is higher on the grid (i.e. less) than p

					// Color in the nearest to the float pixel int pixel which is either y or y+1
					if (d < d1) { y += change; }  // y+1 is closer, color it in

					img.get(y, x1 + px) = white;
				}
			}
			else
			{
				float slope = dx / dy;

				int change = slope < 0 ? -1 : 1;

				if (dy < 0)
				{
					std::swap(y1, y2);
					std::swap(x1, x2);
				}

				int x = x1;

				for (int px = 0; px < std::abs(dy); ++px)
				{
					float p = x + px * slope;  

					int d = x + 1 - p;  
					int d1 = p - x;  

					if (d < d1) { x += change; } 

					img.get(x, y1 + px) = white;
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