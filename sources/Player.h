#ifndef PLAYER_H
#define PLAYER_H

#include "Npc.h"

class Player : public Npc
{
public:

	Player(const Texture::Name texture, KillCallback killCallback, float speed);
	~Player();

	void onChangeWorld(const World *world);
	void onResize(const World *world);
	void onOverlapBy(const Entity *overlap, const World *world);
	Category getCategory() const;

	void draw();

	void setAngry(Timestamp angry);
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

	PlayerController *_controller;
	Timestamp _angryTime;

	MoveState move(float x, float y);
};

#endif // PLAYER_H
