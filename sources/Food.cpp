#include "Food.h"

Food::Food(const LevelMap *map, const TextureAtlas::Loader atlas, int tileId, const Point& position) {
	_map = const_cast<LevelMap*>(map);
	_atlas = atlas.load();
	_tileId = tileId;
	_x = position.x;
	_y = position.y;
	_z = 1;
}

Food::~Food() {
	_atlas->unload();
}

void Food::draw() const {
	Point pos(_x, _y);
	_map->globalCoordinatesToScreen(pos, pos);
	_atlas->drawTile(_tileId, pos.x, pos.y, _z, _map->getTileSize(), _map->getTileSize());
}
