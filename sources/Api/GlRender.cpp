#include "GlRender.h"
#include "GlTextureManager.h"

#ifdef WIN32
#include <Windows.h>
#endif
#include <GL/gl.h>

GlRender::GlRender(int width, int height) {
	_renderStopped = false;

	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

	glEnable(GL_COLOR);
	glEnable(GL_TEXTURE_2D);

	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	//glClearDepth(1.0f);

	//glShadeModel(GL_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_Painter = new GlPainter;

	// Create texture manager instance
	GlTextureManager::instance("./");

	setResolution(width, height);
	setWorld(NULL);
}


GlRender::~GlRender() {
	delete GlTextureManager::instance();
}

void GlRender::render() const {
	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();
	//glTranslatef(-1, -1, 0);

	glColor3f(1, 1, 1);
	if(_world) _world->draw();
}

void GlRender::stopRender() {
	_renderStopped = true;
}

void GlRender::resumeRender() {
	_renderStopped = false;
}

void GlRender::setResolution(int width, int height) {
	_width = width;
	_height = height;
	
	glViewport(0, 0, _width, _height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, _width, _height, 0.0, -100.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
}

const World * GlRender::getWorld() const {
	return _world;
}

void GlRender::setWorld(const World *world) {
	_world = const_cast<World*>(world);
	if(_world) _world->setScreenSize(_width, _height);
}

GlRender::GlPainter::~GlPainter() { }

void GlRender::GlPainter::setColor(char r, char g, char b, char a) {
	glColor4ub(r, g, b, a);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GlRender::GlPainter::setLineWidth(int width) {
	glLineWidth(width);
}

void GlRender::GlPainter::pixel(float x, float y, float z) const {
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_POINTS);
	glVertex3f(x, y, z);
	glEnd();
	glPopMatrix();
}

void GlRender::GlPainter::line(float fromX, float fromY, float toX, float toY, float z) const {
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_LINES);
	glVertex3f(fromX, fromY, z);
	glVertex3f(toX, toY, z);
	glEnd();
	glPopMatrix();
}

void GlRender::GlPainter::rect(float fromX, float fromY, float toX, float toY, float z, bool fill) const {
	glPushMatrix();
	glLoadIdentity();
	glBegin(fill ? GL_QUADS : GL_LINE_LOOP);
	glVertex3f(fromX, fromY, z);
	glVertex3f(toX, fromY, z);
	glVertex3f(toX, toY, z);
	glVertex3f(fromX, toY, z);
	glEnd();
	glPopMatrix();
}

void GlRender::GlPainter::rectx(float fromX, float fromY, float toX, float toY, float z,
							   float fromTexX, float fromTexY, float toTexX, float toTexY) const {
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_QUADS);
	glTexCoord2f(fromTexX, fromTexY); glVertex3f(fromX, fromY, z);
	glTexCoord2f(toTexX, fromTexY); glVertex3f(toX, fromY, z);
	glTexCoord2f(toTexX, toTexY); glVertex3f(toX, toY, z);
	glTexCoord2f(fromTexX, toTexY); glVertex3f(fromX, toY, z);
	glEnd();
	glPopMatrix();
}
