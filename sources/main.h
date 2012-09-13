#ifndef MAIN_H
#define MAIN_H

#include "Api/Api.h"
#include "Player.h"
#include "GameInterface.h"

class Main
{
public:
	Main();
	~Main();

	bool initialize();
	int work();
	void cleanup();

	void onEat(int type);

private:

	Context *_context;
	Render *_render;
	LevelMap *_map;
	Player *_player;

	GameInterface *_gameInterface;
};

#endif // MAIN_H
