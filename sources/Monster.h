#ifndef MONSTER_H
#define MONSTER_H

#include "Api/Api.h"
#include "LevelMap.h"

class Intelligence;

class Monster : public Entity
{
public:
	Monster(const Texture::Name texture, Intelligence *intelligence, float speed);
	~Monster();

	void move();
	bool moveTo(const Point& stop);
	void stop();
	Timestamp lastMotion();

	void draw();

	void onChangeWorld(const World *world);
	void onResize(const World *world);
	void onWorldScroll(const World *world);
	void onOverlapBy(const Entity *overlap, const World *world);
	Category getCategory() const;
	int getPhysSize() const;
	void setPosition(const Point &pos);

	bool isOverlap(const Point& center, int radius) const;
	bool isOverlap(const Point& start, const Point& end) const;
private:
	LevelMap *_map;
	TileSprite *_sprite;
	TileSprite::Animation _moveAnimationLeft, _moveAnimationRight, _moveAnimationUp, _moveAnimationDown;
	float _speed, _speedMultip;
	Intelligence *_intelligence;

	Point_i _moveDirection;
	Point _stop;
	Timestamp _lastMoveTime;
	Timestamp _moveTime;
	Mutex _moveMutex;
	mutable Mutex _positionMutex;

	bool move(float x, float y);
};

#endif // MONSTER_H
