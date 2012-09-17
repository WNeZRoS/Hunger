#ifndef FONT_H
#define FONT_H
#include "compatibility.h"
#include "Texture.h"

class Font
{
public:
	const static int DEFAULT = 0;

	enum Align {
		LEFT,
		CENTER,
		RIGHT
	};

	virtual ~Font() = 0;
	virtual Texture * renderToTexture(const XCHAR *text, int width = DEFAULT,
						int height = DEFAULT, Align align = LEFT) = 0;
	virtual void render(const XCHAR *text, float x, float y, float width = DEFAULT,
						float height = DEFAULT, Align align = LEFT) = 0;
};

#endif // FONT_H
