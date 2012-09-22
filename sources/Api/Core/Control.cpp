#include "Control.h"
#include "../compatibility.h"

//Control Control::_Control;

Controller::~Controller() { }
Hud::~Hud() { }

Control::Control() {
	for(int i = 0; i < MAX_POINTERS; i++) _mouseX[i] = _mouseY[i] = 0;
	_lastIdleTime = getCurrentTime();
	_events.clear();
	for(int i = 0; i < 256; i++) _keys[i] = STATE_NO_PRESSED;
	_multiPointer = false;
}

Control::~Control() {
}

Control& Control::instance() {
	static Control * _Control = new Control();
	return *_Control;
}

void Control::addEvent(const Event &event) {
	_events.push_back(event);
}

void Control::addEvent(KeyState state, Keys key, const CallBackFunction& callback) {
	Event event = { state, key };
	event.instance = NULL;
	event.callback.f = callback;
	addEvent(event);
}

void Control::addEvent(KeyState state, Keys key, const Controller *instance, const CallBackMethod& callback) {
	Event event = { state, key };
	event.instance = const_cast<Controller*>(instance);
	event.callback.m = callback;
	addEvent(event);
}

void Control::removeEvent(const Event& event) {
	for(std::vector<Event>::iterator it = _events.begin(); it != _events.end(); it++) {
		Event e = *it;
		if ((e.state == event.state || event.state == STATE_ANY) &&
				(e.key == event.key || event.key == NO_KEY))
			_events.erase(it);
	}
}

void Control::removeEvent(Keys key, KeyState state) {
	Event event = { state, key };
	removeEvent(event);
}

void Control::removeEvent(Controller *controller) {
	for(std::vector<Event>::iterator it = _events.begin(); it != _events.end(); it++) {
		Event e = *it;
		if(e.instance == controller) _events.erase(it);
	}
}

void Control::addHud(const Hud *hud) {
	_huds.push_back(const_cast<Hud*>(hud));
}

void Control::removeHud(const Hud *hud) {
	for(std::vector<Hud*>::iterator it = _huds.begin(); it != _huds.end(); it++) {
		Hud *e = *it;
		if(e == hud) {
			_huds.erase(it);
			delete e;
		}
	}
}

bool Control::isKeyPressed(Keys key) {
	if(key == NO_KEY) return true;
	return _keys[key] == STATE_PRESSED;
}

int Control::getMouseX(int pointer) {
	if(!_multiPointer || (pointer <= 0 && pointer > MAX_POINTERS)) return _mouseX[0];
	return _mouseX[pointer - 1];
}

int Control::getMouseY(int pointer) {
	if(!_multiPointer || (pointer <= 0 && pointer > MAX_POINTERS)) return _mouseY[0];
	return _mouseY[pointer - 1];
}

void Control::setMouseX(int x, int pointer) {
	if(!_multiPointer) pointer = MOUSE_LEFT;
	if(pointer <= 0) pointer = MOUSE_LEFT;
	if(pointer > MAX_POINTERS) return;
	_mouseX[pointer - 1] = x;
}

void Control::setMouseY(int y, int pointer) {
	if(!_multiPointer) pointer = MOUSE_LEFT;
	if(pointer <= 0) pointer = MOUSE_LEFT;
	if(pointer > MAX_POINTERS) return;
	_mouseY[pointer - 1] = y;
}

void Control::idle() {
	if(getCurrentTime() < _lastIdleTime + 50) return;
	_lastIdleTime = getCurrentTime();

	for(std::vector<Event>::const_iterator it = _events.begin(); it != _events.end(); it++) {
		Event e = *it;
		if(e.key == NO_KEY) continue;
		if(((_keys[e.key] == STATE_NO_PRESSED || _keys[e.key] == STATE_PRESSED)
							&& e.state == _keys[e.key]) || e.state == STATE_ANY)
			callCallBack(e.instance, e.callback, _keys[e.key], e.key, getMouseX(e.key), getMouseY(e.key));
	}
}

void Control::drawHud() const {
	for(std::vector<Hud*>::const_iterator it = _huds.begin(); it != _huds.end(); it++) {
		if((*it)->isVisible()) (*it)->draw();
	}
}

void Control::keyboardEvent(KeyState state, Keys key) {
	if(_keys[key] == state) return;

	_keys[key] = state;
	state = state == STATE_NO_PRESSED ? STATE_UP : STATE_DOWN;

	for(std::vector<Event>::iterator it = _events.begin(); it != _events.end(); it++) {
		Event e = *it;
		if(e.key == key && (state == e.state || e.state == STATE_ANY))
			callCallBack(e.instance, e.callback, state, key, getMouseX(e.key), getMouseY(e.key));
	}
}

void Control::mouseEvent(KeyState state, Keys key, int x, int y) {
	if(_keys[key] == state) return;
	setMouseX(x, key);
	setMouseY(y, key);

	_keys[key] = state;
	state = state == STATE_NO_PRESSED ? STATE_UP : STATE_DOWN;

	if(key >= MOUSE_LEFT && key <= MOUSE_BUTTON_5) {
		switch(state) {
		case STATE_UP:
			for(std::vector<Hud*>::const_iterator it = _huds.begin(); it != _huds.end(); it++)
				reinterpret_cast<Hud*>(*it)->end(key);
			break;
		case STATE_DOWN:
			for(std::vector<Hud*>::const_iterator it = _huds.begin(); it != _huds.end(); it++)
				reinterpret_cast<Hud*>(*it)->start(getMouseX(key), getMouseY(key), key);
			break;
		default: break;
		}
	}

	for(std::vector<Event>::iterator it = _events.begin(); it != _events.end(); it++) {
		Event e = *it;
		if(e.key == key && (state == e.state || e.state == STATE_ANY))
			callCallBack(e.instance, e.callback, state, key, getMouseX(e.key), getMouseY(e.key));
	}
}

void Control::mouseMoveEvent(int x, int y, int pointer) {
	if(!_multiPointer) pointer = MOUSE_LEFT;
	setMouseX(x, pointer);
	setMouseY(y, pointer);

	if(isKeyPressed(static_cast<Control::Keys>(pointer))) {
		for(std::vector<Hud*>::const_iterator it = _huds.begin(); it != _huds.end(); it++)
			reinterpret_cast<Hud*>(*it)->update(getMouseX(pointer), getMouseY(pointer), pointer);
	}

	for(std::vector<Event>::iterator it = _events.begin(); it != _events.end(); it++) {
		Event e = *it;
		if(e.state == STATE_MOUSE_MOVE && isKeyPressed(e.key))
			callCallBack(e.instance, e.callback, STATE_MOUSE_MOVE, e.key, x, y);
	}
}

void Control::setMultiPointerMode(bool multi) {
	_multiPointer = multi;
}

void Control::callCallBack(const Controller *instance, CallBack callback, KeyState state, Keys key, int x, int y) const {
	Event event = { state, key, const_cast<Controller*>(instance), callback };

	if(event.callback.f) {
		if(!event.instance) event.callback.f(event, x, y);
		else (event.instance->*(event.callback.m))(event, x, y);
	}
}
