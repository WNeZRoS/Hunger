#ifndef TEXTUREATLAS_H
#define TEXTUREATLAS_H

#include "Texture.h"

class TextureAtlas
{
public:

	struct Loader
	{
		Texture::Name texture;
		int rows, columns;

		Loader(const Texture::Name texture, int rows, int columns);
		TextureAtlas * load() const;
	};

	void unload() const;
	void drawTile(int tileId, float x, float y, float z = 0,
						  float width = 1, float height = 1) const;

	friend class Texture;
protected:
	float _tileWidth, _tileHeight;
	const Texture *_texture;

	static TextureAtlas * create(const Texture *texture, int tilesInRow, int tilesInColumn);
	TextureAtlas(const Texture *texture, int tilesInRow, int tilesInColumn);
	~TextureAtlas();
};

#endif // TEXTUREATLAS_H
