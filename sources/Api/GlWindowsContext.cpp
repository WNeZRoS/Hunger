#include "GlWindowsContext.h"
#include "GlRender.h"
#include "Control.h"
#include "Logger.h"

#include "GL/importgl.h"

#include <stdexcept>
#include <windowsx.h>

#define WINDOW_CLASS_NAME _T("SimpleGL")
#define WINDOW_STYLE WS_CAPTION | WS_SYSMENU | WS_SIZEBOX

using namespace std;

GlWindowsContext **GlWindowsContext::s_windows = NULL;

GlWindowsContext::GlWindowsContext(const XCHAR *title, int width, int height) {
	if(title == NULL) throw std::runtime_error("Title is null");
	if(width <= 0 || height <= 0) throw std::runtime_error("Any of dimensions lower or equal zero");

	createWindow(title, width, height);
	createGraphicContext();
	printGraphicInformation();

	_render = new GlRender(width, height);

	setResolution(width, height);

	addMeToWindowsList();
}

GlWindowsContext::GlWindowsContext(const XCHAR *title, int width, int height, bool fullscreen) {
	if(title == NULL) throw std::runtime_error("Title is null");
	if(width <= 0 || height <= 0) throw std::runtime_error("Any of dimensions lower or equal zero");

	createWindow(title, width, height);
	createGraphicContext();
	printGraphicInformation();
	
	_render = new GlRender(width, height);

	setResolution(width, height);
	setFullscreen(fullscreen);

	addMeToWindowsList();
}

GlWindowsContext::GlWindowsContext(const XCHAR *title, int width, int height, int x, int y) {
	if(title == NULL) throw std::runtime_error("Title is null");
	if(width <= 0 || height <= 0) throw std::runtime_error("Any of dimensions lower or equal zero");

	createWindow(title, width, height, x, y);
	createGraphicContext();
	printGraphicInformation();

	_render = new GlRender(width, height);

	addMeToWindowsList();
}

GlWindowsContext::GlWindowsContext(const XCHAR *title, int width, int height, int glMajor, int glMinor, bool fullscreen) {
	if(title == NULL) throw std::runtime_error("Title is null");
	if(width <= 0 || height <= 0) throw std::runtime_error("Any of dimensions lower or equal zero");

	createWindow(title, width, height);
	createGraphicContext(glMajor, glMinor);
	printGraphicInformation();

	_render = new GlRender(width, height);

	setResolution(width, height);
	setFullscreen(fullscreen);

	addMeToWindowsList();
}

GlWindowsContext::GlWindowsContext(const XCHAR *title, int width, int height, int glMajor, int glMinor, int x, int y) {
	if(title == NULL) throw std::runtime_error("Title is null");
	if(width <= 0 || height <= 0) throw std::runtime_error("Any of dimensions lower or equal zero");

	createWindow(title, width, height);
	createGraphicContext(glMajor, glMinor);
	printGraphicInformation();

	_render = new GlRender(width, height);

	addMeToWindowsList();
}


void GlWindowsContext::addMeToWindowsList() {
	if(s_windows == NULL) {
		s_windows = new GlWindowsContext*[2];
		s_windows[0] = (GlWindowsContext*) 1;
		s_windows[1] = this;
	} else {
		GlWindowsContext **old = s_windows;
		int count = (int) old[0] + 1;

		s_windows = new GlWindowsContext*[count + 1];
		s_windows[0] = (GlWindowsContext*) count;
		s_windows[count] = this;

		for(int i = 1; i < count; i++) {
			s_windows[i] = old[i];
		}

		delete old;
	}
}

