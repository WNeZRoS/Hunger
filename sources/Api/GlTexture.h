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
	GlTexture(const unsigned char *imageData, unsigned int type, int width, int height, unsigned int dataType = 0);
	~GlTexture();

	void setSegment(const unsigned char *bytes, int x, int y, int width, int height, ImageType type);

	void activate() const;
	void deactivate() const;
	static void unbind();
	bool isValidTexture() const;

	friend class GlRender;
protected:
	unsigned int _id;

	unsigned int getId() const;
};

#endif // GLTEXTURE_H
