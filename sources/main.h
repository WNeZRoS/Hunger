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

	bool initialize();
	int work();

private:

	Context *_context;
	Render *_render;
	Player *_player;
};

#endif // MAIN_H
