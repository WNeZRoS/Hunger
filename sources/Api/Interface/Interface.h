#ifndef INTERFACE_H
#define INTERFACE_H

#include "View.h"
#include "../Core/Control.h"
#include <vector>

class Interface : public Controller
{
public:
	enum RenderMode {
		/**
		  Render in quad ((0,0),(1,1)) scaled to full screen
		  */
		Full,
		/**
		  Render in quad ((0,0),(1,1)) aligned to center of screen
		  */
		Center,
		/**
		  Render on screen (used screen coordinates)
		  */
		Screen
	};

	Interface(RenderMode renderMode);
	virtual ~Interface();

	virtual void draw() const;
	virtual void onScreenResized(int width, int height);

	virtual int addView(const View *v);
	virtual View * getView(int id) const;
	virtual void removeView(int id);

	void onMouseMove(const Control::Event& event, int x, int y);
	void onMouseClick(const Control::Event& event, int x, int y);
protected:
	std::vector<View*> _views;
	RenderMode _renderMode;
	float _xOffset, _yOffset, _xMult, _yMult;
};

#endif // INTERFACE_H
