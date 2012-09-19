#include "FreeTypeFont.h"
#include "Render.h"
#include "Logger.h"
#include "compatibility.h"
#include <stdexcept>

int FreeTypeFont::_FreeTypeUsages = 0;
FT_Library FreeTypeFont::_FreeType;

FreeTypeFont * FreeTypeFont::create(const char *font, int size, bool monoSpace) {
	if(_FreeTypeUsages == 0) {
		if(FT_Init_FreeType(&_FreeType)) {
			Log::logger << Log::error << "FT_Init_FreeType failed";
			return NULL;
		}
	}

	_FreeTypeUsages++;

	FT_Face face;
	if(FT_New_Face(_FreeType, font, 0, &face)) {
		Log::logger << Log::error << "Load font face failed";
		return NULL;
	}

	FT_Set_Char_Size(face, size * 64, size * 64, 72, 72);

	Render::getPainter()->setRenderTarget(_CacheSize, _CacheSize);
	Texture *cacheTexture = Render::getPainter()->getTexture();

	if(!cacheTexture) {
		return NULL;
	}

	Log::Debug << "Font::create has monospace is " << (monoSpace ? "true" : "false");
	FreeTypeFont *result = new FreeTypeFont(face, cacheTexture);
	result->setMonoSpace(monoSpace);
	return result;
}

FreeTypeFont::FreeTypeFont(FT_Face &face, Texture *cache) {
	_face = face;
	_monoSpace = false;
	_cacheTexture = cache;
}

FreeTypeFont::~FreeTypeFont() {
	delete _cacheTexture;
	_cache.clear();

	FT_Done_Face(_face);

	_FreeTypeUsages--;
	if(_FreeTypeUsages == 0) FT_Done_FreeType(_FreeType);
}

void FreeTypeFont::setMonoSpace(bool monoSpace) {
	if(_monoSpace != monoSpace) _cache.clear();
	_monoSpace = monoSpace;
}

Texture * FreeTypeFont::renderToTexture(const XCHAR *text, int width, int height, Align align) {
	int ownWidth = width, ownHeight = height;
	getTextSizes(text, ownWidth, ownHeight);

	Render::getPainter()->setRenderTarget(ownWidth, ownHeight);
	render(text, 0, 0, ownWidth, ownHeight, align);
	Log::Debug << "Render '" << text << "' in rect " << ownWidth << "x" << ownHeight;
	return Render::getPainter()->getTexture();
}

void FreeTypeFont::render(const XCHAR *text, float x, float y, float width, float height, Align align) {
	if(!text) return;

	int ownWidth = width, ownHeight = height;
	getTextSizes(text, ownWidth, ownHeight);

	static const int segmentsCount = (_CacheSize / _CacheSegmentSize);
	static const float segment = 1.0f / segmentsCount;

	float rx = 0, ry = _CacheSegmentSize;
	std::vector<float> xOffset;
	std::vector<DrawStruct> draws;

	for(unsigned int i = 0; i < xstrlen(text); i++) {
		DrawStruct ds;
		ds.symbol = text[i];
		unsigned int index = 0;
		if(ds.symbol > ' ') index = cacheGlyph(ds.symbol);
		if(index == NO_GLYPH) continue;

		ds.element = _cache.at(index);

		ds.tx = (index % segmentsCount);
		ds.ty = (index - ds.tx) / segmentsCount;
		ds.tx *= segment;
		ds.ty *= segment;

		ds.visible = ds.symbol > _T(' ');
		ds.width = _monoSpace ? _CacheSegmentSize : (ds.visible ? ds.element.width + 2 : _CacheSegmentSize / 4);
		Log::Debug << "ds.width: " << ds.width << " " << (_monoSpace ? "true" : "false");
		rx += ds.width;
		if(rx > ownWidth || ds.symbol == _T('\n')) {
			switch(align) {
			case Font::CENTER: xOffset.push_back((ownWidth - rx) / 2.0f); break;
			case Font::RIGHT: xOffset.push_back(ownWidth - rx); break;
			default: xOffset.push_back(0); break;
			}
			rx = 0;
			ry += _CacheSegmentSize;
		}
		draws.push_back(ds);
	}

	switch(align) {
	case Font::CENTER: xOffset.push_back((ownWidth - rx) / 2.0f); break;
	case Font::RIGHT: xOffset.push_back(ownWidth - rx); break;
	default: xOffset.push_back(0); break;
	}

	unsigned int line = 0;
	rx = x + xOffset.at(line); ry = y + (ownHeight - ry) / 2.0f;

	for(std::vector<DrawStruct>::iterator it = draws.begin(); it != draws.end(); it++) {
		DrawStruct& ds = (*it);
		_cacheTexture->activate();
		Render::getPainter()->rectx(rx, ry + ds.element.top,
									rx + _CacheSegmentSize,
									ry + _CacheSegmentSize + ds.element.top, 99.0f,
									ds.tx, ds.ty, ds.tx + ds.visible * segment, ds.ty + ds.visible * segment);

		rx += ds.width;
		if(rx - x > ownWidth || ds.symbol == _T('\n')) {
			line++;
			rx = x + xOffset.at(line < xOffset.size() ? line : xOffset.size() - 1);
			ry += _CacheSegmentSize;
		}
	}
	_cacheTexture->deactivate();

	if(x != 0) {
		Render::getPainter()->setColor(0x55, 0x55, 0x55);
		Render::getPainter()->rect(100, 100, 612, 612, 99);
		_cacheTexture->activate();
		Render::getPainter()->setCotor(0xFF, 0xFF, 0xFF);
		Render::getPainter()->rectx(100, 100, 612, 612);
		_cacheTexture->deactivate();
	}
}

