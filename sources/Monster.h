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

	void draw();

	void onChangeWorld(const World *world);
	void onOverlapBy(const Entity *overlap, const World *world);

	bool moveByPath(const Array<PathFinder::PathElement>& path);
private:
	Intelligence *_intelligence;
	Array<PathFinder::PathElement> _path;
	unsigned int _step;

	unsigned int getCurrentPathStep() const;
	void moveToStep(unsigned int step);

	MoveState move();
	MoveState move(float x, float y);
};

#endif // MONSTER_H
