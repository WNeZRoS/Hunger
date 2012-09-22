LOCAL_PATH := $(call my-dir)

include $(LOCAL_PATH)/Freetype.mk

include $(CLEAR_VARS)

LOCAL_MODULE := hunger
LOCAL_CFLAGS := -DANDROID_NDK -DDEBUG -g -ggdb -O0
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../sources $(LOCAL_PATH)/../../../libs/include

LOCAL_SRC_FILES := \
	../../../sources/Api/GlRender.cpp \
	../../../sources/Api/Sprite.cpp \
	../../../sources/Api/GlTexture.cpp \
	../../../sources/Api/TextureAtlas.cpp \
	../../../sources/Api/AbstractClasses.cpp \
	../../../sources/Api/TileSprite.cpp \
	../../../sources/Api/compatibility.cpp \
	../../../sources/Api/Control.cpp \
	../../../sources/Api/World.cpp \
	../../../sources/Api/TextureManager.cpp \
	../../../sources/Api/GlTextureManager.cpp \
	../../../sources/Api/TgaTexture.cpp \
	../../../sources/Api/Logger.cpp \
	../../../sources/Api/Point.cpp \
	../../../sources/Api/Texture.cpp \
	../../../sources/Api/Random.cpp \
	../../../sources/Api/GlAndroidContext.cpp \
	../../../sources/Api/FileManager.cpp \
	../../../sources/Api/FreeTypeFont.cpp \
	../../../sources/Api/Interface.cpp \
	../../../sources/Api/View.cpp \
	../../../sources/Api/TextView.cpp \
	../../../sources/Api/ButtonView.cpp \
	../../../sources/Api/Thread.cpp \
	../../../sources/GL/importgl.cpp \
	../../../sources/GameInterface.cpp \
	../../../sources/StickHud.cpp \
	../../../sources/Food.cpp \
	../../../sources/LevelMap.cpp \
	../../../sources/main.cpp \
	../../../sources/Player.cpp \
	../../../sources/Monster.cpp \
	../../../sources/Intelligence.cpp \
	../../../sources/GameWorld.cpp

LOCAL_LDLIBS := -ldl -lGLESv1_CM -llog
LOCAL_STATIC_LIBRARIES := freetype

include $(BUILD_SHARED_LIBRARY)
