#ifndef API_H
#define API_H

#include "Context.h"
#include "Render.h"
#include "Control.h"
#include "Entity.h"
#include "FileManager.h"
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
		typedef GlWindowsContext AppContext;
	#endif
	#ifdef ANDROID_NDK
		#include "GlAndroidContext.h"
		typedef GlAndroidContext AppContext;
	#endif
	#include "GlTexture.h"

typedef GlTexture XTexture;
#else
	typedef Context AppContext;
	typedef Texture XTexture;
#endif

#include "compatibility.h"

#endif // API_H
