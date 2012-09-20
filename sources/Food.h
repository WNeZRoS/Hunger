#ifndef FOOD_H
#define FOOD_H

#include "Api/Api.h"
#include "LevelMap.h"

class Food : public Entity
{
public:
	typedef void (Controller::*EatFunc)(int type);

	struct EatCallback
	{
		Controller *pointer;
		EatFunc method;
	};

	Food(const TextureAtlas::Loader atlas, int tileId, const Point& position, EatCallback eatCallback);
	~Food();

	void draw();

	void onChangeWorld(const World *world);
	void onResize(const World *world);
	void onWorldScroll(const World *world);
	void onOverlapBy(const Entity *overlap, const World *world);
	Category getCategory() const;
	int getPhysSize() const;

	bool isOverlap(const Point& center, int radius) const;
	bool isOverlap(const Point& start, const Point& end) const;
private:
	LevelMap *_map;
	TextureAtlas *_atlas;
	int _tileId;
	Point _screenPosition;
	EatCallback _eatCallback;
};

#endif // FOOD_H
