//#include <iostream>
//#include <string>
//#include <cmath>
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
//			int r;
//			int g;
//			int b;
//
//			inFile >> r;
//			inFile >> g;
//			inFile >> b;
//
//			RGB color{ static_cast<BYTE>(r), static_cast<BYTE>(g), static_cast<BYTE>(b) };
//
//			int x = 0;
//			int y = R;
//
//			int p = 2 * (1 - R);  // pDiag_Initial
//
//			// Draw a circle
//			while (y >= 0)  //  in one quarter, than symmetrically put a pixel in other 3 quarters
//			{
//				// Draw the line in a semi-circle. x changes, y stays the same (x0 and y0 are offsets);
//				for (int i = -x; i < x; ++i)
//				{
//					img.get(i + x0, y + y0) = color;
//				}
//
//				for (int i = -x; i < x; ++i)
//				{
//					img.get(i + x0, -y + y0) = color;
//				}
//
//				int p1 = 2 * (p + y) - 1; // horizontal or diagonal 
//				int p2 = 2 * (p - x) - 1; // vertical or diagonal 
//
//				if (p < 0)  // draw a diagonal or an outside pixel
//				{
//					if (p1 < 0)  // draw an outside/horizontal pixel 
//					{
//						++x;
//						int pDiag_Step_H = 2 * x + 3;
//						p += pDiag_Step_H;
//					}
//					else  // p1 >= 0: draw a diagonal pixel
//					{
//						++x;
//						--y;
//						int pDiag_Step = 2 * x - 2 * y + 6;
//						p += pDiag_Step;
//					}
//				}
//				else  // pDiag >= 0: draw a diagonal or an inside pixel
//				{
//					if (p2 > 0)  // draw an inside/vertical pixel
//					{
//						--y;
//						int pDiag_Step_V = 3 - 2 * y;
//						p += pDiag_Step_V;
//					}
//					else  // p2 =< 0:  draw a diagonal pixel
//					{
//						++x;
//						--y;
//						int pDiag_Step = 2 * x - 2 * y + 6;
//						p += pDiag_Step;
//					}
//				}
//			}
//		}
//
//		img.save(output);
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