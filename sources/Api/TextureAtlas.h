#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H

#include "Texture.h"

class TextureAtlas
{
public:
	static TextureAtlas * create(const Texture *texture, int tilesInRow, int tilesInColumn);
	~TextureAtlas();

	void drawTile(int tileId, float x, float y, float z = 0,
						  float width = 1, float height = 1) const;

protected:
	float _tileWidth, _tileHeight;
	const Texture *_texture;

	TextureAtlas(const Texture *texture, int tilesInRow, int tilesInColumn);
};

#endif // TEXTUREATLAS_H
