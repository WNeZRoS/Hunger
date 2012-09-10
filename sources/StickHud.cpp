#include "StickHud.h"
#include "Api/Render.h"

#define _USE_MATH_DEFINES
#include <cmath>

StickHud::StickHud() {
	_startX = _startY = -1;
	_currentX = _currentY = 50;
	_sensivity = 15;
	for(int i = 0; i < 4; i++) _pressed[i] = false;
}

StickHud::~StickHud() {

}

void StickHud::draw() const {
	Render::getPainter()->setColor(0xFF, 0x00, 0x00, 0x60);
	Render::getPainter()->rect(_startX - 5, _startY - 5, _startX + 5, _startY + 5, 99.0f, true);
	Render::getPainter()->setColor(0xFF, 0xFF, 0xFF, 0xFF);
	Render::getPainter()->line(_startX, _startY, _currentX, _currentY);
	Render::getPainter()->setColor(0x00, 0xFF, 0x00, 0x80);
	Render::getPainter()->rect(_currentX - 15, _currentY - 15, _currentX + 15, _currentY + 15, 99.0f, true);
	Render::getPainter()->setColor(0xFF, 0xFF, 0xFF, 0xFF);
	Render::getPainter()->rect(_currentX - 15, _currentY - 15, _currentX + 15, _currentY + 15, 99.0f, false);
}

bool StickHud::isVisible() const {
	return _startX >= 0 && _startY >= 0;
}

bool StickHud::start(int x, int y) {
	_startX = _currentX = x;
	_startY = _currentY = y;
	return true;
}

void StickHud::update(int x, int y) {
	_currentX = x;
	_currentY = y;

	float dx = _startX - _currentX;
	float dy = _startY - _currentY;
	float dg = std::sqrt(dx * dx + dy * dy);

	static const float pi8 = M_PI / 8;

	bool pressed[4] = { false, false, false, false };
	if(dg >= _sensivity) {
		dx = std::acos(dx / dg); //		in [     0, PI   ]
		//dy = std::asin(dy / dg); // 	in [ -PI/2, PI/2 ]
		dg = dy / std::abs(dy) * dx; //	in [   -PI, PI   ]
		pressed[0] = dg > pi8 && dg <= pi8*7;
		pressed[1] = dg > pi8*5 || dg <= -pi8*5;
		pressed[2] = dg > -pi8*7 && dg <= -pi8;
		pressed[3] = dg > -pi8*3 && dg <= pi8*3;
	}

	for(int i = 0; i < 4; i++)
		if(pressed[i] != _pressed[i]) {
			_pressed[i] = pressed[i];
			Control::instance().keyboardEvent(_pressed[i] ? Control::STATE_PRESSED
														  : Control::STATE_NO_PRESSED, intToKey(i));
		}
}

void StickHud::end() {
	_startX = _startY = -1;

	for(int i = 0; i < 4; i++)
		if(_pressed[i]) {
			_pressed[i] = false;
			Control::instance().keyboardEvent(Control::STATE_NO_PRESSED, intToKey(i));
		}
}

Control::Keys StickHud::intToKey(int i) const {
	switch(i) {
	case 0: return Control::KEY_UP;
	case 1: return Control::KEY_RIGHT;
	case 2: return Control::KEY_DOWN;
	case 3: return Control::KEY_LEFT;
	}
	return Control::KEY_UP;
}
