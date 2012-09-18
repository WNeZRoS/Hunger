#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include "Api/Api.h"
#include "LevelMap.h"

class GameWorld : public World
{
public:
	GameWorld();
	~GameWorld();

	void restart();
	void updated(const Entity *upd);

private:
	bool _firstUpdate;
};

#endif // GAMEWORLD_H
