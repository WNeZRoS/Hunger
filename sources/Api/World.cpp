#include "World.h"
#include "compatibility.h"

using namespace std;

World::World() {
	_map = NULL;
	_entitiesMutex = new Mutex;
}

World::~World() {
	if(_map) delete _map;
	_entitiesMutex->lock();
	for(list<Entity*>::iterator it = _entities.begin(); it != _entities.end(); it++)
		delete *it;

	_entities.clear();
	_entitiesMutex->unlock();
	delete _entitiesMutex;
}

void World::setMap(const Map *map) {
	_map = const_cast<Map*>(map);

	_entitiesMutex->lock();
	for(list<Entity*>::iterator it = _entities.begin(); it != _entities.end(); it++) {
		(*it)->onChangeWorld(this);
		(*it)->onResize(this);
		(*it)->onWorldScroll(this);
	}
	_entitiesMutex->unlock();
}

Map * World::getMap() const {
	return _map;
}

int World::addEntity(const Entity *entity) {
	if(!entity) return -1;

	_entitiesMutex->lock();
	_entities.push_back(const_cast<Entity*>(entity));
	unsigned int id = _entities.size() - 1;
	_entitiesMutex->unlock();

	const_cast<Entity*>(entity)->onChangeWorld(this);
	const_cast<Entity*>(entity)->onResize(this);
	const_cast<Entity*>(entity)->onWorldScroll(this);

	return id;
}

void World::removeEntity(Entity *entity) {
	bool locked = _entitiesMutex->lock(this);
	for(list<Entity*>::iterator it = _entities.begin(); it != _entities.end(); it++)
		if(*it == entity) {
			delete entity;
			_entities.erase(it);
			break;
		}
	if(locked) _entitiesMutex->unlock();
}

Entity * World::getEntity(int id) const {
	Entity *result = NULL;
	_entitiesMutex->lock();
	for(list<Entity*>::const_iterator it = _entities.begin(); it != _entities.end(); it++)
		if(--id == 0) {
			result = *it;
			break;
		}
	_entitiesMutex->unlock();
	return result;
}

unsigned int World::getEntitiesCount() const {
	_entitiesMutex->lock();
	unsigned int size = _entities.size();
	_entitiesMutex->unlock();
	return size;
}

void World::setScreenSize(int width, int height) {
	if(_width == width && _height == height) return;

	_width = width;
	_height = height;

	if(_map) _map->fillScreen(width, height);

	_entitiesMutex->lock();
	for(list<Entity*>::iterator it = _entities.begin(); it != _entities.end(); it++) {
		(*it)->onResize(this);
		(*it)->onWorldScroll(this);
	}
	_entitiesMutex->unlock();
}

void World::setCenter(float x, float y) {
	if(!_map) return;

	float xOffset = _map->_xOffset, yOffset = _map->_yOffset;
	_map->setCenter(x, y);
	if(xOffset == _map->_xOffset && yOffset == _map->_yOffset) return;

	_entitiesMutex->lock();
	for(list<Entity*>::iterator it = _entities.begin(); it != _entities.end(); it++) {
		(*it)->onWorldScroll(this);
	}
	_entitiesMutex->unlock();
}

void World::updated(const Entity *upd) {
	_updateMutex.lock();
	_updatesBy.push_front(upd);
	_updateMutex.unlock();
}

void World::updated() {
	_updateMutex.lock();
	for(std::list<const Entity*>::iterator it = _updatesBy.begin(); it != _updatesBy.end(); it++) {
		const Entity *upd = *it;

		int size = upd->getPhysSize() / 4.0f;
		Point start = upd->getPosition() - size;
		Point end = upd->getPosition() + size;

		bool lock = _entitiesMutex->lock(this);
		for(list<Entity*>::iterator it = _entities.begin(); it != _entities.end(); ) {
			Entity* e = *it++;
			if(e != upd && e->isOverlap(start, end))
				e->onOverlapBy(upd, this);
		}
		if(lock) _entitiesMutex->unlock();
	}
	_updatesBy.clear();
	_updateMutex.unlock();
}

void World::restart() {
	bool lock = _entitiesMutex->lock(this);
	for(list<Entity*>::iterator it = _entities.begin(); it != _entities.end(); it++) {
		(*it)->onChangeWorld(this);
		(*it)->onResize(this);
		(*it)->onWorldScroll(this);
	}
	if(lock) _entitiesMutex->unlock();
}

void World::draw() {
	this->updated();
	if(_map) _map->draw();

	_entitiesMutex->lock();
	for(int c = Entity::START; c != Entity::END; c++) {
		for(list<Entity*>::iterator it = _entities.begin(); it != _entities.end(); it++) {
			if((*it)->getCategory() == c) (*it)->draw();
		}
	}
	_entitiesMutex->unlock();
}
