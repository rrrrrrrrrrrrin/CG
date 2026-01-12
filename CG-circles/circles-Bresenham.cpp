#include <iostream>
#include <string>
#include <cmath>
#include "rgbimage.h"

/*
	Consider the I quarter of the circle.
	x starts at 0, y starts at R (in our grid).
	Draw the line while y >= 0 (quarter ends)
	
	1. We have [3 possible pixels]: pixel is 
	   [outside the circle] (goes more [horizontal] than the circle outline
	   so we will increment x "to reach it": [++x]),

	   [inside] (goes more [vertical] 
	   so we will decrement y "to reach it": [--y]) 

	   and "on the line" - a [diagonal] pixel (same horizontal and vertical curve,
	   but not necessarily on the line; moved by a circle line: [++x, --y]).

	2. We will draw the pixel, to which the distance
	   from the center of the circle is the minimum of 3 distances
	   
	   The distance to the pixel is determined as x^2 + y^2. 
	   If distance > R^2 the pixel is outside,
	   distance < R^2 - inside, distance = R^2 - on the circle.
	   Than our decision parameter, 
	   let's call it [p = x^2 + y^2 - R^2], for example, if [p > 0 => pixel is outside]

	3. The 3 pixels can be defined as: 

	   [outside/horizontal] (++x) = [(x+1)^2 + y^2 - R^2]
	   
	   [diagonal] (x+1 and y-1) = [(x+1)^2 + (y-1)^2 - R^2]
		
	   [inside/vertical] (--y) = [x^2 + (y-1)^2 - R^2]

	4. Let's determine which pixel to draw 
	[based on the distance to the diagonal pixel]
	   
	   The [diagonal pixel can either be outside the circle or inside].

	   The decision parameter for the pixel is [pDiag = (x+1)^2 + (y-1)^2 - R^2].

	   1) If [pDiag > 0], the [pixel is outside],
		  we should draw [either diagonal or inside pixel] => [either diagonal or vertical step]
	      (choose the shortest distance <=> the closest to the circle outline pixel)

	   2) If [pDiag < 0], the [pixel is inside],
	      we should draw [either diagonal or outside pixel] => [either diagonal or horizontal step]
	      (choose the shortest distance <=> the closest to the circle outline pixel)


	5. Determine which pixel to draw in each individual case 
	   (define decision parameters). To do that, find the shortest distance:

	   |pOutside|, |pInside| and |pDiag| are distances.

	   |pInside| < |pDiag| < |pOutside|

	   We compare the diagonal pixel and the outside pixel.
	   Diagonal pixel should be inside the circle for this comparison,
	   to actually have a difference between distances and choose the shortest one.
	   Analogy - the diagonal and the inside pixel, pDiag > 0 (diagonal pixel is outside).

	   1) Decision parameter [pDiag < 0] - pixel is inside,
	                         [pOutside > 0] - pixel is outside
							 ((x+1)^2 + y^2 - R^2 > 0 => (x+1)^2 + y^2 > R^2)
	      => p1 = |pOutside| - |pDiag|

		  Consider [pDiag < 0], so [the diagonal pixel is inside the circle outline]
		  (closer to the circle than the outside/horizontal pixel), 
		  so if the diagonal pixel has a shorter distance:
		  pDiag < pOutside => p1 > 0: draw the diagonal pixel

		  p1 = 2(x-1)^2 - 2R^2 + (y-1)^2 + y^2
		  
		  We need to [derivate a pDiag value]: pDiag = (x+1)^2 + (y-1)^2 - R^2 
		  (WE WILL GET [pDiag DOUBLED] ALL THE TIME LATER AS WELL
		   AS HERE WHEN GETTING A DECISION PARAMETER VALUE)

		  (it will be the only value that has a pDiag_Initial value,
		  is checked in the loop pDiag > 0, < 0
		  to determine other checks (p1 or p2) and will be 
		  changed with pDiag Steps: p = p + pDiag Step) 

		  p1 = 2(x-1)^2 - 2R^2 + (y-1)^2 + y^2 {-2y + 2y + 1 - 1} 
		  = 2(pDiag + y) - 1 => [p1 = 2(pDiag + y) - 1]

		  If pOutside < pDiag (the distance of the outside pixel is shorter; draw this pixel) 
		  => pOutside - pDiag < 0 => if [p1 < 0], then draw the [outside pixel]
		                             if [p1 > 0]: [diagonal]


	   2) Analogy - the diagonal and the inside pixel.
	   
		  Decision parameter [pDiag > 0] - pixel is outside,
		  [pInside < 0] - pixel is inside 
		  (x^2 + (y-1)^2 - R^2 < 0 => x^2 + (y-1)^2 < R^2)
		  
		  => p2 = |pDiag| - |pInside| = (x+1)^2 + (y-1)^2 - R^2 + x^2 + (y-1)^2 - R^2.
		  
		  Derivate pDiag from the expression: 
		  p2 = 2(y-1)^2 - 2R^2 + (x+1)^2 + x^2 {+2x - 2x + 1 - 1} 
		  = 2(pDiag) - 2x - 1 = 2(pDiag - x) - 1

		  => [p2 = 2(pDiag - x) - 1]

		  [p2 < 0]: [diagonal], [p2 > 0]: [inside]


	6. We have a decision parameter pDiag.
	   Find pDiag STEPS to determine how we should update pDiag_Initial:

	   pDiag_Initial = p_Diag(x = 0, y = R) 
	   = 1 + (R-1)^2 - R^2 
	   = 1 + R^2 - 2R + 1 - R^2 
	   = -2R + 2 = 2(1-R)] 
	   => [pDiag_Initial = 2(1-R)]
	   

	   |pDiag_Next_V| (--y) < |pDiag| < |pDiag_Next_H| (++x)

	   pDiag_Step_H = |pDiag_Next_H| 
	   ( pDiag(x = x+1, change of an outside pixel, i.e. horizontal step) ) - |pDiag|

	   Horizontal step => x = x+1 and pDiag < 0, pDiag_Next_H > 0 
	   (the pixel is outside now bcs ++x) =>

	   pDiag_Step_H = (x+2)^2 + (y-1)^2 - R^2 + (x+1)^2 + (y-1)^2 - R^2 
	   = 2(y-1)^2 - 2R^2 + (x+1)^2 + x^2 + 4x + 4 {-2x + 2x -3 + 3}
	   = 2((x+1)^2 + (y-1)^2 - R^2) + 2x + 3 = 2(pDiag + x) + 3

	   [pDiag_Step_H = 2(pDiag + x) + 3]
	

	   pDiag_Step_V = |pDiag| -  |pDiag_Next_V| 
	   ( pDiag(y = y-1), change of an inside pixel, i.e. vertical step)

	   Vertical step => y -= y and pDiag > 0, pDiag_Next_V < 0 
	   (the pixel is inside now bcs --y) =>

	   pDiag_Step_V = (x+1)^2 + (y-2)^2 - R^2 + (x+1)^2 + (y-1)^2 - R^2 
	   = 2(pDiag) - 2y + 3 
	   = 2(pDiag - y) + 3 
	   ((y-2)^2 = y^2 - 4y + 4; => to get (y-1)^2: -2y + 2y - 3 + 3)

	   [pDiag_Step_V = 2(pDiag - y) + 3]


	   pDiag_Step = |pDiag_Next| ( pDiag(x = x+1, y = y-1) ) - |pDiag|

	   Let's say pDiag < 0, pDiag_Next > 0 
	   (let's say the pixel moved outside from the inside, hence signs;
	    to derivate pDiag AND have positive values)

	   pDiag_Step = (x+2)^2 + (y-2)^2 - R^2 + (x+1)^2 + (y-1)^2 - R^2 
	   = pDiag - R^2 + (x+2)^2 - (y-2)^2 
	   = pDiag - R^2 + (x+1)^2 + 2x + 3 + (y-1)^2 - 2y + 3
	   = 2(pDiag + x - y) + 6

	   [pDiag_Step = 2(pDiag + x - y) + 6]


	7. Final values that will be used in the algorithm:

	   Initial x, y: x=0, y=R

	   Decision parameters:

	   [pDiag_Initial = 2(1-R)]

	   [pDiag > 0]: [diagonal or outside pixel]
	   [pDiag < 0]: [diagonal or inside pixel]

	   [p1 = 2(pDiag + y) - 1]
	   [p1 < 0: outside]
	   [p1 > 0: diagonal]

	   [p2 = 2(pDiag - x) - 1]
	   [p2 < 0: diagonal]
	   [p2 > 0: inside]


	   For steps: pDiag = pDiag + pDiag Step:
	   [pDiag_Step_H = 2(pDiag + x) + 3 = > 2pDiag + 2x + 3 => 
	   Just get rid of two (don't want to result in floats by division of parameters):
	   pDiag_Step_H = pDiag + 2x + 3 => pDiag will change: pDiag += 2x + 3]; ++x

	   [pDiag_Step_V = 2(pDiag - y) + 3 => pDiag += 3 - 2y]; --y

	   [pDiag_Step = 2(pDiag + x - y) + 6 => pDiag += 2x - 2y + 6]; ++x, --y

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

		int x0 = 0;
		int y0 = 0;
		int R = 0;

		while (inFile.get() != EOF)
		{
			// Collect start positions of x,y and a radius
			inFile >> x0;  
			inFile >> y0;
			inFile >> R;

			int x = 0;
			int y = R;

			int p = 2*(1 - R);  // pDiag_Initial

			// Draw a circle
			// in one quarter, than symmetrically
			// put a pixel in other 3 quarters
			while (y >= 0) 
			{
				// Color in the pixel; offset x and y coordinate by x0, y0
				img.get(x + x0, y + y0) = white;  // y = x

				// Duplicate the algorithm for 1 quarter on all 4 quarters
				img.get(x + x0, -y + y0) = white;  // x = -y
				img.get(-x + x0, -y + y0) = white;  // -x = -y
				img.get(-x + x0, y + y0) = white;  // -x = y

				int p1 = 2*(p + y) - 1; // horizontal or diagonal 
				int p2 = 2*(p - x) - 1; // vertical or diagonal 

				if (p < 0)  // draw a diagonal or an outside pixel
				{
					if (p1 < 0)  // draw an outside/horizontal pixel 
					{
						++x;
						int pDiag_Step_H = 2*x + 3;
						p += pDiag_Step_H;
					}
					else  // p1 >= 0: draw a diagonal pixel
					{
						++x;
						--y;
						int pDiag_Step = 2*x - 2*y + 6;
						p += pDiag_Step;
					}
				}
				else  // pDiag >= 0: draw a diagonal or an inside pixel
				{
					if (p2 > 0)  // draw an inside/vertical pixel
					{
						--y;
						int pDiag_Step_V = 3 - 2*y;
						p += pDiag_Step_V;
					}
					else  // p2 =< 0:  draw a diagonal pixel
					{
						++x;
						--y;
						int pDiag_Step = 2 * x - 2 * y + 6;
						p += pDiag_Step;
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