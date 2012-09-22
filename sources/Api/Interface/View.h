#ifndef VIEW_H
#define VIEW_H

#include "../Core/Texture.h"

class View
{
public:
	typedef unsigned char uchar;

	View(float x, float y, float width, float height);
	virtual ~View();

	virtual void draw(float xOffset, float yOffset, float xMult, float yMult) const;
	virtual void onMouseMove(float x, float y);
	virtual void onMouseClick(int button, bool down, float x, float y);

	void getBackgroundColor(uchar& r, uchar& g, uchar& b, uchar& a) const;
	void getPosition(float& x, float& y) const;
	float getHeight() const;
	float getWidth() const;

	virtual void setBackground(uchar r, uchar g, uchar b, uchar a = 0xFF);
	virtual void setBackground(Texture::Name texture);
	void setBorderSize(int size);
	void setBorderColor(uchar r, uchar g, uchar b, uchar a = 0xFF);
	void setPosition(float x, float y);
	void setSize(float width, float height);

	bool inView(float x, float y) const;
protected:

	struct Background
	{
		uchar red, green, blue, alpha;
		Texture *image;
	};

	float _x, _y;
	float _width, _height;
	Background _background;

private:
	uchar _borderColor[4];
	int _borderSize;
};

#endif // VIEW_H
