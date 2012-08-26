#include "GlTexture.h"
#include "TextureAtlas.h"

#include <GL/gl.h>
#include <iostream>

GlTexture::GlTexture() {
	_id = 0;
	std::cout << "GlTexture " << _id << std::endl;
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
	std::cout << "GlTexture to Atlas " << _id << std::endl;
	return new TextureAtlas(this, rowSize, columnSize);
}
