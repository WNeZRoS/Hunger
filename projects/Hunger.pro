TEMPLATE = app
CONFIG += console
CONFIG -= qt

win32:LIBS += -L../libs/lib-x86 -lopengl32 -lgdi32 -lfreetype -static
unix:LIBS += -lgl -lfreetype

SOURCES += \
    ../sources/Api/GlRender.cpp \
    ../sources/Api/Sprite.cpp \
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
    ../sources/Player.cpp \
    ../sources/Api/TextureManager.cpp \
    ../sources/Api/GlTextureManager.cpp \
    ../sources/Api/TgaTexture.cpp \
    ../sources/Api/Logger.cpp \
    ../sources/Food.cpp \
    ../sources/Api/Point.cpp \
    ../sources/Api/Texture.cpp \
    ../sources/Api/Random.cpp \
    ../sources/StickHud.cpp \
    ../sources/Api/GlAndroidContext.cpp \
    ../sources/Api/FileManager.cpp \
    ../sources/GL/importgl.cpp \
    ../sources/Api/FreeTypeFont.cpp \
    ../sources/Api/Interface.cpp \
    ../sources/Api/View.cpp \
    ../sources/GameInterface.cpp \
    ../sources/Api/TextView.cpp \
    ../sources/Api/ButtonView.cpp

HEADERS += \
    ../sources/Api/Render.h \
    ../sources/Api/GlRender.h \
    ../sources/Api/Context.h \
    ../sources/Api/Texture.h \
    ../sources/Api/compatibility.h \
    ../sources/Api/Sprite.h \
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
    ../sources/Player.h \
    ../sources/Api/TextureManager.h \
    ../sources/Api/GlTextureManager.h \
    ../sources/Api/TgaTexture.h \
    ../sources/Api/Logger.h \
    ../sources/Food.h \
    ../sources/Api/Point.h \
    ../sources/Api/Random.h \
    ../sources/StickHud.h \
    ../sources/GL/importgl.h \
    ../sources/Api/GlAndroidContext.h \
    ../sources/Api/FileManager.h \
    ../sources/Api/Font.h \
    ../sources/Api/FreeTypeFont.h \
    ../sources/Api/Interface.h \
    ../sources/Api/View.h \
    ../sources/GameInterface.h \
    ../sources/Api/TextView.h \
    ../sources/Api/ButtonView.h

INCLUDEPATH += ../sources ../libs/include
Debug: QMAKE_CXXFLAGS += -DDEBUG
