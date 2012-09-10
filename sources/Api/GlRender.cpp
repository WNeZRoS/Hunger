#include "GlRender.h"
#include "GlTextureManager.h"
#include "Control.h"

#include "GL/importgl.h"

GlRender::GlRender(int width, int height) {
	_renderStopped = false;

	glClearColor(0.0f, 0.0f, 0.2f, 1.0f);

	//glEnable(GL_COLOR);
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
	GlTextureManager::instance();

	_world = NULL;
	setResolution(width, height);
}


GlRender::~GlRender() {
	delete _Painter;
	if(_world) delete _world;
	delete GlTextureManager::instance();
}

void GlRender::render() const {
	if(_renderStopped) return;

	glClear(GL_COLOR_BUFFER_BIT);

	glLoadIdentity();
	//glTranslatef(-1, -1, 0);

	getPainter()->setColor(0xFF, 0xFF, 0xFF);
	if(_world) _world->draw();

	Control::instance().drawHud();
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
	if(_world) _world->setScreenSize(_width, _height);
}

const World * GlRender::getWorld() const {
	return _world;
}

void GlRender::setWorld(const World *world) {
	_world = const_cast<World*>(world);
	if(_world) _world->setScreenSize(_width, _height);
}

GlRender::GlPainter::GlPainter() {
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			_color[i][j] = 0xFF;

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
}

GlRender::GlPainter::~GlPainter() {
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void GlRender::GlPainter::setColor(char r, char g, char b, char a) {
	for(int i = 0; i < 4; i++) {
		_color[i][0] = r;
		_color[i][1] = g;
		_color[i][2] = b;
		_color[i][3] = a;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GlRender::GlPainter::setLineWidth(int width) {
	glLineWidth(width);
}

void GlRender::GlPainter::pixel(float x, float y, float z) const {
	glPushMatrix();
	glLoadIdentity();
	float vertexs[3] = { x, y, z };

	glVertexPointer(3, GL_FLOAT, 0, vertexs);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, _color);
	glDrawArrays(GL_POINTS, 0, 1);
	glPopMatrix();
}

void GlRender::GlPainter::line(float fromX, float fromY, float toX, float toY, float z) const {
	glPushMatrix();
	glLoadIdentity();

	float vertexs[2][3] = {
		{ fromX, fromY, z },
		{ toX, toY, z }
	};

	glVertexPointer(3, GL_FLOAT, 0, vertexs);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, _color);
	glDrawArrays(GL_LINES, 0, 2);
	glPopMatrix();
}

void GlRender::GlPainter::rect(float fromX, float fromY, float toX, float toY, float z, bool fill) const {
	glPushMatrix();
	glLoadIdentity();

	float vertexs[4][3] = {
		{ fromX, fromY, z },
		{ toX, fromY, z },
		{ toX, toY, z },
		{ fromX, toY, z }
	};

	glVertexPointer(3, GL_FLOAT, 0, vertexs);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, _color);
	if(fill) {
		unsigned short indices[6] = { 0, 1, 2,   2, 3, 0 };
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
	} else glDrawArrays(GL_LINE_LOOP, 0, 4);

	glPopMatrix();
}

void GlRender::GlPainter::rectx(float fromX, float fromY, float toX, float toY, float z,
							   float fromTexX, float fromTexY, float toTexX, float toTexY) const {
	glPushMatrix();
	glLoadIdentity();

	float texs[4][2] = {
		{ fromTexX, fromTexY },
		{ toTexX, fromTexY },
		{ toTexX, toTexY },
		{ fromTexX, toTexY }
	};

	float vertexs[4][3] = {
		{ fromX, fromY, z },
		{ toX, fromY, z },
		{ toX, toY, z },
		{ fromX, toY, z }
	};

	unsigned short indices[6] = { 0, 1, 2,   2, 3, 0 };

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertexs);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, _color);
	glTexCoordPointer(2, GL_FLOAT, 0, texs);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glPopMatrix();
}
