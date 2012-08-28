#include "GlTextureManager.h"
#include "TgaTexture.h"
#include "compatibility.h"
#include <string>
#include <iostream>

GlTextureManager::GlTextureManager(const char *texturesPath) : TextureManager(texturesPath) {
}

TextureManager * GlTextureManager::instance(const char *texturesPath) {
	if(!_TextureManagerInstance)
		_TextureManagerInstance = new GlTextureManager(texturesPath);
	return _TextureManagerInstance;
}

Texture * GlTextureManager::loadTexture(const char *textureFilePath) {
	static const Extension AllowedExtensions[] = {
		{ ".tga", reinterpret_cast<GlTexture::LoaderMethod>(&TgaTexture::loadGl) },
		{ NULL, NULL }
	};

	std::string path(textureFilePath);
	for(unsigned int i = 0; AllowedExtensions[i].name != NULL
		&& AllowedExtensions[i].method != NULL; i++) {
		std::string fullpath = path + AllowedExtensions[i].name;
		GlTexture *texture = AllowedExtensions[i].method(fullpath.c_str());
		std::cout << "Texture (" << fullpath << ") " << (texture ? "created" : "is null") << std::endl;
		if(texture) return texture;
	}
	return NULL;
}

