#ifndef GLTEXTUREMANAGER_H
#define GLTEXTUREMANAGER_H

#include "TextureManager.h"
#include "GlTexture.h"

class GlTextureManager : public TextureManager
{
public:
	static TextureManager * instance();

private:
	struct Extension
	{
		const char *name;
		GlTexture::LoaderMethod method;
	};

	GlTextureManager();
	Texture * loadTexture(const char *textureFilePath);
};

#endif // GLTEXTUREMANAGER_H
