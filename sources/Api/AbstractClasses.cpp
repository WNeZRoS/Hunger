#include "Context.h"
#include "Render.h"
#include "Map.h"
#include "Entity.h"
#include "Font.h"

Render::Painter *Render::_Painter = 0;

Context::~Context() { }
Render::~Render() { }
Render::Painter::~Painter() { }
Map::~Map() { }
Entity::~Entity() { }
Font::~Font() { }
