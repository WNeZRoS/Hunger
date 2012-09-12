#define IMPORTGL_API
#define IMPORTGL_FNPTRINIT = NULL

#include "importgl.h"

#define GET_PROC_ADDR wglGetProcAddress
#define IMPORT_FUNC(funcName) { \
	void * procAddress = (void *)GET_PROC_ADDR(#funcName); \
	if (procAddress == NULL) result = 0; \
	*((void **)&funcName) = procAddress; }

namespace ext
{
int loadExts() {
	int result = 0;

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
	return result;
}
}
