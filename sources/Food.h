#ifndef FOOD_H
#define FOOD_H

#include "Api/Entity.h"
#include "Api/TextureAtlas.h"
#include "Api/Point.h"
#include "LevelMap.h"

class Food : public Entity
{
public:
	Food(const LevelMap *map, const TextureAtlas::Loader atlas, int tileId, const Point& position);
	~Food();

	void draw() const;

private:
	LevelMap *_map;
	TextureAtlas *_atlas;
	int _tileId;
};

#endif // FOOD_H
