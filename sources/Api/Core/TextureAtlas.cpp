#include "TextureAtlas.h"
#include "Render.h"
#include "TextureManager.h"
#include "../compatibility.h"
#include <cmath>
#include "Logger.h"

TextureAtlas::Loader::Loader(const Texture::Name texture, int rows, int columns) {
	this->texture = texture;
	this->rows = rows;
	this->columns = columns;
}

TextureAtlas * TextureAtlas::Loader::load() const {
	if(rows <= 0 || columns <= 0 || !texture) return NULL;
	Texture * t = LOAD_TEXTURE(texture);
	if(!t) return NULL;
	return t->toAtlas(rows, columns);
}

TextureAtlas * TextureAtlas::create(const Texture *texture, int tilesInRow, int tilesInColumn) {
	if(tilesInRow <= 0 || tilesInColumn <= 0 || !texture) return NULL;
	return new TextureAtlas(texture, tilesInRow, tilesInColumn);
}

TextureAtlas::TextureAtlas(const Texture *texture, int tilesInRow, int tilesInColumn) {
	_tileWidth = 1.0f / tilesInRow;
	_tileHeight = 1.0f / tilesInColumn;
	_texture = texture;
}

TextureAtlas::~TextureAtlas() {
	_texture->_atlas = 0;
	Log::logger << Log::debug << "atlas deleted";
}

void TextureAtlas::unload() const {
	_texture->atlasUnload();
	UNLOAD_TEXTURE(_texture);
}

void TextureAtlas::drawTile(int tileId, float x, float y, float z, float width, float height) const {
	if(tileId < 0) return;

	float tileX = _tileWidth * tileId;
	float tileY = _tileHeight * floor(tileX);
	if(tileY > 1) return;
	tileX -= floor(tileX);

	_texture->activate();
	Render::getPainter()->rectx(x, y, x + width, y + height, z, tileX, tileY, tileX + _tileWidth, tileY + _tileHeight);
}

