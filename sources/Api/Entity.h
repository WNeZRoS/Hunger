#ifndef ENTITY_H
#define ENTITY_H

#include "Point.h"

class World;

class Entity
{
public:
	enum Category {
		START = 0,
		DECORATION = 0,
		ITEM = 1,
		PLAYER = 2,
		NPC = 3,
		END = 4
	};

	virtual ~Entity() = 0;

	virtual void draw() = 0;
	virtual void onChangeWorld(const World *world) = 0;
	virtual void onResize(const World *world) = 0;
	virtual void onWorldScroll(const World *world) = 0;
	virtual void onOverlapBy(const Entity *overlap, const World *world) = 0;

	virtual bool isOverlap(const Point& center, int radius) const = 0;
	virtual bool isOverlap(const Point& start, const Point& end) const = 0;

	virtual const Category getCategory() const = 0;
	virtual void getPosition(float& x, float& y) const { x = _position.x; y = _position.y; }
	virtual const Point& getPosition() const { return _position; }
	virtual int getPhysSize() const = 0;
	virtual void setPosition(float x, float y) { _position.x = x; _position.y = y; }
	virtual void setPosition(const Point& pos) { _position = pos; }

protected:
	Point _position;
};

#endif // ENTITY_H
