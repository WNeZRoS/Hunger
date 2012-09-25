#ifndef MONSTER_H
#define MONSTER_H

#include "Npc.h"

class Intelligence;

class Monster : public Npc
{
	friend class Intelligence;

public:
	Monster(const Texture::Name texture, KillCallback killCallback, float speed, Intelligence *intelligence);
	~Monster();

	void onChangeWorld(const World *world);
	void onOverlapBy(const Entity *overlap, const World *world);

private:
	Intelligence *_intelligence;

	bool move();
	bool move(float x, float y);
};

#endif // MONSTER_H
