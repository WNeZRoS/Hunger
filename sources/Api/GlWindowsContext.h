#ifndef GLWINDOWSCONTEXT_H
#define GLWINDOWSCONTEXT_H

#include <windows.h>

#include "Context.h"
#include "Render.h"

class GlWindowsContext : public Context
{
public:

	enum Exceptions
	{
		TITLE_IS_NULL,
		WIDTH_UNDER_ZERO,
		HEIGHT_UNDER_ZERO,
		REGISTER_CLASS_FAIL,
		CREATE_WINDOW_FAIL,
		GET_DC_FAIL,
		CHOOSE_PIXEL_FORMAT_FAIL,
		CREATE_TEMP_RENDER_CONTEXT_FAIL,
		GET_WGLCREATECONTEXTATTRIBSARB_FAIL,
		CREATE_RENDER_CONTEXT_FAIL,
		FULLSCREEN_MODE_FAIL
	};

	GlWindowsContext(const XCHAR *title, int width, int height);
	GlWindowsContext(const XCHAR *title, int width, int height, bool fullscreen);
	GlWindowsContext(const XCHAR *title, int width, int height, int x, int y);
	GlWindowsContext(const XCHAR *title, int width, int height, int glMajor, int glMinor, bool fullscreen);
	GlWindowsContext(const XCHAR *title, int width, int height, int glMajor, int glMinor, int x, int y);

	~GlWindowsContext();

	int mainLoop();

	void setTitle(const XCHAR *title);
	void setResolution(int width, int height);
	void setFullscreen(bool fullscreen);

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

	struct WindowParams
	{
		int width;
		int height;
		bool fullscreen;
	};

	HINSTANCE _hInstance;
	HWND _hWnd;
	HDC _hDC;
	HGLRC _hRC;
	WindowParams _params;
	bool _running;

	void createWindow(const XCHAR *title, int width, int height, int x = -1, int y = -1);
	void createGraphicContext(int major = 0, int minor = 0);
	void printGraphicInformation();
	void addMeToWindowsList();

	static GlWindowsContext **s_windows;
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

#endif // GLWINDOWSCONTEXT_H
