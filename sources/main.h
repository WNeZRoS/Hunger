#ifndef MAIN_H
#define MAIN_H

#include "Api/Api.h"
#include "Player.h"

class Main : public Controller
{
public:
	Main();
	~Main();

	void keyUpEvent(const Control::Event& event, int x, int y);
	void keyDownEvent(const Control::Event& event, int x, int y);
	void keyLeftEvent(const Control::Event& event, int x, int y);
	void keyRightEvent(const Control::Event& event, int x, int y);
	void keyNoEvent(const Control::Event &event, int x, int y);

	bool initialize();
	int work();

private:

	Context *_context;
	Render *_render;
	LevelMap *_map;
	Player *_player;

	int _moveX, _moveY;
};

#endif // MAIN_H
