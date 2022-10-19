#include "Image.h"
#include <algorithm>
#include <string>

#include <ctype.h>
#include <iostream>

#include "ppm\ppm.h"
#define _IMAGE

namespace imaging {
	//getter for buffer
	Color * Image::getRawDataPtr() {
		return buffer;
	}
	//getter for Pixel
	Color Image::getPixel(unsigned int x, unsigned int y) const {
		/*if (x > width || y > height || x < width || y < height) {
			Color black(0, 0, 0);
			return black;
		}*/
		return buffer[x + y * width];
	}
	//setter for Pixel
	void Image::setPixel(unsigned int x, unsigned int y, Color & value) {
		/*
		if (x > width || y > height || x < width || y < height) {
			cout << "Pixel (" << x << "," << y << ") is out of bounds" << endl;
		}
		*/
		unsigned int place_in_buffer = x + y * width;
		buffer[place_in_buffer] = value;
	}
	//setter for buffer
	void Image::setData(const Color * & data_ptr) {
		if (buffer != 0) {
			buffer = (Color *)data_ptr;
		}
	}
	//default constructor
	Image::Image() {
		width = 0;
		height = 0;
		buffer = 0;
	}
	//width,height constructor
	Image::Image(unsigned int _width, unsigned int _height) : width(_width), height(_height), buffer(0) {}
	//width,height,buffer constructor
	Image::Image(unsigned int width, unsigned int height, const Color * data_ptr) {
		this->Image::Image(width, height);
		*buffer = *data_ptr;
	}
	//copy constructor
	Image::Image(const Image &src) {
		this->Image::Image(src.width, src.height, src.buffer);
	}
	Image & Image::operator= (const Image & right) {return (Image &)right;}
	//destructor
	Image::~Image() {}
	//load implementation
	bool Image::load(const std::string & filename, const std::string & format) {
		//check if the file ends with .ppm
		int length = strlen(filename.c_str());
		//filename length
		if (length >= 4 && strcmp(filename.c_str() + length - 4, format.c_str()) == 0) {

			int h=0;
			int w=0;
			//read ppm from the folder
			float *store_ppm = ReadPPM(filename.c_str(), &w, &h);
			
			// if the h and w vars remain uncanged, it will return false
			if (w == 0 || h == 0) {
				return false;
			}

			this->width = w;
			this->height = h;

			//converting buffer into an array
			buffer = new Color[width*height];

			//each pixel has 3 colors
			int j = 0;
			int i = 0;
			while (i < width*height * 3) {
				buffer[j].r = store_ppm[i];
				buffer[j].g = store_ppm[i + 1];
				buffer[j].b = store_ppm[i + 2];
				i = i + 3;
				j++;
			}
			delete store_ppm;
			return true;
		}
		return false;
	}

	//save implementation
	bool Image::save(const std::string & filename, const std::string & format) {

		//initialize a new color object and make it have the highest values
		Color * higest_val_color = new Color();
		higest_val_color->r = 1.0;
		higest_val_color->g = 1.0;
		higest_val_color->b = 1.0;

		float * to_be_written = new float[width * height * 3];
		int j = 0;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				setPixel(x, y, *higest_val_color - getPixel(x, y));
				to_be_written[j] = getPixel(x, y).r;
				to_be_written[j + 1] = getPixel(x, y).g;
				to_be_written[j + 2] = getPixel(x, y).b;
				j = j + 3;
			}
		}
		/*
		float *inverted_buffer = new float[width*height * 3];
		int j = 0;
		int i = 0;

		while (i < width*height) {
			inverted_buffer[i] = *higest_val_color - buffer[i];
			to_be_written[j] = inverted_buffer[i].r;
			to_be_written[j+1] = inverted_buffer[i].g;
			to_be_written[j+2] = inverted_buffer[i].b;
			i++;
			j = j + 3;
		}*/

		WritePPM(to_be_written, width, height, filename.c_str());

		//delete the new vars
		delete higest_val_color;
		delete buffer;
		return true;
	}
}