GlWindowsContext::~GlWindowsContext() {
	delete _render;

	// restore screen resolution
	if(_params.fullscreen) {
		ChangeDisplaySettings(NULL, CDS_RESET);
		ShowCursor(TRUE);
	}

	// delete render context
	if(_hRC) {
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(_hRC);
	}

	if(_hDC) ReleaseDC(_hWnd, _hDC);
	if(_hWnd) DestroyWindow(_hWnd);
	if(_hInstance) UnregisterClass(WINDOW_CLASS_NAME, _hInstance);

	// Remove window for windows list
	if(s_windows != NULL) {
		GlWindowsContext **old = s_windows;
		int count = (int) old[0] - 1;

		if(count >= 1) {
			s_windows = new GlWindowsContext*[count + 1];
			s_windows[0] = (GlWindowsContext*) count;
			
			for(int i = 1, j = 1; i <= count + 1; i++) {
				if(old[i] != this) {
					s_windows[j] = old[i];
					j++;
				}
			}
		} else s_windows = NULL;

		delete old;
	}
}

void GlWindowsContext::createWindow(const XCHAR *title, int width, int height, int x, int y) {
	_hInstance = (HINSTANCE) GetModuleHandle(NULL);

	// register window class
	WNDCLASSEX wcx;
	memset(&wcx, 0, sizeof(wcx));
	wcx.cbSize= sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcx.lpfnWndProc = (WNDPROC) WindowProc;
	wcx.hInstance = _hInstance;
	wcx.lpszClassName = WINDOW_CLASS_NAME;
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);

	if(!RegisterClassEx(&wcx)) throw std::runtime_error("Fail to register window class");

	// window's styles
	DWORD style = WINDOW_STYLE;
	DWORD exStyle = WS_EX_APPWINDOW;

	if(x == -1) x = (GetSystemMetrics(SM_CXSCREEN) - width)  / 2;
	if(y == -1) y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	
	RECT rect;
	rect.left = x;
	rect.right = x + width;
	rect.top = y;
	rect.bottom = y + height;

	AdjustWindowRectEx (&rect, style, FALSE, exStyle);

	// create window
	_hWnd = CreateWindowEx(exStyle, WINDOW_CLASS_NAME, title, style, rect.left, rect.top,
	rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, _hInstance, NULL);

	_params.width = width;
	_params.height = height;
	_params.fullscreen = false;

	if(!_hWnd) throw std::runtime_error("Can't create window");
}

void GlWindowsContext::createGraphicContext(int major, int minor) {
	_hDC = GetDC(_hWnd);
	if(!_hDC) throw std::runtime_error("Can't get DC");

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;

	int format = ChoosePixelFormat(_hDC, &pfd);
	if (!format || !SetPixelFormat(_hDC, format, &pfd)) throw std::runtime_error("Pixel format setup fail");

	HGLRC hRCTemp = wglCreateContext(_hDC);
	if(!hRCTemp || !wglMakeCurrent(_hDC, hRCTemp)) throw std::runtime_error("Create temp render context fail");
	
	if(major == 0 && minor == 0) _hRC = hRCTemp;
	else {
		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = 
			(PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hRCTemp);

		if(!wglCreateContextAttribsARB) throw std::runtime_error("Get wglCreateContextAttribsARB fail");

		int attribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, major,
			WGL_CONTEXT_MINOR_VERSION_ARB, minor,
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0
		};

		_hRC = wglCreateContextAttribsARB(_hDC, 0, attribs);
		if(!_hRC || !wglMakeCurrent(_hDC, _hRC)) throw std::runtime_error("Create render context fail");
	}
}

void GlWindowsContext::printGraphicInformation() {
	Log::logger << Log::info << "OpenGL render context information:";
	Log::logger << Log::info << "  Renderer: " << glGetString(GL_RENDERER);
	Log::logger << Log::info << "  Vendor: " << glGetString(GL_VENDOR);
	Log::logger << Log::info << "  Version:" << glGetString(GL_VERSION);

#ifdef GL_MAJOR_VERSION
	int major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	Log::logger << Log::info << "  GLSL version:" << glGetString(GL_SHADING_LANGUAGE_VERSION);
	Log::logger << Log::info << "  OpenGL version:" << major << "." << minor;
#endif
}

