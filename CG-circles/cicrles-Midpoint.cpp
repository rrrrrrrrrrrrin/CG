//#include <iostream>
//#include <string>
//#include <cmath>
//#include "rgbimage.h"
//
///*
//	// Set the origin for the circle, we will offset x and y coordinate by x0, y0 later
//	int x = 0;
//	// The grid increases x to the right and increases y downwards
//	// Place y higher by R on the grid, by decreasing it by R. x's position is not changed; to start drawing a circe
//	int y = -R;
//
//	// Draw a circle
//	while (x < -y)  // The octant is restricted by the line x = -y (consider the grid)
//	{
//		// Take yMid to determine which pixel to color in
//		// If yMid is inside the circle, color in the pixel at y; if outside, color in the pixel at y+1
//		int yMid = y + static_cast<int>(lround(0.5));
//
//		// Determine if the distance between yMid and the circle's origin is outside the circle
//		if (x * x + yMid * yMid > R * R)
//		{
//			++y;  // change y to increase downwards
//		}
//
//		// Color in the pixel; offset x and y coordinate by x0, y0
//		img.get(x + x0, y + y0) = white;  // y = x
//
//		// Duplicate the algorithm for 1 octant on all 8 octants
//		img.get(y + y0, x + x0) = white;  // x = y
//		img.get(x + x0, -y + y0) = white;  // x = -y
//		img.get(y + y0, -x + x0) = white;  // y = -x
//		img.get(-y + y0, -x + x0) = white;  // -y = -x
//		img.get(-x + x0, -y + y0) = white;  // -x = -y
//		img.get(-x + x0, y + y0) = white;  // -x = y
//		img.get(-y + y0, x + x0) = white;  // -y = x
//
//		++x;
//	}
//
//	The general algorithm ro get rid of floats (subsequently division) is to define a decision parameter (error),
//	find its initial value, determine its step value (by finding errorNext - errorCurrent) and changing error in the loop
//	according to the step value under certain conditions (the same algorithm is used to get rid of division in Bresenham's line algorithm)
//
//	1. Let's call our decision parameter (or error) p. In our loop we determine whether y increases by this expression: x * x + yMid * yMid > R * R
//		Then if p = x * x + yMid * yMid - R * R > 0 then ++y. yMId = y + 0.5 => [p = x^2 + (y+0.5)^2 - R^2]
//
//	2. pInitial (initial values of x and yMid are x = 0 and yMid = y + 0.5 = 0.5 - R) = 0 + (0.5-R)^2 - R^2 =
//		= 0.25 - R + R^2 - R^2 = 0.25 - R. We can get rid of the float or multiply every term by the denominator: [pInitial = -R]
//
//	3. Let's go back to p to determine pNext. [pNext is p when x+1]: pNext (yMid = y + 0.5) = (x+1)^2 + (y + 0.5)^2 - R^2 =
//		= x^2 + 2x + 1 + y^2 + y + 0.25 - R^2 = x^2 + 2x + y^2 + y + 1 - R^2 + 0.25. Let's get rid of the float (could also mupltiply every term by its denominator)
//		[pNext = x^2 + 2x + y^2 + y - R^2 + 1] ( DON'T SUM UP 1 + 0.25 because 1 is derivated from (x+1) in pNext )
//
//	4. pStep = pNext - pCurrent. pCurrent here is p => pStep = x^2 + 2x + y^2 + y - R^2 + 1 - x^2 - y^2 - y - 0.25 + R^2 => [pStep = 2x + 1] (get rid of the float)
//
//	5. Got the value of pStep when y doesn't increase (before recalculation of p in a loop): p = p + pStep. If it does (++y) p will change accordingly: (x stays the same in p)
//		p = p + pIncreased, where pIncreased = pNext - p(y-1) ( p(y-1) = x^2 + (y+0.5-1) - R^2. We subtract 1 from y because y already was incremented
//		before the recalculation of p in the loop; in pNext y is now considered incremented) = x^2 + 2x + y^2 + y - R^2 + 1 - (x^2 + (y - 0.5)^2 - R^2) =
//		= x^2 + 2x + y^2 + y - R^2 + 1 - x^2 - y^2 + y - 0.25 + R^2 => [pIncreased = 2x + 2y + 1] (get rid of the float)
//
//	6. Finally, if p > 0: the pixel is outside a circle => ++y, x+1 (p = p + pIncreased)
//				if p < 0: the pixel is inside a circle  => y doesn't change, x+1 (p = p + pStep)
//
//*/
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
//		while (inFile.get() != EOF)
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
//			int p = -R;  // pInitial
//
//			// Draw a circle
//			while (x < -y)  // The octant is restricted by the line x = -y (consider the grid)
//			{
//				// Pixel is outside the circle
//				if (p > 0)
//				{
//					++y;
//					p = p + 2*(x + y) + 1; // p = p + pIncreased;
//				}
//				else  // p < 0: pixel is inside the circle
//				{
//					p = p + 2*x + 1;
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