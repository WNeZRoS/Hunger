#ifndef GLANDROIDCONTEXT_H
#define GLANDROIDCONTEXT_H

#include "Context.h"
#include "Render.h"
#include <jni.h>

enum Events {
	ACTIVATE = 0,
	CLOSE = 1,
	GOT_FOCUS = 2,
	LOST_FOCUS = 3,
	KEY_DOWN = 4,
	KEY_UP = 5,
	MOUSE_MOVE = 6,
	MOUSE_DOWN = 7,
	MOUSE_UP = 8,
	DRAW = 9,
	RESIZE = 10
};

class GlAndroidContext : public Context
{
public:
	GlAndroidContext(const XCHAR *title, int width, int height);
	~GlAndroidContext();

	int mainLoop();

	void setTitle(const XCHAR *title);
	void setResolution(int width, int height);
	void setFullscreen(bool fullscreen);

	friend void javaEvents(int event, int p1, int p2);
protected:
	void keyDownEvent(int key);
	void keyUpEvent(int key);
	void closeEvent();
	void gotFocusEvent();
	void lostFocusEvent();
	void activateEvent(bool activate);
	void mouseMoveEvent(int x, int y);
	void mouseKeyDownEvent(int key);
	void mouseKeyUpEvent(int key);
	void drawEvent() const;
	void resizeEvent(int width, int height);

private:
	void printGraphicInformation();

	static GlAndroidContext *_Context;
	bool _running;
};

#endif // GLANDROIDCONTEXT_H
