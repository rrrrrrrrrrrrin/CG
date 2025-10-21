#include <iostream>
#include "rgbimage.h"

int main(int argc, char* argv[]) {
	if (argc < 3) {
		return 1;
	}
	std::ifstream inFile(argv[1]);
	if (!inFile) {
		return 2;
	}

	try 
	{
		// Координаты левого верхнего угла изображения (0,0).
		// Координаты правого нижнего угла изображения (799, 599).
		// В центре изображения нарисовать квадрат 200 на 200 пикселей белого цвета

		rgbImg img(800, 600);        // Задаем изображение width=800px, height=600px
		RGB white{ 255, 255, 255 };  // Задаем цвет в модели RGB (по 1 байту на канал).

		for (int row = 200; row < 400; ++row)
			for (int col = 300; col < 500; ++col)
				img.get(col, row) = white;  // Пиксель в строке row, столбце col красим в белый цвет

		// Сохраняем изображения в BMP-файл
		img.save("white_square.bmp");
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