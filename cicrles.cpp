//#include <iostream>
//#include <string>
//#include "rgbimage.h"
//
//int main(int argc, char* argv[]) {
//	if (argc < 3) {
//		return 1;
//	}
//
//	const char* input = argv[1];
//	const char* output = argv[2];
//
//	std::ifstream inFile(input);
//	if (!inFile) {
//		return 2;
//	}
//
//	try 
//	{
//		int width = 0;
//		int height = 0;
//
//		inFile >> width;
//		inFile >> height;
//
//		rgbImg img(width, height);
//		RGB white{ 255, 255, 255 };
//
//		int x0 = 0;
//		int y0 = 0;
//		int R = 0;
//
//		int ch;
//		while ( (ch = inFile.get()) != EOF)
//		{
//			// Collect start positions of x,y and a radius
//			inFile >> x0;  
//			inFile >> y0;
//			inFile >> R;
//
//			// Set the origin for the circle, we will offset x and y coordinate by x0, y0 later
//			int x = 0;
//			// The grid increases x to the right and increases y downwards
//			// Place y higher by R on the grid, by decreasing it by R. x's position is not changed; to start drawing a circe
//			int y = -R;
//
//			// Draw a circle
//			while (x < -y)  // The octant is restricted by the line x = -y (consider the grid)
//			{
//				// Take yMid to determine which pixel to color in 
//				// If yMid is inside the circle, color in the pixel at y; if outside, color in the pixel at y+1
//				int yMid = y + 0.5;
//				
//				// Determine if the distance between yMid and the circle's origin is outside the circle 
//				if (x * x + yMid * yMid > R * R) 
//				{
//					++y;  // change y to increase downwards
//				}
//				
//				// Color in the pixel; offset x and y coordinate by x0, y0
//				img.get(x + x0, y + y0) = white;  // y = x
//
//				// Duplicate the algorithm for 1 octant on all 8 octants
//				img.get(y + y0, x + x0) = white;  // x = y
//				img.get(x + x0, -y + y0) = white;  // x = -y
//				img.get(y + y0, -x + x0) = white;  // y = -x
//				img.get(-y + y0, -x + x0) = white;  // -y = -x
//				img.get(-x + x0, -y + y0) = white;  // -x = -y
//				img.get(-x + x0, y + y0) = white;  // -x = y
//				img.get(-y + y0, x + x0) = white;  // -y = x
//
//				++x;
//			}
//
//			img.save(output);
//		}
//	}
//	catch (std::exception& e) {
//		std::cout << "Error. " << e.what() << std::endl;
//	}
//	catch (...) {
//		std::cout << "Unknown error. " << std::endl;
//	}
//	inFile.close();
//	return 0;
//}