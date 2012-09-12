#include "FreeTypeFont.h"
#include "Render.h"
#include "Logger.h"
#include <stdexcept>

int FreeTypeFont::_FreeTypeUsages = 0;
FT_Library FreeTypeFont::_FreeType;

FreeTypeFont::FreeTypeFont(const char *font, int size, bool monoSpace) {
	if(_FreeTypeUsages == 0) {
		if(FT_Init_FreeType(&_FreeType)) throw std::runtime_error("FT_Init_FreeType failed");
	}

	_FreeTypeUsages++;

	if(FT_New_Face(_FreeType, font, 0, &_face)) throw std::runtime_error("Load font face failed");
	FT_Set_Char_Size(_face, size * 64, size * 64, 72, 72); // 72 -> 96

	Render::getPainter()->setRenderTarget(_CacheSize, _CacheSize);
	_cacheTexture = Render::getPainter()->getTexture();
	if(!_cacheTexture) throw std::runtime_error("Cache texture not created");

	_monoSpace = monoSpace;
}

FreeTypeFont::~FreeTypeFont() {
	delete _cacheTexture;
	_cache.clear();

	FT_Done_Face(_face);

	_FreeTypeUsages--;
	if(_FreeTypeUsages == 0) FT_Done_FreeType(_FreeType);
}

Texture * FreeTypeFont::renderToTexture(const XCHAR *text, int width, int height, Align align) {
	int ownWidth = width, ownHeight = height;
	getTextSizes(text, ownWidth, ownHeight);

	Render::getPainter()->setRenderTarget(ownWidth, ownHeight);
	render(text, 0, 0, ownWidth, ownHeight, align);
	return Render::getPainter()->getTexture();
}

void FreeTypeFont::render(const XCHAR *text, float x, float y, float width, float height, Align align) {
	int ownWidth = width, ownHeight = height;
	getTextSizes(text, ownWidth, ownHeight);

	float rx = x, ry = y;

	for(unsigned int i = 0; i < wcslen(text); i++) {
		XCHAR c = text[i];
		unsigned int index = 0;
		if(c > ' ') index = cacheGlyph(c);
		if(index == NO_GLYPH) continue;
		CacheElement letter = _cache.at(index);

		static int segmentsCount = (_CacheSize / _CacheSegmentSize);
		static float segment = 1.0f / segmentsCount;

		float tx = (index % segmentsCount);
		float ty = (index - tx) / segmentsCount;
		tx *= segment;
		ty *= segment;

		bool inv = c > _T(' ');

		_cacheTexture->activate();
		Render::getPainter()->rectx(rx, ry + letter.top, rx + _CacheSegmentSize,
									ry + _CacheSegmentSize + letter.top, 99.0f,
									tx, ty, tx + inv * segment, ty + inv * segment);

		rx += _monoSpace ? _CacheSegmentSize : (inv ? letter.width + 2 : _CacheSegmentSize / 4);
		if(rx - x >= ownWidth || c == _T('\n')) {
			rx = x;
			ry += _CacheSegmentSize;
		}
	}
	_cacheTexture->deactivate();
}

inline int FreeTypeFont::toPowerOf2(int a) const {
	int x;
	for(x = 1; x <= a; x <<= 1);
	return x;
}

unsigned int FreeTypeFont::cacheGlyph(XCHAR symbol) {
	unsigned int index = 0;
	for(; index < _cache.size(); index++) {
		if(_cache.at(index).symbol == symbol) return index;
	}

	if(FT_Load_Char(_face, symbol, FT_LOAD_RENDER)) return NO_GLYPH;

	FT_Glyph glyph;
	if(FT_Get_Glyph(_face->glyph, &glyph)) return NO_GLYPH;

	FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;
	FT_Bitmap& bitmap = bitmapGlyph->bitmap;

	unsigned char *data = new unsigned char[2 * _CacheSegmentSize * _CacheSegmentSize];
	for(unsigned int i = 0; i < 2 * _CacheSegmentSize * _CacheSegmentSize; i++) data[i] = 0;

	int xof = (_CacheSegmentSize - bitmap.width) / 2;
	if(xof > 0 && !_monoSpace) xof = 0;

	for(int h = 0; h < bitmap.rows && h < _CacheSegmentSize; h++) {
		for (int w = xof < 0 ? -xof : 0; w < bitmap.width; w++) {
			if(w + xof >= _CacheSegmentSize) break;
			int luminance = bitmap.buffer[(bitmap.rows - h - 1) * bitmap.width + w];
			data[(h * _CacheSegmentSize + w + xof) * 2] = 255;
			data[(h * _CacheSegmentSize + w + xof) * 2 + 1] = luminance;
		}
	}

	int x = (index % (_CacheSize / _CacheSegmentSize));
	int y = (index - x) / (_CacheSize / _CacheSegmentSize);

	if(y * _CacheSegmentSize > _CacheSize) return NO_GLYPH;
	_cacheTexture->setSegment(data, x * _CacheSegmentSize, (_CacheSize / _CacheSegmentSize - y - 1) * _CacheSegmentSize,
							  _CacheSegmentSize, _CacheSegmentSize, Texture::LUMINANCE_ALPHA);

	delete [] data;
	//Log::Debug << "Bitmap " << _face->glyph->bitmap_top << " " << bitmap.width << " " << bitmap.rows;
	CacheElement ce = { symbol, bitmap.width < _CacheSegmentSize ? bitmap.width : _CacheSegmentSize,
					  bitmap.rows - _face->glyph->bitmap_top};
	_cache.push_back(ce);
	return index;
}

void FreeTypeFont::getTextSizes(const XCHAR *text, int& width, int &height) const {
	if(width == DEFAULT) {
		for(int i = 0; text[i] != 0; i++) {
			if((text[i] == _T('\n') || text[i+1] == 0) && i - width > width)
				width = i - width;
		}
		width *= _CacheSegmentSize;
	}

	if(height == DEFAULT) {
		int lineMaxWidth = width;
		height = 1;
		for(int i = 0, lineSize = 0; text[i] != 0; i++) {
			if(text[i] == _T('\n') || lineSize > lineMaxWidth) {
				height++;
				lineSize = 0;
			} else lineSize += _CacheSegmentSize;
		}
		height *= _CacheSegmentSize;
	}

	height = toPowerOf2(height);
	width = toPowerOf2(width);
}
