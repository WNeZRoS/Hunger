LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := hunger

LOCAL_CFLAGS := -DANDROID_NDK -DDEBUG -g -ggdb -O0

LOCAL_SRC_FILES := \
    Food.cpp \
	LevelMap.cpp \
	main.cpp \
	Player.cpp \
	StickHud.cpp \
	Api/AbstractClasses.cpp \
	Api/compatibility.cpp \
	Api/Control.cpp \
	Api/FileManager.cpp \
	Api/GlAndroidContext.cpp \
	Api/GlRender.cpp \
	Api/GlTexture.cpp \
	Api/GlTextureManager.cpp \
	Api/Logger.cpp \
	Api/Point.cpp \
	Api/Random.cpp \
	Api/Sprite.cpp \
	Api/Texture.cpp \
	Api/TextureAtlas.cpp \
	Api/TextureManager.cpp \
	Api/TgaTexture.cpp \
	Api/TileSprite.cpp \
	Api/World.cpp

LOCAL_LDLIBS := -lGLESv1_CM -llog

include $(BUILD_SHARED_LIBRARY)
