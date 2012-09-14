#include "ButtonView.h"
#include "Control.h"
#include "TextureManager.h"

ButtonView::ButtonView(const std::XSTRING& text, const Font *font, float x, float y, float width, float height)
	: TextView(text, font, x, y, width, height) {
	_backgroundNormal.image = _backgroundFocused.image = _backgroundPressed.image = NULL;
	_state = NORMAL;

	setBackground(0x0, 0x0, 0x0);
	setBackgroundFocused(0x55, 0x55, 0x0);
	setBackgroundPressed(0x33, 0x33, 0x55);
	setBorderSize(1);
	setBorderColor(0xFF, 0xFF, 0xFF);
	setTextAlign(Font::CENTER);
}

ButtonView::~ButtonView() {
	if(_backgroundNormal.image) UNLOAD_TEXTURE(_backgroundNormal.image);
	if(_backgroundFocused.image) UNLOAD_TEXTURE(_backgroundFocused.image);
	if(_backgroundPressed.image) UNLOAD_TEXTURE(_backgroundPressed.image);
	_background.image = NULL;
}

void ButtonView::draw(float xOffset, float yOffset, float xMult, float yMult) const {
	TextView::draw(xOffset, yOffset, xMult, yMult);
}

void ButtonView::onMouseMove(float x, float y) {
	if(_state != PRESSED) {
		_state = inView(x, y) ? FOCUSED : NORMAL;
		if(_state == FOCUSED) _background = _backgroundFocused;
		else _background = _backgroundNormal;
	}

	TextView::onMouseMove(x, y);
}

void ButtonView::onMouseClick(int button, bool down, float x, float y) {
	if(button == Control::MOUSE_LEFT) {
		if(inView(x, y)) {
			if(_clickEvent.instance && !down && _state == PRESSED)
				(_clickEvent.instance->*(_clickEvent.method))(this);
			if(down) _state = PRESSED;
		}
		if(!down) _state = NORMAL;
		if(_state == PRESSED) _background = _backgroundPressed;
		else onMouseMove(x, y);
	}
	TextView::onMouseClick(button, down, x, y);
}

void ButtonView::setOnClickEvent(ClickEvent event) {
	_clickEvent = event;
}

void ButtonView::setBackground(uchar r, uchar g, uchar b, uchar a) {
	_backgroundNormal.red = r;
	_backgroundNormal.green = g;
	_backgroundNormal.blue = b;
	_backgroundNormal.alpha = a;
	if(_state == NORMAL) _background = _backgroundNormal;
}

void ButtonView::setBackground(Texture::Name texture) {
	if(_backgroundNormal.image) UNLOAD_TEXTURE(_backgroundNormal.image);
	_backgroundNormal.image = LOAD_TEXTURE(texture);
	if(_state == NORMAL) _background = _backgroundNormal;
}

void ButtonView::setBackgroundFocused(uchar r, uchar g, uchar b, uchar a) {
	_backgroundFocused.red = r;
	_backgroundFocused.green = g;
	_backgroundFocused.blue = b;
	_backgroundFocused.alpha = a;
	if(_state == FOCUSED) _background = _backgroundFocused;
}

void ButtonView::setBackgroundFocused(Texture::Name texture) {
	if(_backgroundFocused.image) UNLOAD_TEXTURE(_backgroundFocused.image);
	_backgroundFocused.image = LOAD_TEXTURE(texture);
	if(_state == FOCUSED) _background = _backgroundFocused;
}

void ButtonView::setBackgroundPressed(uchar r, uchar g, uchar b, uchar a) {
	_backgroundPressed.red = r;
	_backgroundPressed.green = g;
	_backgroundPressed.blue = b;
	_backgroundPressed.alpha = a;
	if(_state == PRESSED) _background = _backgroundPressed;
}

void ButtonView::setBackgroundPressed(Texture::Name texture) {
	if(_backgroundPressed.image) UNLOAD_TEXTURE(_backgroundPressed.image);
	_backgroundPressed.image = LOAD_TEXTURE(texture);
	if(_state == PRESSED) _background = _backgroundPressed;
}
