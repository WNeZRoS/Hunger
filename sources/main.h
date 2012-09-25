#ifndef MAIN_H
#define MAIN_H

#include "Api/Api.h"
#include "Player.h"
#include "GameInterface.h"
#include "Intelligence.h"
#include "GameWorld.h"

class Main
{
public:
	Main();
	~Main();

	bool initialize();
	int work();
	void cleanup();

	void onEat(int type);
	void onKilled(Npc *corpse);

private:

	Context *_context;
	Render *_render;
	GameWorld *_world;
	LevelMap *_map;
	Player *_player;
	Intelligence *_intelligence;

	GameInterface *_gameInterface;
};

#endif // MAIN_H
