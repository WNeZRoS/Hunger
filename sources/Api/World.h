#ifndef WORLD_H
#define WORLD_H

#include "Map.h"
#include "Entity.h"
#include <list>

class World
{
public:
	World();
	~World();

	void draw();

	void setMap(const Map *map);
	Map * getMap() const;

	int addEntity(const Entity *entity);
	void removeEntity(Entity *entity);
	Entity * getEntity(int id) const;
	unsigned int getEntitiesCount() const;

	void setCenter(float x, float y);
	void setScreenSize(int width, int height);

	void restart();
	void updated(const Entity *upd);
protected:
	Map *_map;
	std::list<Entity*> _entities;
	int _width, _height;
};

#endif // WORLD_H
