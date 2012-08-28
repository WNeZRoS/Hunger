#include "TextureManager.h"
#include <string>
#include <cstring>
#include <iostream>

TextureManager *TextureManager::_TextureManagerInstance = 0;

TextureManager * TextureManager::instance() {
	return _TextureManagerInstance;
}

TextureManager::TextureManager(const char *texturesPath) {
	if(texturesPath) {
		_texturesPath = const_cast<char*>(texturesPath);
	} else {
		_texturesPath = new char[3];
		std::strcpy(_texturesPath, "./");
	}
}

TextureManager::~TextureManager() {
	for(std::vector<TextureInfo>::iterator it = _textures.begin(); it != _textures.end(); it++) {
		TextureInfo info = *it;
		delete info.texture;
	}

	_textures.clear();
}

Texture * TextureManager::load(const char *textureName) {
	for(std::vector<TextureInfo>::iterator it = _textures.begin(); it != _textures.end(); it++) {
		TextureInfo& info = *it;
		if(std::strcmp(info.texturename, textureName) == 0) {
			info.usage++;
			std::cout << "find loaded texture " << info.usage << std::endl;
			return info.texture;
		}
	}

	std::string texturePath(_texturesPath);
	texturePath += textureName;

	Texture *texture = loadTexture(texturePath.c_str());
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
	for(std::vector<TextureInfo>::iterator it = _textures.begin();
		it != _textures.end(); it++) {
		TextureInfo& info = *it;
		if(info.texture == texture) {
			info.usage--;
			std::cout << "delete " << info.texturename << " for " << info.usage << std::endl;
			if(info.usage == 0) {
				delete info.texture;
				_textures.erase(it, it);
			}
		}
	}
}
