#include "TextView.h"
#include "../Core.h"

TextView::TextView(const std::XSTRING& text, const Font *font, float x, float y, float width, float height)
	: View(x, y, width, height) {
	setBorderSize(0);
	_textTexture = NULL;
	setFont(font);
	setText(text);
	setTextAlign(Font::LEFT);
	setTextColor(0xFF, 0xFF, 0xFF);
}

TextView::~TextView() {
	if(_textTexture) delete _textTexture;
}

void TextView::draw(float xOffset, float yOffset, float xMult, float yMult) const {
	View::draw(xOffset, yOffset, xMult, yMult);

	if(_textTexture) {
		this->calculateTextSizes(xMult, yMult);

		_textTexture->activate();
		Render::getPainter()->setCotor(_textColor[0], _textColor[1], _textColor[2], _textColor[3]);
		Render::getPainter()->rectx(xOffset + (_x + _textX) * xMult, yOffset + (_y + _textY) * yMult,
									xOffset + (_x + _textX + _textWidth) * xMult, yOffset + (_y + _textY + _textHeight) * yMult);
	}
}

void TextView::onMouseMove(float x, float y) {
	View::onMouseMove(x, y);
}

void TextView::onMouseClick(int button, bool down, float x, float y) {
	View::onMouseClick(button, down, x, y);
}

const std::XSTRING& TextView::getText() const {
	return _text;
}

Font::Align TextView::getTextAlign() const {
	return _textAlign;
}

void TextView::getTextColor(uchar& r, uchar& g, uchar& b, uchar& a) const {
	r = _textColor[0];
	g = _textColor[1];
	b = _textColor[2];
	a = _textColor[3];
}

void TextView::setFont(const Font *font) {
	_font = const_cast<Font*>(font);
}

void TextView::setText(const std::XSTRING& text) {
	_text = _T("") + text;
	if(_font) {
		if(_textTexture) {
			delete _textTexture;
			_textTexture = NULL;
		}
		_textTexture = _font->renderToTexture(_text.c_str(), Font::DEFAULT, Font::DEFAULT, _textAlign);
	}
}

void TextView::setTextAlign(Font::Align align) {
	_textAlign = align;
	if(_font) {
		if(_textTexture) {
			delete _textTexture;
			_textTexture = NULL;
		}
		_textTexture = _font->renderToTexture(_text.c_str(), Font::DEFAULT, Font::DEFAULT, _textAlign);
	}
}

void TextView::setTextColor(uchar r, uchar g, uchar b, uchar a) {
	_textColor[0] = r;
	_textColor[1] = g;
	_textColor[2] = b;
	_textColor[3] = a;
}

void TextView::calculateTextSizes(float xMult, float yMult) const {
	if(!_textTexture) return;

	switch(_textAlign) {
	case Font::LEFT:
		_textX = 0;
		_textY = (_height - _textTexture->getHeight() / yMult) / 2.0f;
		_textWidth = _textTexture->getWidth() / xMult;
		_textHeight = _textTexture->getHeight() / yMult;
		break;
	case Font::RIGHT:
		_textX = (_width - _textTexture->getWidth() / xMult);
		_textY = (_height - _textTexture->getHeight() / yMult) / 2.0f;
		_textWidth = _textTexture->getWidth() / xMult;
		_textHeight = _textTexture->getHeight() / yMult;
		break;
	case Font::CENTER:
		_textX = (_width - _textTexture->getWidth() / xMult) / 2.0f;
		_textY = (_height - _textTexture->getHeight() / yMult) / 2.0f;
		_textWidth = _textTexture->getWidth() / xMult;
		_textHeight = _textTexture->getHeight() / yMult;
		break;
	}
}
