#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include "../compatibility.h"
#include "View.h"
#include "Font.h"
#include <string>

class TextView : public View
{
public:
	TextView(const std::XSTRING& text, const Font *font, float x, float y, float width, float height);
	virtual ~TextView();

	virtual void draw(float xOffset, float yOffset, float xMult, float yMult) const;
	virtual void onMouseMove(float x, float y);
	virtual void onMouseClick(int button, bool down, float x, float y);

	const std::XSTRING& getText() const;
	Font::Align getTextAlign() const;
	void getTextColor(uchar& r, uchar& g, uchar& b, uchar& a) const;

	void setFont(const Font *font);
	void setText(const std::XSTRING& text);
	void setTextAlign(Font::Align align);
	void setTextColor(uchar r, uchar g, uchar b, uchar a = 0xFF);

private:
	Font *_font;
	std::XSTRING _text;
	Font::Align _textAlign;
	uchar _textColor[4];
	Texture *_textTexture;
	mutable float _textX, _textY, _textWidth, _textHeight;

	void calculateTextSizes(float xMult, float yMult) const;

};

#endif // TEXTVIEW_H
