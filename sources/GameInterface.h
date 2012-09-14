#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include "Api/Interface.h"
#include "Api/TextView.h"
#include "Api/ButtonView.h"
#include "Api/FreeTypeFont.h"

class GameInterface : public Interface, public ButtonView::Events
{
public:
	GameInterface();
	~GameInterface();

	int getScore() const;
	int getLives() const;

	void setScore(int score);
	void setLives(int lives);

	void onPauseClick(ButtonView *button);

private:
	int _score;
	int _lives;

	Font *_font;
	View *_panelView;
	TextView *_scoreView;
	ButtonView *_pauseButton;
	//ImageView *_livesView;
};

#endif // GAMEINTERFACE_H
