#include "Food.h"

Food::Food(const TextureAtlas::Loader atlas, int tileId, const Point& position, EatCallback eatCalback) {
	_atlas = atlas.load();
	_tileId = tileId;
	_position = position;
	_map = NULL;
	_screenPosition = 0;
	_eatCallback = eatCalback;

	//Log::logger << Log::debug << "new food at " << _position << " not " << position;
}

Food::~Food() {
	_atlas->unload();
}

void Food::onChangeWorld(const World *world) {
	if(_map) _position -= _map->getOne() / 2;
	_map = reinterpret_cast<LevelMap*>(world->getMap());
	if(_map) _position += _map->getOne() / 2.0f;
}

void Food::onResize(const World *world) {

}

void Food::onWorldScroll(const World *world) {
	if(!_map) return;
	_screenPosition = _position - (_map->getOne() / 2.0f);
	_map->globalCoordinatesToScreen(_screenPosition, _screenPosition);
}

void Food::onOverlapBy(const Entity *overlap, const World *world) {
	Log::logger << Log::debug << "overlap food by " << overlap->getCategory() << " world " << world;
	if(overlap->getCategory() == PLAYER) {
		if(_eatCallback.pointer) (_eatCallback.pointer->*(_eatCallback.method))(_tileId);
		const_cast<World*>(world)->removeEntity(this);
	}
}

const Entity::Category Food::getCategory() const {
	return ITEM;
}

int Food::getPhysSize() const {
	return _map ? _map->getOne() : 1;
}

bool Food::isOverlap(const Point &center, int radius) const {
	return false; // TODO
}

bool Food::isOverlap(const Point &start, const Point &end) const {
	return start <= _position && _position <= end;
}

void Food::draw() {
	if(!_map || !_atlas) return;
	_atlas->drawTile(_tileId, _screenPosition.x, _screenPosition.y, 1, _map->getTileSize(), _map->getTileSize());
}
