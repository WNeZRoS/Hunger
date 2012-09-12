#ifndef FREETYPEFONT_H
#define FREETYPEFONT_H

#include "Font.h"
#include "Texture.h"
#include <vector>

// Free type incudes
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

class FreeTypeFont : public Font
{
public:
	FreeTypeFont(const char *font, int size, bool monoSpace = false);
	~FreeTypeFont();

	Texture * renderToTexture(const XCHAR *text, int width, int height, Align align);
	void render(const XCHAR *text, float x, float y, float width, float height, Align align);

private:

	struct CacheElement
	{
		XCHAR symbol;
		unsigned short width;
		unsigned short top;
	};

	std::vector<CacheElement> _cache;
	Texture *_cacheTexture;
	static const unsigned int _CacheSize = 512;
	static const unsigned int _CacheSegmentSize = 16;
	static const unsigned int NO_GLYPH = -1;

	static int _FreeTypeUsages;
	static FT_Library _FreeType;
	FT_Face _face;
	bool _monoSpace;

	inline int toPowerOf2(int a) const;
	unsigned int cacheGlyph(XCHAR symbol);
	void getTextSizes(const XCHAR *text, int& width, int& height) const;
};

#endif // FREETYPEFONT_H
