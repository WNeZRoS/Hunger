#include "TextureAtlas.h"
#include "Render.h"
#include <cmath>
#include <iostream>

TextureAtlas::TextureAtlas(const Texture *texture, int tilesInRow, int tilesInColumn) {
	_tileWidth = 1.0f / tilesInRow;
	_tileHeight = 1.0f / tilesInColumn;
	_texture = texture;
}

void TextureAtlas::drawTile(int tileId, int x, int y, int z, int width, int height) const {
	if(tileId < 0) return;

	float tileX = _tileWidth * tileId;
	float tileY = _tileHeight * floor(tileX);
	if(tileY > 1) return;
	tileX -= floor(tileX);

	_texture->activate();
	Render::getPainter()->rectx(x, y, x + width, y + height, z, tileX, tileY, tileX + _tileWidth, tileY + _tileHeight);
}

