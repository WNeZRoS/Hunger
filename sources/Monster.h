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

	bool moveTo(const Point& stop);
	void stop();

	void draw();

	void onChangeWorld(const World *world);
	void onResize(const World *world);
	void onWorldScroll(const World *world);
	void onOverlapBy(const Entity *overlap, const World *world);
	const Category getCategory() const;
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

	void move();
	bool move(float x, float y);
};

#endif // MONSTER_H
