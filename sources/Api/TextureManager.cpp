#include "TextureManager.h"
#include "Logger.h"
#include "compatibility.h"
#include <string>
#include <cstring>

TextureManager *TextureManager::_TextureManagerInstance = 0;

TextureManager * TextureManager::instance() {
	return _TextureManagerInstance;
}

TextureManager::TextureManager() {
}

TextureManager::~TextureManager() {
	for(std::list<TextureInfo>::iterator it = _textures.begin(); it != _textures.end(); it++) {
		TextureInfo info = *it;
		delete info.texture;
	}
	_textures.clear();
}

Texture * TextureManager::load(const char *textureName) {
	for(std::list<TextureInfo>::iterator it = _textures.begin(); it != _textures.end(); it++) {
		TextureInfo& info = *it;
		if(std::strcmp(info.texturename, textureName) == 0) {
			if(!info.texture->isValidTexture()) {
				delete info.texture;
				info.texture = loadTexture(textureName);
				info.usage = 1;
			} else info.usage++;
			Log::logger << Log::debug << "find loaded texture " << info.usage;
			return info.texture;
		}
	}

	Texture *texture = loadTexture(textureName);
	if(texture) {
		TextureInfo info;
		info.texture = texture;
		info.texturename = textureName;
		info.usage = 1;
		_textures.push_back(info);
	}
	return texture;
}

Texture * TextureManager::loadTexture(const char *textureFilePath) {
	return NULL;
}

void TextureManager::unload(const Texture *texture) {
	for(std::list<TextureInfo>::iterator it = _textures.begin(); it != _textures.end(); it++) {
		TextureInfo& info = *it;
		if(info.texture == texture) {
			info.usage--;
			Log::logger << Log::debug << "unuse " << info.texturename << " for " << info.usage;
			if(info.usage == 0) {
				Log::logger << Log::info << "Delete texture " << info.texturename;
				delete info.texture;
				_textures.erase(it);
			}
			break;
		}
	}
}

Texture * TextureManagerWrapper::load(const char *texturename, const char *sourcename, int line) {
	Log::logger << Log::debug << "Load texture '" << texturename << "' from "
				<< (sourcename == NULL ? "unksrc" : sourcename) << ":" << line;
	return TextureManager::instance()->load(texturename);
}

void TextureManagerWrapper::unload(const Texture* texture, const char *sourcename, int line) {
	Log::logger << Log::debug << "Unload texture from "
				<< (sourcename == NULL ? "unksrc" : sourcename) << ":" << line;
	return TextureManager::instance()->unload(texture);
}
