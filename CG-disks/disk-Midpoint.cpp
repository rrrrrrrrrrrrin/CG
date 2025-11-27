#include <iostream>
#include <string>
#include <cmath>
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

		int x0 = 0;
		int y0 = 0;
		int R = 0;

		while (inFile.get() != EOF)
		{
			// Collect start positions of x,y and a radius
			inFile >> x0;  
			inFile >> y0;
			inFile >> R;

			int r;
			int g;
			int b;
			
			inFile >> r;
			inFile >> g;
			inFile >> b;
			
			RGB color{ static_cast<BYTE>(r), static_cast<BYTE>(g), static_cast<BYTE>(b) };

			// Set the origin for the circle, we will offset x and y coordinate by x0, y0 later
			int x = 0;
			// The grid increases x to the right and increases y downwards
			// Place y higher by R on the grid, by decreasing it by R. x's position is not changed; to start drawing a circe
			int y = -R;

			int p = -R;  // pInitial

			// Draw a circle
			while (x < -y)  // The octant is restricted by the line x = -y (consider the grid)
			{
				// Pixel is outside the circle
				if (p > 0)
				{
					++y;
					p = p + 2*(x + y) + 1; // p = p + pIncreased;
				}
				else  // p < 0: pixel is inside the circle
				{
					p = p + 2*x + 1;
				}
				
				// x changes, y stays the same
				for (int i = -x; i < x; ++i)
				{
					img.get(i + x0, y + y0) = color;
				}

				for (int i = -x; i < x; ++i)
				{
					img.get(i + x0, -y + y0) = color;
				}

				// y changes, x stays the same
				for (int i = -y; i < y; ++i)
				{
					img.get(i + y0, x + x0) = color;
				}

				for (int i = -y; i < y; ++i)
				{
					img.get(i + y0, -x + x0) = color;
				}

				++x;
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