void GlWindowsContext::setResolution(int width, int height) {
	if(width <= 0 || height <= 0) throw std::runtime_error("Any of dimensions lower or equal zero");

	if(_params.fullscreen) {
		DEVMODE devMode;
		memset(&devMode, 0, sizeof(devMode));
		devMode.dmSize = sizeof(devMode);
		devMode.dmPelsWidth = width;
		devMode.dmPelsHeight = height;
		devMode.dmBitsPerPel = GetDeviceCaps(_hDC, BITSPIXEL);
		devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		LONG result = ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
		if(result != DISP_CHANGE_SUCCESSFUL) {
			_params.fullscreen = false;
			throw std::runtime_error("Enter fullscreen mode fail");
		}
	}

	int x, y;
	DWORD style, exStyle;

	if(_params.fullscreen) {
		ShowCursor(FALSE);

		style = WS_POPUP;
		exStyle = WS_EX_APPWINDOW | WS_EX_TOPMOST;

		x = y = 0;
	} else {
		style = WINDOW_STYLE;
		exStyle = WS_EX_APPWINDOW;

		x = (GetSystemMetrics(SM_CXSCREEN) - width)  / 2;
		y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	}

	RECT rect;
	rect.left = x;
	rect.right = x + width;
	rect.top = y;
	rect.bottom = y + height;

	AdjustWindowRectEx (&rect, style, FALSE, exStyle);

	SetWindowLong(_hWnd, GWL_STYLE,   style);
	SetWindowLong(_hWnd, GWL_EXSTYLE, exStyle);

	SetWindowPos(_hWnd, HWND_TOP, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_FRAMECHANGED);

	ShowWindow(_hWnd, SW_SHOW);
	SetForegroundWindow(_hWnd);
	SetFocus(_hWnd);
	UpdateWindow(_hWnd);

	GetClientRect(_hWnd, &rect);
	_params.width  = rect.right - rect.left;
	_params.height = rect.bottom - rect.top;

	_render->setResolution(_params.width, _params.height);
}

void GlWindowsContext::setFullscreen(bool fullscreen) {
	_params.fullscreen = fullscreen;

	if(_params.fullscreen && !fullscreen) {
		ChangeDisplaySettings(NULL, CDS_RESET);
		ShowCursor(TRUE);
	} else if(!_params.fullscreen && fullscreen) {
		DEVMODE devMode;
		memset(&devMode, 0, sizeof(devMode));
		devMode.dmSize = sizeof(devMode);
		devMode.dmPelsWidth = _params.width;
		devMode.dmPelsHeight = _params.height;
		devMode.dmBitsPerPel = GetDeviceCaps(_hDC, BITSPIXEL);
		devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

		LONG result = ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
		if(result != DISP_CHANGE_SUCCESSFUL) {
			_params.fullscreen = false;
			throw std::runtime_error("Enter fullscreen mode fail");
		}
	}

	if(_params.fullscreen == fullscreen) setResolution(_params.width, _params.height);
}

void GlWindowsContext::setTitle(const XCHAR *title) {
	SetWindowText(_hWnd, title);
}

int GlWindowsContext::mainLoop() {
	MSG msg;
	_running = true;

	while (_running) {
		while (PeekMessage(&msg, _hWnd, 0, 0, PM_REMOVE) && _running) {
			// TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(_running) {
			Control::instance().idle();
			drawEvent();
			SwapBuffers(_hDC);
			Sleep(2);
		}
	}

	return msg.wParam;
}

