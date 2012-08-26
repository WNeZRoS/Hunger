TEMPLATE = app
CONFIG += console
CONFIG -= qt
LIBS += -lopengl32 -lgdi32

SOURCES += \
    ../sources/Api/GlRender.cpp \
    ../sources/Api/Sprite.cpp \
    ../sources/Api/TgaGlTexture.cpp \
    ../sources/Api/GlTexture.cpp \
    ../sources/Api/TextureAtlas.cpp \
    ../sources/Api/GlWindowsContext.cpp \
    ../sources/Api/AbstractClasses.cpp \
    ../sources/Api/TileSprite.cpp \
    ../sources/Api/compatibility.cpp \
    ../sources/Api/Control.cpp \
	../sources/Api/World.cpp \
	../sources/LevelMap.cpp \
	../sources/main.cpp \
    ../sources/Player.cpp

HEADERS += \
    ../sources/Api/Render.h \
    ../sources/Api/GlRender.h \
    ../sources/Api/Context.h \
    ../sources/Api/Texture.h \
    ../sources/Api/compatibility.h \
    ../sources/Api/Sprite.h \
    ../sources/Api/TgaGlTexture.h \
    ../sources/Api/GlTexture.h \
    ../sources/Api/TextureAtlas.h \
    ../sources/Api/GlWindowsContext.h \
    ../sources/Api/Map.h \
	../sources/Api/Entity.h \
    ../sources/Api/TileSprite.h \
    ../sources/Api/Control.h \
	../sources/Api/Api.h \
	../sources/Api/World.h \
    ../sources/LevelMap.h \
    ../sources/main.h \
    ../sources/Player.h

INCLUDEPATH += ../sources