inline int FreeTypeFont::toPowerOf2(int a) const {
	int x;
	for(x = 1; x < a; x <<= 1);
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

	FT_BitmapGlyph bitmapGlyph = reinterpret_cast<FT_BitmapGlyph>(glyph);
	FT_Bitmap& bitmap = bitmapGlyph->bitmap;

	static const int bytesForPixel = 4;
	unsigned char *data = new unsigned char[bytesForPixel * _CacheSegmentSize * _CacheSegmentSize];
	for(unsigned int i = 0; i < bytesForPixel * _CacheSegmentSize * _CacheSegmentSize; i++) data[i] = 0;

	int height = bitmap.rows;
	int width = bitmap.width;

	int xof = (_CacheSegmentSize - width) / 2;
	if(xof > 0 && !_monoSpace) xof = 0;

	Log::Debug << "Load symbol '" << symbol << "' " << width << "x" << height << " - " << xof;
	for(int h = 0; h < height && h < _CacheSegmentSize; h++) {
		for (int w = xof < 0 ? -xof : 0; w < width; w++) {
			if(w + xof >= _CacheSegmentSize) break;
			int luminance = bitmap.buffer[(height - h - 1) * width + w];
			data[(h * _CacheSegmentSize + w + xof) * bytesForPixel] = 255;
			data[(h * _CacheSegmentSize + w + xof) * bytesForPixel + 1] = 255;
			data[(h * _CacheSegmentSize + w + xof) * bytesForPixel + 2] = 255;
			data[(h * _CacheSegmentSize + w + xof) * bytesForPixel + 3] = luminance;
		}
	}

	int x = (index % (_CacheSize / _CacheSegmentSize));
	int y = (index - x) / (_CacheSize / _CacheSegmentSize);

	if(y * _CacheSegmentSize > _CacheSize) return NO_GLYPH;
	_cacheTexture->setSegment(data, x * _CacheSegmentSize, (_CacheSize / _CacheSegmentSize - y - 1) * _CacheSegmentSize,
							  _CacheSegmentSize, _CacheSegmentSize, Texture::RGBA);

	delete [] data;
	//Log::Debug << "Bitmap " << _face->glyph->bitmap_top << " " << bitmap.width << " " << bitmap.rows;
	CacheElement ce = { symbol, width < _CacheSegmentSize ? width : _CacheSegmentSize,
					  bitmap.rows - _face->glyph->bitmap_top};
	_cache.push_back(ce);
	return index;
}

void FreeTypeFont::getTextSizes(const XCHAR *text, int& width, int &height) const {
	if(width == DEFAULT) {
		width = 1;
		int lw = 0;
		for(int i = 1; text[i - 1] != 0; i++) {
			if((text[i] == _T('\n') || text[i] == 0)) {
				if(i - lw > width) width = i - lw;
				lw = i;
			}
		}
		//Log::Debug << "get size... w:" << width << " lw:" << lw;
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
