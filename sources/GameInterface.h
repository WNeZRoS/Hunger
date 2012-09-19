#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include "Api/Interface.h"
#include "Api/TextView.h"
#include "Api/ButtonView.h"
#include "Api/FreeTypeFont.h"

class GameInterface : public Interface
{
public:
	GameInterface();
	~GameInterface();

	int getScore() const;
	int getLives() const;

	void setScore(int score);
	void setLives(int lives);

	void onPauseClick(ButtonView *button);
	void draw() const;

private:
	int _score;
	int _lives;

	Font *_font;
	View *_panelView;
	TextView *_scoreView;
	TextView *_livesView;
	ButtonView *_pauseButton;
	//ImageView *_livesView;
};

#endif // GAMEINTERFACE_H
