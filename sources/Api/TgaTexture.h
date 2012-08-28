#ifndef TGAGLTEXTURE_H
#define TGAGLTEXTURE_H

class Texture;

class TgaTexture
{
public:
	static Texture * loadGl(const char *filename);
};

#endif // TGAGLTEXTURE_H