LRESULT CALLBACK GlWindowsContext::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if(s_windows == NULL) return DefWindowProc(hWnd, msg, wParam, lParam);

	GlWindowsContext * current = NULL;

	for(int i = 1; i <= (int) s_windows[0]; i++) {
		if(s_windows[i]->_hWnd == hWnd) { 
			current = s_windows[i];
			break;
		}
	}

	if(current == NULL) return DefWindowProc(hWnd, msg, wParam, lParam);

	switch (msg) {
	case WM_KEYDOWN:
		current->keyDownEvent(wParam);
		break;
	case WM_KEYUP:
		current->keyUpEvent(wParam);
		break;
	case WM_SETFOCUS:
		current->gotFocusEvent();
		break;
	case WM_KILLFOCUS:
		current->lostFocusEvent();
		break;
	case WM_ACTIVATE:
		current->activateEvent(LOWORD(wParam) == WA_INACTIVE);
		break;
	case WM_CLOSE:
		current->closeEvent();
		PostQuitMessage(0);
		current->_running = false;
		break;
	case WM_MOUSEMOVE:
		current->mouseMoveEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;
	case WM_LBUTTONDOWN:
		current->mouseKeyDownEvent(Control::MOUSE_LEFT);
		break;
	case WM_LBUTTONUP:
		current->mouseKeyUpEvent(Control::MOUSE_LEFT);
		break;
	case WM_RBUTTONDOWN:
		current->mouseKeyDownEvent(Control::MOUSE_RIGHT);
		break;
	case WM_RBUTTONUP:
		current->mouseKeyUpEvent(Control::MOUSE_RIGHT);
		break;
	case WM_MBUTTONDOWN:
		current->mouseKeyDownEvent(Control::MOUSE_MIDDLE);
		break;
	case WM_MBUTTONUP:
		current->mouseKeyUpEvent(Control::MOUSE_MIDDLE);
		break;
	case WM_SIZE:
		current->resizeEvent(LOWORD(lParam), HIWORD(lParam));
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void GlWindowsContext::activateEvent(bool activate) { }
void GlWindowsContext::closeEvent() { }

void GlWindowsContext::gotFocusEvent() {
	if(_render) _render->resumeRender();
}

void GlWindowsContext::lostFocusEvent() {
	if(_render) _render->stopRender();
}

void GlWindowsContext::keyDownEvent(int key) {
	switch(key) {
	case 87: // W
		Control::instance().keyboardEvent(Control::STATE_PRESSED, Control::KEY_UP);
		break;
	case 83: // S
		Control::instance().keyboardEvent(Control::STATE_PRESSED, Control::KEY_DOWN);
		break;
	case 65: // A
		Control::instance().keyboardEvent(Control::STATE_PRESSED, Control::KEY_LEFT);
		break;
	case 68: // D
		Control::instance().keyboardEvent(Control::STATE_PRESSED, Control::KEY_RIGHT);
		break;
	case 32: // Space
		Control::instance().keyboardEvent(Control::STATE_PRESSED, Control::KEY_ACTION);
		break;
	case 27: // Esc
		Control::instance().keyboardEvent(Control::STATE_PRESSED, Control::KEY_EXIT);
		break;
	}
}
void GlWindowsContext::keyUpEvent(int key) {
	switch(key) {
	case 87: // W
		Control::instance().keyboardEvent(Control::STATE_NO_PRESSED, Control::KEY_UP);
		break;
	case 83: // S
		Control::instance().keyboardEvent(Control::STATE_NO_PRESSED, Control::KEY_DOWN);
		break;
	case 65: // A
		Control::instance().keyboardEvent(Control::STATE_NO_PRESSED, Control::KEY_LEFT);
		break;
	case 68: // D
		Control::instance().keyboardEvent(Control::STATE_NO_PRESSED, Control::KEY_RIGHT);
		break;
	case 32: // Space
		Control::instance().keyboardEvent(Control::STATE_NO_PRESSED, Control::KEY_ACTION);
		break;
	case 27: // Esc
		Control::instance().keyboardEvent(Control::STATE_NO_PRESSED, Control::KEY_EXIT);
		break;
	}
}

void GlWindowsContext::mouseMoveEvent(int x, int y) {
	Control::instance().mouseMoveEvent(x, y);
}
void GlWindowsContext::mouseKeyDownEvent(int key) {
	Control::instance().mouseEvent(Control::STATE_PRESSED, static_cast<Control::Keys>(key));
}
void GlWindowsContext::mouseKeyUpEvent(int key) {
	Control::instance().mouseEvent(Control::STATE_NO_PRESSED, static_cast<Control::Keys>(key));
}

void GlWindowsContext::drawEvent() const {
	if(_render) _render->render();
}

void GlWindowsContext::resizeEvent(int width, int height) {
	_params.width = width;
	_params.height = height;
	_render->setResolution(width, height);
}
