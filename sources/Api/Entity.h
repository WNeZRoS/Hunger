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

	Entity() { _world = NULL; _lastPosition = 0; _position = 0; }
	virtual ~Entity() = 0;

	virtual void draw() = 0;
	virtual void onChangeWorld(const World *world) { _world = const_cast<World*>(world); }
	virtual void onResize(const World *world) { }
	virtual void onWorldScroll(const World *world) { }
	virtual void onOverlapBy(const Entity *overlap, const World *world) { }

	virtual bool isOverlap(const Point& center, int radius) const { return false; }
	virtual bool isOverlap(const Point& start, const Point& end) const { return false; }

	virtual World * getWorld() const { return _world; }
	virtual const Category getCategory() const = 0;
	virtual void getPosition(float& x, float& y) const { x = _position.x; y = _position.y; }
	virtual const Point& getPosition() const { return _position; }
	virtual int getPhysSize() const { return 0; }
	virtual void setPosition(float x, float y) { _position.x = x; _position.y = y; }
	virtual void setPosition(const Point& pos) { _position = pos; }
	virtual const Point& getLastPosition() const { return _lastPosition; }

protected:
	Point _position, _lastPosition;
	World *_world;
};

#endif // ENTITY_H
