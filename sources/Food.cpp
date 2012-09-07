#include "Food.h"
#include "Api/Logger.h"
#include "Api/Render.h"

Food::Food(const TextureAtlas::Loader atlas, int tileId, const Point& position) {
	_atlas = atlas.load();
	_tileId = tileId;
	_position = position;
}

Food::~Food() {
	_atlas->unload();
}

void Food::onChangeWorld(const World *world) {
	if(_map) _position -= _map->getTileSize() / 2.0f;
	_map = reinterpret_cast<LevelMap*>(world->getMap());
	_position += _map->getTileSize() / 2.0f;
}

void Food::onOverlapBy(const Entity *overlap, const World *world) {
	Log::logger << Log::debug << "overlap food by " << overlap->getCategory();
	if(overlap->getCategory() == PLAYER)
		const_cast<World*>(world)->removeEntity(this);
}

const Entity::Category Food::getCategory() const {
	return ITEM;
}

int Food::getPhysSize() const {
	return _map->getTileSize();
}

bool Food::isOverlap(const Point &center, int radius) const {
	return false; // TODO
}

bool Food::isOverlap(const Point &start, const Point &end) const {
	return start <= _position && _position <= end;
}

void Food::draw() const {
	Point pos = _position - (_map->getTileSize() / 2.0f);
	_map->globalCoordinatesToScreen(pos, pos);
	_atlas->drawTile(_tileId, pos.x, pos.y, 1, _map->getTileSize(), _map->getTileSize());
}
