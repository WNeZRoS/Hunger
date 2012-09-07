#ifndef FOOD_H
#define FOOD_H

#include "Api/Entity.h"
#include "Api/TextureAtlas.h"
#include "Api/Point.h"
#include "Api/World.h"
#include "LevelMap.h"

class Food : public Entity
{
public:
	Food(const TextureAtlas::Loader atlas, int tileId, const Point& position);
	~Food();

	void draw() const;

	void onChangeWorld(const World *world);
	void onOverlapBy(const Entity *overlap, const World *world);
	const Category getCategory() const;
	int getPhysSize() const;

	bool isOverlap(const Point& center, int radius) const;
	bool isOverlap(const Point& start, const Point& end) const;
private:
	LevelMap *_map;
	TextureAtlas *_atlas;
	int _tileId;
};

#endif // FOOD_H
