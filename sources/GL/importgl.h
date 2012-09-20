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

#ifndef IMPORTGL_API
#define IMPORTGL_API extern
#endif
#ifndef IMPORTGL_FNPTRINIT
#define IMPORTGL_FNPTRINIT
#endif

#ifndef _MSC_VER
#define FNDEF(retType, funcName, args) IMPORTGL_API retType (*funcName) args IMPORTGL_FNPTRINIT
#else
#define FNTYPE(funcName) funcName ## _type
#define FNDEF(retType, funcName, args) \
	typedef retType (__stdcall *FNTYPE(funcName)) args; \
	IMPORTGL_API FNTYPE(funcName) funcName IMPORTGL_FNPTRINIT
#endif

namespace ext
{

#define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506
#define GL_MAX_RENDERBUFFER_SIZE 0x84E8
#define GL_FRAMEBUFFER_BINDING 0x8CA6
#define GL_RENDERBUFFER_BINDING 0x8CA7
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET 0x8CD4
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT 0x8CD8
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS 0x8CD9
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS 0x8CDA
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED 0x8CDD
#define GL_MAX_COLOR_ATTACHMENTS 0x8CDF
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_COLOR_ATTACHMENT1 0x8CE1
#define GL_COLOR_ATTACHMENT2 0x8CE2
#define GL_COLOR_ATTACHMENT3 0x8CE3
#define GL_COLOR_ATTACHMENT4 0x8CE4
#define GL_COLOR_ATTACHMENT5 0x8CE5
#define GL_COLOR_ATTACHMENT6 0x8CE6
#define GL_COLOR_ATTACHMENT7 0x8CE7
#define GL_COLOR_ATTACHMENT8 0x8CE8
#define GL_COLOR_ATTACHMENT9 0x8CE9
#define GL_COLOR_ATTACHMENT10 0x8CEA
#define GL_COLOR_ATTACHMENT11 0x8CEB
#define GL_COLOR_ATTACHMENT12 0x8CEC
#define GL_COLOR_ATTACHMENT13 0x8CED
#define GL_COLOR_ATTACHMENT14 0x8CEE
#define GL_COLOR_ATTACHMENT15 0x8CEF
#define GL_DEPTH_ATTACHMENT 0x8D00
#define GL_STENCIL_ATTACHMENT 0x8D20
#define GL_FRAMEBUFFER 0x8D40
#define GL_RENDERBUFFER 0x8D41
#define GL_RENDERBUFFER_WIDTH 0x8D42
#define GL_RENDERBUFFER_HEIGHT 0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT 0x8D44
#define GL_STENCIL_INDEX1 0x8D46
#define GL_STENCIL_INDEX4 0x8D47
#define GL_STENCIL_INDEX8 0x8D48
#define GL_STENCIL_INDEX16 0x8D49
#define GL_RENDERBUFFER_RED_SIZE 0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE 0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE 0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE 0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE 0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE 0x8D55

	FNDEF(GLboolean, glIsRenderbuffer, (GLuint));
	FNDEF(void, glBindRenderbuffer, (GLenum, GLuint));
	FNDEF(void, glDeleteRenderbuffers, (GLsizei, const GLuint *));
	FNDEF(void, glGenRenderbuffers, (GLsizei, GLuint *));
	FNDEF(void, glRenderbufferStorage, (GLenum, GLenum, GLsizei, GLsizei));
	FNDEF(void, glGetRenderbufferParameteriv, (GLenum, GLenum, GLint *));
	FNDEF(GLboolean, glIsFramebuffer, (GLuint));
	FNDEF(void, glBindFramebuffer, (GLenum, GLuint));
	FNDEF(void, glDeleteFramebuffers, (GLsizei, const GLuint *));
	FNDEF(void, glGenFramebuffers, (GLsizei, GLuint *));
	FNDEF(GLenum, glCheckFramebufferStatus, (GLenum));
	FNDEF(void, glFramebufferTexture1D, (GLenum, GLenum, GLenum, GLuint, GLint));
	FNDEF(void, glFramebufferTexture2D, (GLenum, GLenum, GLenum, GLuint, GLint));
	FNDEF(void, glFramebufferTexture3D, (GLenum, GLenum, GLenum, GLuint, GLint, GLint));
	FNDEF(void, glFramebufferRenderbuffer, (GLenum, GLenum, GLenum, GLuint));
	FNDEF(void, glGetFramebufferAttachmentParameteriv, (GLenum, GLenum, GLenum, GLint *));
	FNDEF(void, glGenerateMipmap, (GLenum));

	int loadExts();
}

#endif // IMPORTGL_H
