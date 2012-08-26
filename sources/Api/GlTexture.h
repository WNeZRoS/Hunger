#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include "Texture.h"

class GlTextureAtlas;

class GlTexture : public Texture {
public:
	~GlTexture();

	void activate() const;
	void deactivate() const;

	TextureAtlas * toAtlas(int rowSize, int columnSize) const;

protected:
	unsigned int _id;

	GlTexture();

	unsigned int getId() const;
};

#endif // GLTEXTURE_H
