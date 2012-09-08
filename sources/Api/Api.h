#ifndef API_H
#define API_H

#include "Context.h"
#include "Render.h"
#include "Control.h"
#include "Entity.h"
#include "Logger.h"
#include "Map.h"
#include "Point.h"
#include "Random.h"
#include "Sprite.h"
#include "Texture.h"
#include "TextureAtlas.h"
#include "TextureManager.h"
#include "TileSprite.h"
#include "World.h"

#ifdef OPENGL
	#ifdef WIN32
	#include "GlWindowsContext.h"
	#endif
#endif

#endif // API_H
