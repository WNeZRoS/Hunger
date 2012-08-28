#include "TextureAtlas.h"
#include "Render.h"
#include "TextureManager.h"
#include <cmath>
#include <iostream>

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
	TextureManager::instance()->unload(_texture);
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

