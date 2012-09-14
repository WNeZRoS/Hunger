#include "GameInterface.h"
#include "Api/FileManager.h"
#include <sstream>

GameInterface::GameInterface() : Interface(Full) {
	_panelView = new View(0, 0, 1, 0.05f);
	_panelView->setBackground(0x0, 0x0, 0x0, 0x66);
	_panelView->setBorderColor(0xFF, 0xFF, 0xFF, 0x55);
	addView(_panelView);

	char fontPath[256];
	FileManager::instance().getPath("font.ttf", fontPath);
	_font = new FreeTypeFont(fontPath, 16);

	_scoreView = new TextView(_T("Score: 0"), _font, 0.75f, 0, 0.24f, 0.05f);
	_scoreView->setTextAlign(Font::RIGHT);
	_scoreView->setBackground(0x00, 0x00, 0x00, 0x00);
	addView(_scoreView);

	_pauseButton = new ButtonView(_T("Pause"), _font, 0.00f, 0, 0.15f, 0.05f);
	ButtonView::ClickEvent pauseClickEvent = { this, reinterpret_cast<ButtonView::ClickEventMethod>(&GameInterface::onPauseClick) };
	_pauseButton->setOnClickEvent(pauseClickEvent);
	addView(_pauseButton);

	_score = 0;
	_lives = 0;
}

GameInterface::~GameInterface() {
	delete _font;
}

int GameInterface::getScore() const {
	return _score;
}

int GameInterface::getLives() const {
	return _lives;
}

void GameInterface::setScore(int score) {
	_score = score;
	std::XSTRINGSTREAM text;
	text << "Score: " << score;
	_scoreView->setText(text.str());
}

void GameInterface::setLives(int lives) {
	_lives = lives;
}

void GameInterface::onPauseClick(ButtonView *button) {

}
