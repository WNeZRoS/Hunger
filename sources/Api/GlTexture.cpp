#include "GlTexture.h"
#include "TextureAtlas.h"
#include "Logger.h"

#ifdef WIN32
#include <Windows.h>
#endif
#include <GL/gl.h>

GlTexture::GlTexture(const char *imageData, GLuint type, int width, int height) {
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, DEFAULT_LINEAR_FILTER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, DEFAULT_LINEAR_FILTER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, imageData);

	_width = width;
	_height = height;
}

GlTexture::GlTexture() {
	_id = 0;
	Log::logger << Log::debug << "GlTexture " << _id;
}

GlTexture::~GlTexture() {
	if(_id != 0) {
		glDeleteTextures(1, &_id);
		_id = 0;
	}
}

unsigned int GlTexture::getId() const {
	return _id;
}

void GlTexture::activate() const {
	glBindTexture(GL_TEXTURE_2D, _id);
}

void GlTexture::deactivate() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}

TextureAtlas * GlTexture::toAtlas(int rowSize, int columnSize) const {
	Log::logger << Log::debug << "GlTexture to Atlas " << _id;
	return TextureAtlas::create(this, rowSize, columnSize);
}
