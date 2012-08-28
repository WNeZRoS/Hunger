#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "Texture.h"
#include <vector>

class TextureManager
{
public:
	/** Get instance of TextureManager.
		Instance must be created by child classes.

	  @return Instance of TextureManager
	  */
	static TextureManager * instance();

	~TextureManager();

	/** Load texture from texture path.

		@param textureName
		Name of texture to load

		@return Instane of texture or NULL if texture not loaded.
	  */
	Texture * load(const char *textureName);

	void unload(const Texture *texture);

protected:
	struct TextureInfo
	{
		const char *texturename;
		Texture *texture;
		unsigned int usage;
	};

	TextureManager(const char *texturesPath = "./");
	virtual Texture * loadTexture(const char *textureFilePath);

	char *_texturesPath;
	std::vector<TextureInfo> _textures;
	static TextureManager *_TextureManagerInstance;
};

#endif // TEXTUREMANAGER_H
