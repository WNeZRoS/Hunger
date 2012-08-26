#include "TgaGlTexture.h"
#include <GL/gl.h>
#include <Windows.h>
#include <stdio.h>

TgaGlTexture::TgaGlTexture() {

}

GlTexture * TgaGlTexture::load(const char *filename, bool linearFilter) {
	unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char TGAcompare[12];
	unsigned char header[6];

	FILE *file = fopen(filename, "rb");

	if(file == NULL) {
		printf("Cannot open file: %s\n", filename);
		fclose(file);
		return NULL;
	}
	if(fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare)) {
		printf("Wrong TGA header\n");
		fclose(file);
		return NULL;
	}
	if(memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0) {
		printf("Cann't memcmp\n");
		fclose(file);
		return NULL;
	}
	if(fread(header, 1, sizeof(header), file) != sizeof(header)) {
		printf("Wrong header\n");
		fclose(file);
		return NULL;
	}

	TgaGlTexture *result = new TgaGlTexture();

	if(!result) return NULL;

	result->_width = header[1] * 256 + header[0];
	result->_height = header[3] * 256 + header[2];

	if(result->_width <= 0 || result->_height <= 0 || (header[4] != 24 && header[4] != 32)) {
		fclose(file);
		printf("Wrong width/height or wrong bpp of texture.\n");
		delete result;
		return NULL;
	}

	unsigned char bitsPerPixel = header[4];
	unsigned char bytesPerPixel = bitsPerPixel / 8;
	unsigned int imageSize = result->_width * result->_height * bytesPerPixel;

	unsigned char *imageData = new unsigned char[imageSize];
	if(imageData == NULL || fread(imageData, 1, imageSize, file) != imageSize) {
		if(imageData) delete [] imageData;
		fclose(file);
		printf("Error reading to ImageData.\n");
		delete result;
		return NULL;
	}

	fclose(file);

	// Invert colors
	for(unsigned int i = 0; i < imageSize; i += bytesPerPixel) {
		char tmp = imageData[i];
		imageData[i] = imageData[i+2];
		imageData[i+2] = tmp;
	}

	// Now build texture.
	glGenTextures(1, &result->_id);
	glBindTexture(GL_TEXTURE_2D, result->_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, linearFilter ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, linearFilter ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	unsigned int type = 0x1904 + bytesPerPixel;
	glTexImage2D(GL_TEXTURE_2D, 0, type, result->_width, result->_height, 0,
				 type, GL_UNSIGNED_BYTE, imageData);

	delete [] imageData;
	return result;
}
