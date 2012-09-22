#include "View.h"
#include "../Core.h"
#include "../compatibility.h"

View::View(float x, float y, float width, float height) {
	setPosition(x, y);
	setSize(width, height);
	setBackground(0x0, 0x0, 0x0);
	setBorderColor(0xFF, 0xFF, 0xFF);
	setBorderSize(1);
	_background.image = NULL;
}

View::~View() {
	if(_background.image) UNLOAD_TEXTURE(_background.image);
}

void View::draw(float xOffset, float yOffset, float xMult, float yMult) const {
	Render::getPainter()->setColor(_background.red, _background.green, _background.blue, _background.alpha);
	if(_background.image) _background.image->activate();
	Render::getPainter()->rectx(xOffset + xMult*_x, yOffset + yMult*_y,
								xOffset + xMult*(_x+_width), yOffset + yMult*(_y+_height));
	if(_background.image) _background.image->deactivate();
	if(_borderSize > 0) {
		Render::getPainter()->setLineWidth(_borderSize);
		Render::getPainter()->setCotor(_borderColor[0], _borderColor[1], _borderColor[2], _borderColor[3]);
		Render::getPainter()->rect(xOffset + xMult*_x, yOffset + yMult*_y,
								   xOffset + xMult*(_x+_width), yOffset + yMult*(_y+_height),
								   99.0f, false);
	}
}

void View::onMouseMove(float x, float y) {

}

void View::onMouseClick(int button, bool down, float x, float y) {

}

void View::getBackgroundColor(uchar& r, uchar& g, uchar& b, uchar& a) const {
	r = _background.red;
	g = _background.green;
	b = _background.blue;
	a = _background.alpha;
}

void View::getPosition(float& x, float& y) const {
	x = _x;
	y = _y;
}

float View::getHeight() const {
	return _height;
}

float View::getWidth() const {
	return _width;
}

void View::setBackground(uchar r, uchar g, uchar b, uchar a) {
	_background.red = r;
	_background.green = g;
	_background.blue = b;
	_background.alpha = a;
}

void View::setBackground(Texture::Name texture) {
	if(_background.image) UNLOAD_TEXTURE(_background.image);
	_background.image = LOAD_TEXTURE(texture);
}

void View::setBorderColor(uchar r, uchar g, uchar b, uchar a) {
	_borderColor[0] = r;
	_borderColor[1] = g;
	_borderColor[2] = b;
	_borderColor[3] = a;
}

void View::setBorderSize(int size) {
	_borderSize = size;
}

void View::setPosition(float x, float y) {
	_x = x;
	_y = y;
}

void View::setSize(float width, float height) {
	_width = width;
	_height = height;
}

bool View::inView(float x, float y) const {
	return _x <= x && x <= _x + _width && _y <= y && y <= _y + _height;
}
