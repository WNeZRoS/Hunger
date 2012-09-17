#include "GlTexture.h"
#include "TextureAtlas.h"
#include "Logger.h"
#include "compatibility.h"
#include <cstdlib>

#include "GL/importgl.h"

GlTexture::GlTexture(const unsigned char *imageData, GLuint type, int width, int height, GLuint dataType) {
	glGenTextures(1, &_id);
	this->activate();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, DEFAULT_LINEAR_FILTER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, DEFAULT_LINEAR_FILTER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	if(dataType == 0) dataType = type;
	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, dataType, GL_UNSIGNED_BYTE, imageData);
	this->deactivate();

	_width = width;
	_height = height;
}

GlTexture::~GlTexture() {
	Log::logger << Log::debug << "gltexture delete";
	if(_id != 0) {
		glDeleteTextures(1, &_id);
		_id = 0;
	}
}

void GlTexture::setSegment(const unsigned char *bytes, int x, int y, int width, int height, ImageType type) {
	int format = 0;
	switch(type) {
	case RGB: format = GL_RGB; break;
	case RGBA: format = GL_RGBA; break;
	case ALPHA: format = GL_ALPHA; break;
	case LUMINANCE: format = GL_LUMINANCE; break;
	case LUMINANCE_ALPHA: format = GL_LUMINANCE_ALPHA; break;
	}

	this->activate();
	glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, format, GL_UNSIGNED_BYTE, bytes);
	this->deactivate();
}

unsigned int GlTexture::getId() const {
	return _id;
}

bool GlTexture::isValidTexture() const {
	return glIsTexture(_id);
}

void GlTexture::activate() const {
	glBindTexture(GL_TEXTURE_2D, _id);
}

void GlTexture::deactivate() const {
	GlTexture::unbind();
}

void GlTexture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}
