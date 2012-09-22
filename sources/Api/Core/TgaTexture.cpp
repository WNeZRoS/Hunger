#include "TgaTexture.h"
#include "GlTexture.h"
#include "Logger.h"
#include "../compatibility.h"

#include "GL/importgl.h"
#include "FileManager.h"

using namespace std;

Texture * TgaTexture::loadGl(const char *filename) {
	ifstream *file = FileManager::instance().read(filename, true);

	if(!file || !file->is_open()) {
		Log::logger << Log::error << "Cannot open file: " << filename;
		if(file) delete file;
		return NULL;
	}

	static const char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	char TGAcompare[12];

	file->read(TGAcompare, sizeof(TGAcompare));
	if(file->gcount() != sizeof(TGAcompare)) {
		Log::logger << Log::error << "Wrong TGA header";
		delete file;
		return NULL;
	}
	if(memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0) {
		Log::logger << Log::error << "Cann't memcmp";
		delete file;
		return NULL;
	}

	unsigned char header[6];
	file->read((char*) header, sizeof(header));
	if(file->gcount() != sizeof(header)) {
		Log::logger << Log::error << "Wrong header";
		delete file;
		return NULL;
	}

	int width = header[1] * 256 + header[0];
	int height = header[3] * 256 + header[2];

	if(width <= 0 || height <= 0 || (header[4] != 24 && header[4] != 32)) {
		delete file;
		Log::logger << Log::error << "Wrong width/height or wrong bpp of texture.";
		return NULL;
	}

	unsigned char bitsPerPixel = header[4];
	unsigned char bytesPerPixel = bitsPerPixel / 8;
	unsigned int imageSize = width * height * bytesPerPixel;

	unsigned char *imageData = new unsigned char[imageSize];
	file->read(reinterpret_cast<char*>(imageData), imageSize);
	if(imageData == NULL || (unsigned int) file->gcount() != imageSize) {
		if(imageData) delete [] imageData;
		delete file;
		Log::logger << Log::error << "Error reading to ImageData.";
		return NULL;
	}

	delete file;

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
