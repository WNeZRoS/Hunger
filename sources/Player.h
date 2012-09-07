#ifndef PLAYER_H
#define PLAYER_H

#include "Api/Api.h"
#include "LevelMap.h"

class Player : public Entity
{
public:
	Player(const Texture::Name texture);
	~Player();

	bool move(int x, int y);
	void draw() const;

	void onChangeWorld(const World *world);
	void onOverlapBy(const Entity *overlap, const World *world);
	const Category getCategory() const;
	int getPhysSize() const;

	bool isOverlap(const Point& center, int radius) const;
	bool isOverlap(const Point& start, const Point& end) const;
private:

	World *_world;
	LevelMap *_map;
	TileSprite *_sprite;
	TileSprite::Animation _moveAnimationLeft, _moveAnimationRight, _moveAnimationUp, _moveAnimationDown;
	int _tileSize, _phisTileSize;
	float _speed;
};

#endif // PLAYER_H
