#ifndef MAIN_H
#define MAIN_H

#include "Api/Api.h"
#include "Player.h"

class Main
{
public:
	Main();
	~Main();

	bool initialize();
	int work();
	void cleanup();

private:

	Context *_context;
	Render *_render;
	LevelMap *_map;
	Player *_player;
};

#endif // MAIN_H
