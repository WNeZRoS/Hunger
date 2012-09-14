#define IMPORTGL_API
#define IMPORTGL_FNPTRINIT = NULL

#include "importgl.h"
#include "../Api/Logger.h"

#ifdef ANDROID_NDK
#include <dlfcn.h>
#endif

namespace ext
{
int loadExts() {
	int result = 0;

#ifdef WIN32
#define IMPORT_FUNC(funcName) { \
	void * procAddress = (void *)wglGetProcAddress(#funcName); \
	if (procAddress == NULL) { Log::Debug << "Fail load " << #funcName; result++;} \
	*((void **)&funcName) = procAddress; }
#endif

#ifdef ANDROID_NDK
	void *sGLESSO = dlopen("libGLESv1_CM.so", RTLD_NOW);
	if(sGLESSO == NULL) return -1; // Cannot find OpenGL ES Common or Common Lite SO.

#define GLESFN(funcName) (#funcName "OES")

#define IMPORT_FUNC(funcName) { \
	void *procAddress = (void *)dlsym(sGLESSO, GLESFN(funcName)); \
	if (procAddress == NULL) { Log::Debug << "Fail load " << GLESFN(funcName); result++;} \
	*((void **)&funcName) = procAddress; }
#endif // ANDROID_NDK

	IMPORT_FUNC(glIsRenderbuffer);
	IMPORT_FUNC(glBindRenderbuffer);
	IMPORT_FUNC(glDeleteRenderbuffers);
	IMPORT_FUNC(glGenRenderbuffers);
	IMPORT_FUNC(glRenderbufferStorage);
	IMPORT_FUNC(glGetRenderbufferParameteriv);
	IMPORT_FUNC(glIsFramebuffer);
	IMPORT_FUNC(glBindFramebuffer);
	IMPORT_FUNC(glDeleteFramebuffers);
	IMPORT_FUNC(glGenFramebuffers);
	IMPORT_FUNC(glCheckFramebufferStatus);
	IMPORT_FUNC(glFramebufferTexture1D);
	IMPORT_FUNC(glFramebufferTexture2D);
	IMPORT_FUNC(glFramebufferTexture3D);
	IMPORT_FUNC(glFramebufferRenderbuffer);
	IMPORT_FUNC(glGetFramebufferAttachmentParameteriv);
	IMPORT_FUNC(glGenerateMipmap);

#ifdef ANDROID_NDK
	dlclose(sGLESSO);
#endif

	return result;
}
}
