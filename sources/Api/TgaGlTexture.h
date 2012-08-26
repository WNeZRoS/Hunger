#ifndef TGAGLTEXTURE_H
#define TGAGLTEXTURE_H

#include "GlTexture.h"

class TgaGlTexture : public GlTexture {
public:
	static GlTexture* load(const char * filename, bool linearFilter = false);

private:
	TgaGlTexture();
};

#endif // TGAGLTEXTURE_H
