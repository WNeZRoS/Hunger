#ifndef GLRENDER_H
#define GLRENDER_H

#include "Render.h"
#include "World.h"

class GlRender : public Render {
public:
	class GlPainter : public Painter {
	public:
		~GlPainter();
		void setColor(char r, char g, char b, char a = 0xFF);
		void setLineWidth(int width);
		void pixel(float x, float y, float z = 99.0f) const;
		void line(float fromX, float fromY, float toX, float toY, float z = 99.0f) const;
		void rect(float fromX, float fromY, float toX, float toY, float z = 99.0f, bool fill = true) const;
		void rectx(float fromX, float fromY, float toX, float toY, float z = 99.0f,
				  float fromTexX = 0, float fromTexY = 0, float toTexX = 1, float toTexY = 1) const;
	};

	GlRender(int width, int height);
	~GlRender();
	
	void stopRender();
	void resumeRender();
	void setResolution(int width, int height);
	void render() const;

	const World * getWorld() const;
	void setWorld(const World * world);

private:
	int _width;
	int _height;
	bool _renderStopped;

	World *_world;
};

#endif // GLRENDER_H
