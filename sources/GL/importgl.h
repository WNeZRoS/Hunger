#ifndef IMPORTGL_H
#define IMPORTGL_H

#ifdef WIN32
	#include <windows.h>
#endif

#ifdef ANDROID_NDK
	#include <GLES/gl.h>
	#define glOrtho glOrthof
	#define GL_CLAMP GL_CLAMP_TO_EDGE
#else
	#include <GL/gl.h>
#endif

#ifdef WIN32
	#include <GL/wglext.h>
#endif

#endif // IMPORTGL_H
