#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "Texture.h"
#include <list>

#define LOAD_TEXTURE(name) TextureManagerWrapper::load(name, __FILE__, __LINE__)
#define UNLOAD_TEXTURE(texture) TextureManagerWrapper::unload(texture, __FILE__, __LINE__)

class TextureManagerWrapper {
public:
	static Texture * load(const char *texturename, const char *sourcename, int line);
	static void unload(const Texture* texture, const char *sourcename, int line);
};

class TextureManager
{
public:
	/** Get instance of TextureManager.
		Instance must be created by child classes.

	  @return Instance of TextureManager
	  */
	static TextureManager * instance();

	~TextureManager();

	friend class TextureManagerWrapper;
protected:
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
	std::list<TextureInfo> _textures;
	static TextureManager *_TextureManagerInstance;
};

#endif // TEXTUREMANAGER_H
