#include "Context.h"
#include "Render.h"
#include "Texture.h"
#include "Map.h"
#include "TextureAtlas.h"
#include "Entity.h"

Render::Painter * Render::_Painter = 0;

Context::~Context() { }
Render::~Render() { }
Render::Painter::~Painter() { }
Texture::~Texture() { }
Map::~Map() { }
TextureAtlas::~TextureAtlas() { }
Entity::~Entity() { }
