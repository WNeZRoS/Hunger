#include "Interface.h"
#include "compatibility.h"

Interface::Interface(RenderMode renderMode) {
	_renderMode = renderMode;
	Control::instance().addEvent(Control::STATE_DOWN, Control::MOUSE_LEFT, this,
								 reinterpret_cast<Control::CallBackMethod>(&Interface::onMouseClick));
	Control::instance().addEvent(Control::STATE_UP, Control::MOUSE_LEFT, this,
								 reinterpret_cast<Control::CallBackMethod>(&Interface::onMouseClick));
	Control::instance().addEvent(Control::STATE_DOWN, Control::MOUSE_MIDDLE, this,
								 reinterpret_cast<Control::CallBackMethod>(&Interface::onMouseClick));
	Control::instance().addEvent(Control::STATE_UP, Control::MOUSE_MIDDLE, this,
								 reinterpret_cast<Control::CallBackMethod>(&Interface::onMouseClick));
	Control::instance().addEvent(Control::STATE_DOWN, Control::MOUSE_RIGHT, this,
								 reinterpret_cast<Control::CallBackMethod>(&Interface::onMouseClick));
	Control::instance().addEvent(Control::STATE_UP, Control::MOUSE_RIGHT, this,
								 reinterpret_cast<Control::CallBackMethod>(&Interface::onMouseClick));
	Control::instance().addEvent(Control::STATE_MOUSE_MOVE, Control::NO_KEY, this,
								 reinterpret_cast<Control::CallBackMethod>(&Interface::onMouseMove));
}

Interface::~Interface() {
	for(std::vector<View*>::iterator it = _views.begin(); it != _views.end(); it++)
		delete (*it);
	_views.clear();
}

void Interface::draw() const {
	for(std::vector<View*>::const_iterator it = _views.begin(); it != _views.end(); it++) {
		View *v = *it;
		v->draw(_xOffset, _yOffset, _xMult, _yMult);
	}
}

void Interface::onScreenResized(int width, int height) {
	switch(_renderMode) {
	case Full:
		_xOffset = _yOffset = 0.0f;
		_xMult = width;
		_yMult = height;
		break;
	case Center:
		if(width < height) {
			_xOffset = 0.0f;
			_yOffset = (height - width) / 2.0f;
			_xMult = _yMult = width;
		} else {
			_xOffset = (width - height) / 2.0f;
			_yOffset = 0.0f;
			_xMult = _yMult = height;
		}
		break;
	case Screen:
		_xOffset = _yOffset = 0.0f;
		_xMult = _yMult = 1.0f;
		break;
	}
}

int Interface::addView(const View *v) {
	_views.push_back(const_cast<View*>(v));
	return _views.size() - 1;
}

View * Interface::getView(int id) const {
	return _views.at(id);
}

void Interface::removeView(int id) {
	View * v = _views.at(id);
	if(v) delete v;
	_views.erase(_views.begin() + id);
}

void Interface::onMouseClick(const Control::Event &event, int x, int y) {
	float lx = x / _xMult, ly = y / _yMult;
	float vx, vy, vw, vh;
	for(std::vector<View*>::const_iterator it = _views.begin(); it != _views.end(); it++) {
		View *v = *it;
		v->getPosition(vx, vy);
		vw = v->getWidth();
		vh = v->getHeight();

		if((vx <= lx && lx <= vx + vw && vy <= ly && ly <= vy + vh) || event.state == Control::STATE_UP)
			v->onMouseClick(event.key, event.state == Control::STATE_DOWN, lx, ly);
	}
}

void Interface::onMouseMove(const Control::Event &event, int x, int y) {
	float lx = x / _xMult, ly = y / _yMult;
	for(std::vector<View*>::const_iterator it = _views.begin(); it != _views.end(); it++) {
		View *v = *it;
		v->onMouseMove(lx, ly);
	}
}
