#include "GlTextureManager.h"
#include "TgaTexture.h"
#include "Logger.h"
#include "compatibility.h"
#include <string>

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
		Log::logger << Log::debug << "Texture (" << fullpath << ") " << (texture ? "created" : "is null");
		if(texture) return texture;
	}
	return NULL;
}

