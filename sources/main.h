#ifndef MAIN_H
#define MAIN_H

#include "Api/Api.h"
#include "Player.h"
#include "GameInterface.h"
#include "Intelligence.h"

class Main
{
public:
	Main();
	~Main();

	bool initialize();
	int work();
	void cleanup();

	void onEat(int type);
	void onKilled();

private:

	Context *_context;
	Render *_render;
	World *_world;
	LevelMap *_map;
	Player *_player;
	Intelligence *_intelligence;

	GameInterface *_gameInterface;
};

#endif // MAIN_H
