#ifndef WORLD_H
#define WORLD_H

#include "Map.h"
#include "Entity.h"
#include "Thread.h"
#include <list>

class World
{
public:
	World();
	virtual ~World();

	virtual void draw();

	virtual void setMap(const Map *map);
	virtual Map * getMap() const;

	virtual int addEntity(const Entity *entity);
	virtual void removeEntity(Entity *entity);
	virtual Entity * getEntity(int id) const;
	virtual unsigned int getEntitiesCount() const;

	virtual void setCenter(float x, float y);
	virtual void setScreenSize(int width, int height);

	virtual void restart();
	virtual void updated(const Entity *upd);
protected:
	Map *_map;
	std::list<Entity*> _entities;
	int _width, _height;
	std::list<const Entity*> _updatesBy;

	Mutex *_entitiesMutex;
	Mutex _updateMutex;

	virtual void updated();
};

#endif // WORLD_H
