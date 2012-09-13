#ifndef GLRENDER_H
#define GLRENDER_H

#include "Render.h"
#include "GlTexture.h"

class World;
class Interface;

class GlRender : public Render
{
public:
	class GlPainter : public Painter
	{
	public:
		GlPainter(GlRender *owner);
		~GlPainter();
		Texture * getTexture();
		void setColor(char r, char g, char b, char a);
		void setCotor(char r, char g, char b, char a);
		void setLineWidth(int width);
		void setRenderTarget(Texture *texture);
		void setRenderTarget(int width, int height);
		void pixel(float x, float y, float z) const;
		void line(float fromX, float fromY, float toX, float toY, float z) const;
		void rect(float fromX, float fromY, float toX, float toY, float z, bool fill) const;
		void rectx(float fromX, float fromY, float toX, float toY, float z,
				  float fromTexX, float fromTexY, float toTexX, float toTexY) const;
	private:
		char _color[4][4];
		GlRender *_owner;
		GlTexture *_target;
		unsigned int _frameBuffer;
	};

	GlRender(int width, int height);
	~GlRender();
	
	void stopRender();
	void resumeRender();
	void setResolution(int width, int height);
	void render() const;

	const World * getWorld() const;
	void setWorld(const World *world);

	const Interface * getInterface() const;
	void setInterface(const Interface *interf);

private:
	int _width;
	int _height;
	bool _renderStopped;

	World *_world;
	Interface *_interface;

	void setViewport(int width, int height);
};

#endif // GLRENDER_H
