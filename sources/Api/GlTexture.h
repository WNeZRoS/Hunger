#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include "Texture.h"

#ifndef DEFAULT_LINEAR_FILTER
	#define DEFAULT_LINEAR_FILTER GL_LINEAR
#endif

class GlTextureAtlas;

class GlTexture : public Texture
{
public:
	typedef GlTexture * (*LoaderMethod)(const char *filename);

	GlTexture(const char *imageData, unsigned int type, int width, int height);
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
