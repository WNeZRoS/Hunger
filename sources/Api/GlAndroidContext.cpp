#ifdef ANDROID_NDK

#include "GlAndroidContext.h"
#include "GlRender.h"
#include "Control.h"
#include "Logger.h"

#include "GL/importgl.h"

GlAndroidContext *GlAndroidContext::_Context = 0;

GlAndroidContext::GlAndroidContext(const XCHAR *title, int width, int height) {
	_render = new GlRender(width, height);
	printGraphicInformation();
	setResolution(width, height);
	setFullscreen(true);
	_Context = this;
}

GlAndroidContext::~GlAndroidContext() {
	delete _render;
}

void GlAndroidContext::printGraphicInformation() {
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

void GlAndroidContext::setResolution(int width, int height) {
	_render->setResolution(width, height);
}

void GlAndroidContext::setFullscreen(bool fullscreen) {
	if(fullscreen); // nothing do
}

void GlAndroidContext::setTitle(const XCHAR *title) {
	if(title); // nothing do
}

int GlAndroidContext::mainLoop() {
	_running = true;
	return 0;
}

void GlAndroidContext::activateEvent(bool activate) { }
void GlAndroidContext::closeEvent() {
	_running = false;
}

void GlAndroidContext::gotFocusEvent() {
	if(_render) _render->resumeRender();
}

void GlAndroidContext::lostFocusEvent() {
	if(_render) _render->stopRender();
}

void GlAndroidContext::keyDownEvent(int key) {
	Log::Debug << "key down " << key;
	switch(key) {
	case 19: // UP
		Control::instance().keyboardEvent(Control::STATE_PRESSED, Control::KEY_UP);
		break;
	case 20: // DOWN
		Control::instance().keyboardEvent(Control::STATE_PRESSED, Control::KEY_DOWN);
		break;
	case 21: // LEFT
		Control::instance().keyboardEvent(Control::STATE_PRESSED, Control::KEY_LEFT);
		break;
	case 22: // RIGHT
		Control::instance().keyboardEvent(Control::STATE_PRESSED, Control::KEY_RIGHT);
		break;
	case 32: // Space
		Control::instance().keyboardEvent(Control::STATE_PRESSED, Control::KEY_ACTION);
		break;
	case 4: // Back
		Control::instance().keyboardEvent(Control::STATE_PRESSED, Control::KEY_EXIT);
		break;
	}
}
void GlAndroidContext::keyUpEvent(int key) {
	switch(key) {
	case 19: // UP
		Control::instance().keyboardEvent(Control::STATE_NO_PRESSED, Control::KEY_UP);
		break;
	case 20: // DOWN
		Control::instance().keyboardEvent(Control::STATE_NO_PRESSED, Control::KEY_DOWN);
		break;
	case 21: // LEFT
		Control::instance().keyboardEvent(Control::STATE_NO_PRESSED, Control::KEY_LEFT);
		break;
	case 22: // RIGHT
		Control::instance().keyboardEvent(Control::STATE_NO_PRESSED, Control::KEY_RIGHT);
		break;
	case 32: // Space
		Control::instance().keyboardEvent(Control::STATE_NO_PRESSED, Control::KEY_ACTION);
		break;
	case 4: // Back
		Control::instance().keyboardEvent(Control::STATE_NO_PRESSED, Control::KEY_EXIT);
		break;
	}
}

void GlAndroidContext::mouseMoveEvent(int x, int y) {
	Control::instance().mouseMoveEvent(x, y);
}
void GlAndroidContext::mouseKeyDownEvent(int key) {
	Control::instance().mouseEvent(Control::STATE_PRESSED, static_cast<Control::Keys>(key));
}
void GlAndroidContext::mouseKeyUpEvent(int key) {
	Control::instance().mouseEvent(Control::STATE_NO_PRESSED, static_cast<Control::Keys>(key));
}

void GlAndroidContext::drawEvent() const {
	if(_render) _render->render();
}

void GlAndroidContext::resizeEvent(int width, int height) {
	if(_render) _render->setResolution(width, height);
}

void javaEvents(int event, int p1, int p2) {
	if(!GlAndroidContext::_Context) return;

	switch(event) {
	case ACTIVATE: GlAndroidContext::_Context->activateEvent(p1 == 1); break;
	case CLOSE: GlAndroidContext::_Context->closeEvent(); break;
	case GOT_FOCUS: GlAndroidContext::_Context->gotFocusEvent(); break;
	case LOST_FOCUS: GlAndroidContext::_Context->lostFocusEvent(); break;
	case KEY_DOWN: GlAndroidContext::_Context->keyDownEvent(p1); break;
	case KEY_UP: GlAndroidContext::_Context->keyUpEvent(p1); break;
	case MOUSE_MOVE: GlAndroidContext::_Context->mouseMoveEvent(p1, p2); break;
	case MOUSE_DOWN: GlAndroidContext::_Context->mouseKeyDownEvent(p1); break;
	case MOUSE_UP: GlAndroidContext::_Context->mouseKeyUpEvent(p1); break;
	case DRAW: GlAndroidContext::_Context->drawEvent(); break;
	case RESIZE: GlAndroidContext::_Context->resizeEvent(p1, p2); break;
	}
}

int main(int argc, char *argv[]);

extern "C" {
	void Java_com_wnezros_hunger_GameContext_event(JNIEnv *env, jobject thiz, jint event, jint p1, jint p2) {
		//if(event != DRAW) LOGI("Event %d: %d x %d\n", event, p1, p2);
		javaEvents(event, p1, p2);
		//if(event != DRAW) LOGD("Event %d: end\n", event);
	}

	void Java_com_wnezros_hunger_GameContext_main(JNIEnv *env) {
		//LOGI("Main\n");
		int result = main(0, NULL);
		//LOGD("Main end with code %d\n", result);
	}
}

#endif
