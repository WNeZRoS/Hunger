#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H

#include "Texture.h"

class TextureAtlas {
public:
	TextureAtlas(const Texture *texture, int tilesInRow, int tilesInColumn);
	~TextureAtlas();

	void drawTile(int tileId, int x, int y, int z = 0,
						  int width = 1, int height = 1) const;

protected:
	float _tileWidth, _tileHeight;
	const Texture *_texture;
};

#endif // TEXTUREATLAS_H
