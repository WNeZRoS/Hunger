#ifndef CONTEXT_H
#define CONTEXT_H

#include "compatibility.h"

class Render;

class Context {

public:
	virtual ~Context() = 0;
	virtual int mainLoop() = 0;

	virtual Render * getRender() const { return _render; }

	virtual void setTitle(const CHAR *title) = 0;
	virtual void setResolution(int width, int height) = 0;
	virtual void setFullscreen(bool fullscreen) = 0;

protected:
	virtual void activateEvent(bool activate) { }
	virtual void closeEvent() { }
	virtual void drawEvent() const { }
	virtual void gotFocusEvent() { }
	virtual void keyDownEvent(int key) { }
	virtual void keyUpEvent(int key) { }
	virtual void lostFocusEvent() { }
	virtual void mouseMoveEvent(int x, int y) { }
	virtual void mouseKeyDownEvent(int key) { }
	virtual void mouseKeyUpEvent(int key) { }

protected:
	Render * _render;

};

#endif // CONTEXT_H
