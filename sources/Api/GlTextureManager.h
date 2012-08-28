#ifndef GLTEXTUREMANAGER_H
#define GLTEXTUREMANAGER_H

#include "TextureManager.h"
#include "GlTexture.h"

class GlTextureManager : public TextureManager
{
public:
	/** Create instance of GlTextureManager or get instance of TextureManager.

	  @param texturesPath
	  Default path to find textures, used only for create instace and later not used.
	  Default value is `pwd`.

	  @return Instance of TextureManager
	  */
	static TextureManager * instance(const char *texturesPath = "./");

private:
	struct Extension
	{
		const char *name;
		GlTexture::LoaderMethod method;
	};

	GlTextureManager(const char *texturesPath = "./");
	Texture * loadTexture(const char *textureFilePath);
};

#endif // GLTEXTUREMANAGER_H
