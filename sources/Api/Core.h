#ifndef CORE_HH
#define CORE_HH

// General
#include "Core/FileManager.h"
#include "Core/Logger.h"
#include "Core/Random.h"
#include "Core/Thread.h"

// Graphics
#include "Core/Context.h"
#include "Core/Render.h"
#include "Core/Control.h"
#include "Core/Sprite.h"
#include "Core/Texture.h"
#include "Core/TextureAtlas.h"
#include "Core/TextureManager.h"
#include "Core/TileSprite.h"

#ifdef OPENGL
	#ifdef WIN32
		#include "Core/GlWindowsContext.h"
		typedef GlWindowsContext AppContext;
	#endif
	#ifdef ANDROID_NDK
		#include "Core/GlAndroidContext.h"
		typedef GlAndroidContext AppContext;
	#endif
	#include "Core/GlTexture.h"

	typedef GlTexture XTexture;
#else
	typedef Context AppContext;
	typedef Texture XTexture;
#endif

#endif // CORE_HH
