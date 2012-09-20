#include "Texture.h"
#include "TextureAtlas.h"
#include "TextureManager.h"
#include "Logger.h"
#include "compatibility.h"

Texture::Texture() {
	_atlas = 0;
	_atlasUsage = 0;
}

Texture::~Texture() {
	Log::logger << Log::debug << "Texture delete";
	if(_atlas) delete _atlas;
	_atlas = 0;
}

int Texture::getWidth() const {
	return _width;
}

int Texture::getHeight() const {
	return _height;
}

TextureAtlas * Texture::toAtlas(int rowSize, int columnSize) const {
	if(!_atlas) {
		_atlas = TextureAtlas::create(this, rowSize, columnSize);
		_atlasUsage = 0;
	}
	_atlasUsage++;
	return _atlas;
}

void Texture::atlasUnload() const {
	_atlasUsage--;
	if(_atlasUsage == 0) {
		delete _atlas;
		_atlas = 0;
	} else if(_atlasUsage < 0)
		Log::logger << Log::warning << "Atlas usage is lower that zero";
}
