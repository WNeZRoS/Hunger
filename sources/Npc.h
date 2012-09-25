#ifndef NPC_H
#define NPC_H

#include "Api/Core.h"
#include "Api/Content.h"
#include "LevelMap.h"

class Npc : public Entity
{
public:
	typedef void (Controller::*KillFunc)(Npc *corpse);

	struct KillCallback
	{
		Controller *pointer;
		KillFunc method;
	};

	Npc(const Texture::Name texture, KillCallback killCallback, float speed);
	virtual ~Npc() = 0;

	virtual bool moveInDirection(int x, int y);
	virtual bool moveTo(const Point& to);
	virtual void stop();

	virtual void draw();

	virtual void onChangeWorld(const World *world);
	virtual void onResize(const World *world);
	virtual void onWorldScroll(const World *world);
	virtual void onOverlapBy(const Entity *overlap, const World *world);
	virtual Category getCategory() const;
	virtual int getPhysSize() const;

	virtual const Point& getPosition() const;
	virtual const Point& getLastPosition() const;

	virtual void setPosition(const Point &pos);

	virtual bool isOverlap(const Point& center, int radius) const;
	virtual bool isOverlap(const Point& start, const Point& end) const;

protected:
	static const unsigned int LEFT = 0;
	static const unsigned int RIGHT = 1;
	static const unsigned int TOP = 2;
	static const unsigned int BOTTOM = 3;

	TileSprite *_sprite;
	TileSprite::Animation _moveAnimation[4];

	LevelMap *_map;
	KillCallback _killCallback;
	Point _lastPosition;
	Point _stopPosition;
	Point_i _moveDirection;
	Timestamp _lastMoveTime;
	float _speed, _speedMultip;
	int _physSize;
	float _positionOffset;

	//mutable Mutex _positionMutex;

	virtual bool move();
	virtual bool move(float x, float y);
	virtual void updateSpritePosition();
};

#endif // NPC_H
