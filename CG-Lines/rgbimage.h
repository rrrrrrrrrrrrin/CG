#pragma once
#include <cstdint>
#include <stdexcept>
#include <fstream>

typedef std::uint16_t  WORD;
typedef std::uint32_t DWORD;
typedef int LONG;
typedef unsigned char BYTE;

struct RGB {
	BYTE Red;
	BYTE Green;
	BYTE Blue;
};

#pragma pack(push,1)
struct BITMAPFILEHEADER {
	WORD bfType;
	DWORD bfSize; // размер файла
	WORD bfReserved1;
	WORD bfReserved2;
	DWORD bfOffBits; // смещение до поля данных (обычно 54 = 16+biSize)
};

struct BITMAPINFOHEADER {
	DWORD biSize; // размер структуры в байтах
	LONG biWidth; // ширина в пикселях
	LONG biHeight;
	WORD biPlanes; // Всегда должно быть 1
	WORD biBitCount; // Кол-во бит на цвет 0 | 1 | 4 | 8 | 16 | 24 | 32
	DWORD biCompression;
	DWORD biSizeImage;
	LONG biXPelsPerMeter; // горизонтальное разрешение
	LONG biYPelsPerMeter; // вертикальное разрешение
	DWORD biClrUsed; // Кол-во используемых цветов (если используется таблица цветов)
	DWORD biClrImportant; // Кол-во существенных цветов
};
#pragma pack(pop)

class rgbImg {
	unsigned int width_;
	unsigned int height_;
	RGB** pixels_;
	void freeMemoryRows(unsigned int rows) {
		for (unsigned int row = 0; row < rows; ++row) {
			delete[] pixels_[row];
			pixels_[row] = nullptr;
		}
	}
	void swap(rgbImg& other) {
		std::swap(pixels_, other.pixels_);
		std::swap(width_, other.width_);
		std::swap(height_, other.height_);
	}
	int static get_offset(int width) {
		int offset = 0;
		if (width % 4 != 0)
		{
			offset = 4 - (3 * width) % 4;
		}
		return offset;
	}
public:
	rgbImg(unsigned int width = 800, unsigned int height = 600) : width_(width), height_(height), pixels_(nullptr) {
		RGB zero = { 0, 0, 0 };
		pixels_ = new RGB * [height_];
		if (pixels_ == nullptr)
		{
			throw std::bad_alloc();
		}
		for (unsigned int row = 0; row < height_; ++row) {
			pixels_[row] = new RGB[width_];
			if (pixels_[row] == nullptr) {
				freeMemoryRows(row);
				delete[] pixels_;
				throw std::bad_alloc();
			}
			for (unsigned int col = 0; col < width_; ++col) {
				pixels_[row][col] = zero;
			}
		}
	}
	rgbImg(const rgbImg& other) :width_(other.width_), height_(other.height_), pixels_(nullptr) {
		pixels_ = new RGB * [height_];
		if (pixels_ == nullptr)
		{
			throw std::bad_alloc();
		}
		for (unsigned int row = 0; row < height_; ++row) {
			pixels_[row] = new RGB[width_];
			if (pixels_[row] == nullptr) {
				freeMemoryRows(row);
				delete[] pixels_;
				throw std::bad_alloc();
			}
			for (unsigned int col = 0; col < width_; ++col) {
				pixels_[row][col] = other.pixels_[row][col];
			}
		}
	}
	rgbImg(rgbImg&& other) noexcept {
		width_ = other.width_;
		height_ = other.height_;
		pixels_ = other.pixels_;
		other.pixels_ = nullptr;
		other.width_ = 0;
		other.height_ = 0;
	}
	rgbImg& operator=(const rgbImg& other) {
		if (this != &other) {
			rgbImg tmp(other);
			swap(tmp);
		}
		return *this;
	}
	rgbImg& operator=(rgbImg&& other) noexcept {
		if (this != &other) {
			swap(other);
		}
		return *this;
	}
	unsigned int width() const { return width_; }
	unsigned int height() const { return height_; }
	RGB get(unsigned int column, unsigned int row) const {
		if (row >= height_)
		{
			throw std::out_of_range("Larger than image height");
		}
		if (column >= width_)
		{
			throw std::out_of_range("Larger than image width");
		}
		return pixels_[row][column];
	}
	RGB& get(unsigned int column, unsigned int row) {
		if (row >= height_)
		{
			throw std::out_of_range("Larger than image height");
		}
		if (column >= width_)
		{
			throw std::out_of_range("Larger than image width");
		}
		return pixels_[row][column];
	}
	bool save(const char* filename) {
		if (pixels_ == nullptr)
		{
			return false;
		}

		std::ofstream outBMP(filename, std::ios_base::binary);
		if (!outBMP.is_open()) {
			throw std::runtime_error("Failed to open output file");
		}

		const int offset = get_offset(static_cast<const int>(width_));
		BITMAPFILEHEADER bmfh;
		char bfType[] = { 'B', 'M' };
		bmfh.bfType = *((WORD*)bfType);
		bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
		bmfh.bfSize = height_ * width_ * 3 + bmfh.bfOffBits;
		bmfh.bfReserved1 = 0;
		bmfh.bfReserved2 = 0;

		outBMP.write((char*)&bmfh, sizeof(BITMAPFILEHEADER));

		BITMAPINFOHEADER bmih;
		bmih.biSize = sizeof(BITMAPINFOHEADER); // размер структуры в байтах
		bmih.biWidth = static_cast<LONG>(width_);  // ширина в пикселях
		bmih.biHeight = static_cast<LONG>(height_);
		bmih.biPlanes = 1;		// Всегда должно быть 1
		bmih.biBitCount = 24;   // Кол-во бит на цвет 0 | 1 | 4 | 8 | 16 | 24 | 32
		bmih.biCompression = 0;
		bmih.biSizeImage = bmfh.bfSize - bmfh.bfOffBits;
		bmih.biXPelsPerMeter = 1;
		bmih.biYPelsPerMeter = 1;
		bmih.biClrUsed = 0;
		bmih.biClrImportant = 0;
		outBMP.write((char*)&bmih, sizeof(BITMAPINFOHEADER));

		BYTE* offset_array = new BYTE[offset];
		for (int i = 0; i < offset; ++i)
		{
			offset_array[i] = 0;
		}

		for (unsigned int row = height_; row > 0; --row) {
			for (unsigned int col = 0; col < width_; ++col) {
				RGB tmp(pixels_[row - 1][col]);
				std::swap(tmp.Blue, tmp.Red);
				outBMP.write((char*)&tmp, sizeof(RGB));
			}
			outBMP.write((char*)offset_array, offset);
		}
		delete[] offset_array;
		outBMP.close();
		return true;
	}
	~rgbImg() {
		if (pixels_ != nullptr) {
			freeMemoryRows(height_);
			delete[] pixels_;
			pixels_ = nullptr;
		}
	}
};
