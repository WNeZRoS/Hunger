#ifndef STICKHUD_H
#define STICKHUD_H

#include "Api/Core.h"

class StickHud : public Hud
{
public:
	StickHud();
	~StickHud();

	void draw() const;
	bool isVisible() const;

	bool start(int x, int y, int pointer);
	void update(int x, int y, int pointer);
	void end(int pointer);

private:
	int _startX, _startY;
	int _currentX, _currentY;
	bool _pressed[4];
	int _sensivity;

	Control::Keys intToKey(int i) const;
};

#endif // STICKHUD_H
