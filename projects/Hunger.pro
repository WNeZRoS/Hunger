TEMPLATE = app
CONFIG += console
CONFIG -= qt

win32:LIBS += -L../libs/lib-x86 -lopengl32 -lgdi32 -lfreetype -lpthread -static
unix:LIBS += -lgl -lfreetype -lpthread

SOURCES += \
    ../sources/Api/AbstractClasses.cpp \
    ../sources/Api/compatibility.cpp \
	../sources/LevelMap.cpp \
	../sources/main.cpp \
    ../sources/Player.cpp \
    ../sources/Food.cpp \
    ../sources/StickHud.cpp \
    ../sources/GL/importgl.cpp \
    ../sources/GameInterface.cpp \
    ../sources/Monster.cpp \
    ../sources/Intelligence.cpp \
    ../sources/GameWorld.cpp \
    ../sources/Api/Core/TileSprite.cpp \
    ../sources/Api/Core/Thread.cpp \
    ../sources/Api/Core/TgaTexture.cpp \
    ../sources/Api/Core/TextureManager.cpp \
    ../sources/Api/Core/TextureAtlas.cpp \
    ../sources/Api/Core/Texture.cpp \
    ../sources/Api/Core/Sprite.cpp \
    ../sources/Api/Core/Random.cpp \
    ../sources/Api/Core/Logger.cpp \
    ../sources/Api/Core/GlWindowsContext.cpp \
    ../sources/Api/Core/GlTextureManager.cpp \
    ../sources/Api/Core/GlTexture.cpp \
    ../sources/Api/Core/GlRender.cpp \
    ../sources/Api/Core/GlAndroidContext.cpp \
    ../sources/Api/Core/FileManager.cpp \
    ../sources/Api/Core/Control.cpp \
    ../sources/Api/Content/World.cpp \
    ../sources/Api/Content/Point.cpp \
    ../sources/Api/Interface/View.cpp \
    ../sources/Api/Interface/TextView.cpp \
    ../sources/Api/Interface/Interface.cpp \
    ../sources/Api/Interface/FreeTypeFont.cpp \
    ../sources/Api/Interface/ButtonView.cpp

HEADERS += \
    ../sources/Api/compatibility.h \
    ../sources/LevelMap.h \
    ../sources/main.h \
    ../sources/Player.h \
    ../sources/Food.h \
    ../sources/StickHud.h \
    ../sources/GL/importgl.h \
    ../sources/GameInterface.h \
    ../sources/Monster.h \
    ../sources/Intelligence.h \
	../sources/GameWorld.h \
    ../sources/Api/Core/TileSprite.h \
    ../sources/Api/Core/Thread.h \
    ../sources/Api/Core/TgaTexture.h \
    ../sources/Api/Core/TextureManager.h \
    ../sources/Api/Core/TextureAtlas.h \
    ../sources/Api/Core/Texture.h \
    ../sources/Api/Core/Sprite.h \
    ../sources/Api/Core/Render.h \
    ../sources/Api/Core/Random.h \
    ../sources/Api/Core/Logger.h \
    ../sources/Api/Core/GlWindowsContext.h \
    ../sources/Api/Core/GlTextureManager.h \
    ../sources/Api/Core/GlTexture.h \
    ../sources/Api/Core/GlRender.h \
    ../sources/Api/Core/GlAndroidContext.h \
    ../sources/Api/Core/FileManager.h \
    ../sources/Api/Core/Control.h \
    ../sources/Api/Core/Context.h \
    ../sources/Api/Content/World.h \
    ../sources/Api/Content/Point.h \
    ../sources/Api/Content/Map.h \
    ../sources/Api/Content/Entity.h \
    ../sources/Api/Interface/View.h \
    ../sources/Api/Interface/TextView.h \
    ../sources/Api/Interface/Interface.h \
    ../sources/Api/Interface/FreeTypeFont.h \
    ../sources/Api/Interface/Font.h \
    ../sources/Api/Interface/ButtonView.h \
    ../sources/Api/Core.h \
    ../sources/Api/Content.h \
    ../sources/Api/Interface.h

INCLUDEPATH += ../sources ../libs/include
Debug: QMAKE_CXXFLAGS += -DDEBUG
QMAKE_CXXFLAGS += -Wall -O2 #-Wold-style-cast #-Weffc++
