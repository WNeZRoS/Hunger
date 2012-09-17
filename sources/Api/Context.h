#ifndef CONTEXT_H
#define CONTEXT_H

#include "compatibility.h"

class Render;

class Context
{

public:
	Context() { }
	Context(const XCHAR *title, int width, int height) { }
	virtual ~Context() = 0;
	virtual int mainLoop() = 0;

	virtual Render * getRender() const { return _render; }

	virtual void setTitle(const XCHAR *title) = 0;
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
	virtual void mouseMoveEvent(int x, int y, int pointer) { }
	virtual void mouseKeyDownEvent(int key, int x, int y) { }
	virtual void mouseKeyUpEvent(int key, int x, int y) { }
	virtual void resizeEvent(int width, int height) { }
protected:
	Render *_render;

};

#endif // CONTEXT_H
