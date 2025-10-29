#include <iostream>
#include <string>
#include "rgbimage.h"

/*
	// Lengths; make values float so that the division by int later on, will give a float slope value
	float dx = x2 - x1;
	float dy = y2 - y1;

	if (dx > 0):

	float slope = dy / dx;
	int y = y1;

	for (px < dx+1):

	float p = round(y1 + (px + 1) * slope);  // current float pixel on the line

	// Determine where to color in an int pixel by checking distance from float pixel to y and y+1
	int d = y + 1 - p;  // y+1 > p
	int d1 = p - y;  // y is higher on the grid (i.e. less) than p

	// Color in the nearest to the float pixel int pixel which is either y or y+1
	if (d < d1) { ++y; }  // y+1 is closer, color it in

	1. p = d1 - d (p >= 0) (d < d1 => 0 < d1 - d => 0 < p) 

	2. p = p - y - y - 1 + p

	3. p = -2y - 1 + 2p

	4. p = -2y - 1 + 2(y1 + (px + 1) * slope)

	5. p = -2y - 1 + 2y1 + 2(px + 1) * dy / dx

	6. Multiply each term by dx to get rid of division : p = -2*y*dx - dx + 2*y1*dx + 2*px*dy + 2*dy

	7. pInitial (initial px = 0, y = y1) = -2*y1*dx - dx + 2*y1*dx + 2*0*dy + 2*dy = 2*dy - dx

	8. In current p value: - dx + 2*y1*dx + 2*dy are constant (dx, dy, y1 don't change in the loop) = > p = 2*dy*px - 2*dx*y

	9. pNext (px = px + 1, y = yNext (y or ++y if p < 0) ) = 2*dy*(px + 1) - 2*dx*y = 2*dy*px + 2*dy - 2*dx*yNext

	10. => pStep = pNext - p = 2*dy*px + 2*dy - 2*dx*yNext - 2*dy*px + 2*dx*y = 2*dy - 2*dx*yNext + 2*dx*y = 2*dy + 2*dx*(y - yNext)

	11. Let's define two variables based on the pStep variable's value of yNext (it can either stay the same or be incremented by one):
		pIncremented = p + pStep(yNext = y+1) = p + 2*dy + 2*dx*(y - y - 1) = p + 2*(dy - dx)
		pUnchanged = p + pStep(yNext = y) = p + 2*dy + 2*dx*(y - y) = p + 2*dy

	12. Obviously, pInitial should be placed outside the loop (dx, dy are constant)

	13. if (p >= 0)
		{
			++y;
			// pIncremented = p + 2 * (dy - dx);
			p = p + 2 * (dy - dx);
		}
		else
		{
			// pUnchanged = p + 2*dy
			p = p + 2 * dy;
		}
	14. Now Bresenham's line algorithm draws a line in an (x, y) octant (abs(dx) > abs(dy), dx > 0 (=> dy > 0); => slope > 0). To draw lines in other 7 octants:

		Determine direction of y(whether to ++y or --y in the loop) :

		direction = dy < 0 ? -1 : 1
		dy *= direction
		++y now will be y += direction (++y or --y)

		Now line can be drawn in octant (x, y) and (-x, y)

		To draw line in octants (x, -y) and (-x, -y):
		(dx < 0)
		if (x1 > x2) then swap(x1, x2), swap(y1, y2)

		Therefore, we transformed (before the loop with direction and swapping of coordinates) any octant into a (x, y) octant

		To draw line in octants (y, x), (y, -x), (-y, x) and (-y, -x),
		set another function where roles of y and x are switched
*/

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

		while (inFile.get() != EOF)
		{
			inFile >> x1;
			inFile >> y1;
			inFile >> x2;
			inFile >> y2;

			int dx = x2 - x1;
			int dy = y2 - y1;
			 
			// line is more horizontal than vertical; y changes depending on the slope = dy/dx (dy by dx bcs it is the change of y)
			if (std::abs(dx) > std::abs(dy)) 
			{
				// Recalculate dx and dy to make them positive if necessary
				if (x1 > x2)  // dx < 0
				{
					std::swap(x1, x2);
					std::swap(y1, y2);
				}

				dx = x2 - x1;
				dy = y2 - y1;

				// Determine direction of y
				int direction = dy < 0 ? -1 : 1;
				dy *= direction;

				if (dx != 0)
				{
					int y = y1;
					int p = 2 * dy - dx;

					for (int px = 0; px < dx + 1; ++px)
					{
						img.get(x1 + px, y) = white;  // (bcs px = 0 draw a pixel first)

						if (p >= 0)
						{
							y += direction;
							p = p + 2 * (dy - dx);
						}
						else
						{
							p = p + 2 * dy;
						}
					}
				}
			}
			else // (std::abs(dy) > std::abs(dx))
			{
				// Recalculate dx and dy to make them positive if necessary
				if (y1 > y2)  // dy < 0
				{
					std::swap(x1, x2);
					std::swap(y1, y2);
				}

				dx = x2 - x1;
				dy = y2 - y1;

				// Determine direction of x
				int direction = dx < 0 ? -1 : 1;
				dx *= direction;

				if (dy != 0)
				{
					int x = x1;
					int p = 2 * dx - dy;

					for (int py = 0; py < dy + 1; ++py)
					{
						// Write img.get(x, y) BECAUSE we put a pixel on an image which has a fixed (width, height), width is Ox, height is Oy. IT IS NOT ABOUT OCTANTS
						img.get(x, y1 + py) = white;  // (bcs py = 0 draw a pixel first)

						if (p >= 0)
						{
							x += direction;
							p = p + 2 * (dx - dy);
						}
						else
						{
							p = p + 2 * dx;
						}
					}
				}
			}
		}

		img.save(output);
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