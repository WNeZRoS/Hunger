#ifndef PLAYER_H
#define PLAYER_H

#include "Api/Api.h"
#include "LevelMap.h"

class Player : public Entity
{
public:
	Player(const LevelMap *map, const Texture *texture);
	~Player();
	bool move(int x, int y);
	void draw() const;

private:

	LevelMap *_map;
	TileSprite *_sprite;
	TileSprite::Animation _moveAnimationLeft, _moveAnimationRight, _moveAnimationUp, _moveAnimationDown;
	int _tileSize, _phisTileSize;
	float _speed;
};

#endif // PLAYER_H
