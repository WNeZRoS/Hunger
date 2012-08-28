#include "TgaTexture.h"
#include "GlTexture.h"
#include "compatibility.h"

#ifdef WIN32
	#include <Windows.h>
#endif

#include <GL/gl.h>
#include <iostream>
#include <fstream>

using namespace std;

Texture * TgaTexture::loadGl(const char *filename) {
	ifstream file(filename, ios::in | ios::binary);

	if(!file.is_open()) {
		cout << "Cannot open file: " << filename << endl;
		return NULL;
	}

	static const char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	char TGAcompare[12];

	file.read(TGAcompare, sizeof(TGAcompare));
	if(file.gcount() != sizeof(TGAcompare)) {
		cout << "Wrong TGA header" << endl;
		file.close();
		return NULL;
	}
	if(memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0) {
		cout << "Cann't memcmp" << endl;
		file.close();
		return NULL;
	}

	unsigned char header[6];
	file.read((char*) header, sizeof(header));
	if(file.gcount() != sizeof(header)) {
		cout << "Wrong header" << endl;
		file.close();
		return NULL;
	}

	int width = header[1] * 256 + header[0];
	int height = header[3] * 256 + header[2];

	if(width <= 0 || height <= 0 || (header[4] != 24 && header[4] != 32)) {
		file.close();
		cout << "Wrong width/height or wrong bpp of texture." << endl;
		return NULL;
	}

	unsigned char bitsPerPixel = header[4];
	unsigned char bytesPerPixel = bitsPerPixel / 8;
	unsigned int imageSize = width * height * bytesPerPixel;

	char *imageData = new char[imageSize];
	file.read(imageData, imageSize);
	if(imageData == NULL || (unsigned int) file.gcount() != imageSize) {
		if(imageData) delete [] imageData;
		file.close();
		cout << "Error reading to ImageData." << endl;
		return NULL;
	}

	file.close();

	// Swap R and B color
	for(unsigned int i = 0; i < imageSize; i += bytesPerPixel) {
		char tmp = imageData[i];
		imageData[i] = imageData[i+2];
		imageData[i+2] = tmp;
	}

	GlTexture * texture = new GlTexture(imageData, 0x1904 + bytesPerPixel, width, height);
	delete [] imageData;
	return texture;
}
