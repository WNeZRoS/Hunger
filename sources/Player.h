#ifndef PLAYER_H
#define PLAYER_H

#include "Api/Api.h"
#include "LevelMap.h"

class Player : public Entity
{
public:
	typedef void (Controller::*KillFunc)();

	struct KillCallback
	{
		Controller *pointer;
		KillFunc method;
	};

	Player(const Texture::Name texture, KillCallback killCallback);
	~Player();

	bool moveInDirection(int x, int y);
	void stop();

	void draw();

	void onChangeWorld(const World *world);
	void onResize(const World *world);
	void onWorldScroll(const World *world);
	void onOverlapBy(const Entity *overlap, const World *world);
	Category getCategory() const;
	int getPhysSize() const;

	const Point& getPosition() const;
	const Point& getLastPosition() const;

	bool isOverlap(const Point& center, int radius) const;
	bool isOverlap(const Point& start, const Point& end) const;

	bool isAngry() const;
private:

	class PlayerController : public Controller
	{
	public:
		PlayerController(Player *instance);
		~PlayerController();

		void keyDownEvent(const Control::Event& event, int x, int y);
		void keyUpEvent(const Control::Event &event, int x, int y);

	private:
		Player *_player;
		int _moveX, _moveY;

		void addControlEventForKey(Control::Keys key);
	};

	LevelMap *_map;
	TileSprite *_sprite;
	TileSprite::Animation _moveAnimationLeft, _moveAnimationRight, _moveAnimationUp, _moveAnimationDown;
	PlayerController *_controller;
	KillCallback _killCallback;

	float _speed;
	Point_i _moveDirection;
	Timestamp _lastMoveTime;
	mutable Mutex _positionMutex;

	void move();
	bool move(float x, float y);
};

#endif // PLAYER_H
