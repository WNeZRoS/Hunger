#ifndef BUTTONVIEW_H
#define BUTTONVIEW_H

#include "TextView.h"

class Interface;

class ButtonView : public TextView
{
public:
	typedef void (Interface::*ClickEventMethod)(ButtonView *button);
	struct ClickEvent
	{
		Interface *instance;
		ClickEventMethod method;
	};

	enum State
	{
		NORMAL,
		FOCUSED,
		PRESSED
	};

	ButtonView(const std::XSTRING& text, const Font *font, float x, float y, float width, float height);
	virtual ~ButtonView();

	virtual void draw(float xOffset, float yOffset, float xMult, float yMult) const;
	virtual void onMouseMove(float x, float y);
	virtual void onMouseClick(int button, bool down, float x, float y);

	void setOnClickEvent(ClickEvent event);
	void setBackground(uchar r, uchar g, uchar b, uchar a = 0xFF);
	void setBackground(Texture::Name texture);
	void setBackgroundFocused(uchar r, uchar g, uchar b, uchar a = 0xFF);
	void setBackgroundFocused(Texture::Name texture);
	void setBackgroundPressed(uchar r, uchar g, uchar b, uchar a = 0xFF);
	void setBackgroundPressed(Texture::Name texture);

private:
	ClickEvent _clickEvent;
	State _state;
	Background _backgroundNormal;
	Background _backgroundFocused;
	Background _backgroundPressed;
};

#endif // BUTTONVIEW_H
