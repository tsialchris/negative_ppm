#include <iostream>
#include <string>
#include "ppm\ppm.h"
#include "Image.h"

using namespace std;
using namespace imaging;

int main(int argc, char *argv[]) {
	
	string image_name;
	if (argc == 1) {
		cout << "Please type the name of the image you want to load: ";
		cin >> image_name;
	}
	else {
		image_name = argv[1];
	}
	
	Image * Example = new Image();
	Example->load(image_name, ".ppm");
	Example->save(image_name, ".ppm");
	system("pause");
}