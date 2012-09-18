#ifndef API_H
#define API_H

// Graphics
#include "Context.h"
#include "Render.h"
#include "Control.h"
#include "Sprite.h"
#include "Texture.h"
#include "TextureAtlas.h"
#include "TextureManager.h"
#include "TileSprite.h"

// General
#include "FileManager.h"
#include "Logger.h"
#include "Random.h"
#include "Thread.h"

// Game's content
#include "Entity.h"
#include "Map.h"
#include "Point.h"
#include "World.h"

// Game's interface
#include "Font.h"
#include "FreeTypeFont.h"
#include "Interface.h"
#include "View.h"
#include "ButtonView.h"
#include "TextView.h"

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
