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
//		RGB white{ 255, 255, 255 };
//
//		float x1 = 0;
//		float y1 = 0;
//		float x2 = 0;
//		float y2 = 0;
//
//		float slopeX = 0;
//		float slopeY = 0;
//
//		while (inFile.get() != EOF)
//		{
//			inFile >> x1;
//			inFile >> y1;
//			inFile >> x2;
//			inFile >> y2;
//
//			// Lengths; make values float so that the division later on, will give a float slope value (if the slope was int, it would just be 0)
//			float dx = x2 - x1;
//			float dy = y2 - y1;
//
//			float max_length = std::max(std::abs(dx), std::abs(dy));
//
//			if (max_length != 0)
//			{
//				slopeX = dx / max_length;  // if not 1 (or -1) (slope = dx/dx),  slope is a float value, line is more horizontal than vertical (tg a = k = dx/dy; y = kx )
//		        slopeY = dy / max_length;  // if not 1 (or -1) (slope = dy/dy), slope is a float value, line is more vertical than horizontal (tg a = k = dy/dx; y = kx )
//
//				for (int px = 0; px <= static_cast<int>(max_length); ++px)
//				{
//					int w = static_cast<int>(std::round(x1 + static_cast<float>(px) * slopeX));
//					int h = static_cast<int>(std::round(y1 + static_cast<float>(px) * slopeY));
//
//					img.get(static_cast<unsigned int>(w), static_cast<unsigned int>(h)) = white;
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