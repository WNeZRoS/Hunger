#include "GameInterface.h"
#include "Api/FileManager.h"
#include <sstream>

GameInterface::GameInterface() : Interface(Full) {
	_score = 0;
	_lives = 0;

	_panelView = 0;
	_font = 0;
	_scoreView = 0;
	_livesView = 0;
	_pauseButton = 0;

	_panelView = new View(0, 0, 1, 0.05f);
	_panelView->setBackground(0x0, 0x0, 0x0, 0x66);
	_panelView->setBorderColor(0xFF, 0xFF, 0xFF, 0x55);
	addView(_panelView);
	
	char *fontPath = 0;
	FileManager::instance().getPath("font.ttf", fontPath);
	_font = FreeTypeFont::create(fontPath, 16);
	delete fontPath;

	_scoreView = new TextView(_T("Score"), _font, 0.75f, 0, 0.24f, 0.05f);
	_scoreView->setTextAlign(Font::RIGHT);
	_scoreView->setBackground(0x00, 0x00, 0x00, 0x00);
	addView(_scoreView);

	_livesView = new TextView(_T("Lives"), _font, 0.40f, 0, 0.20f, 0.05f);
	_livesView->setTextAlign(Font::CENTER);
	_livesView->setBackground(0x00, 0x00, 0x00, 0x00);
	addView(_livesView);

	_pauseButton = new ButtonView(_T("Pause"), _font, 0.00f, 0, 0.15f, 0.05f);
	ButtonView::ClickEvent pauseClickEvent = { this, reinterpret_cast<ButtonView::ClickEventMethod>(&GameInterface::onPauseClick) };
	_pauseButton->setOnClickEvent(pauseClickEvent);
	addView(_pauseButton);
}

GameInterface::~GameInterface() {
	if(_font) delete _font;
}

int GameInterface::getScore() const {
	return _score;
}

int GameInterface::getLives() const {
	return _lives;
}

void GameInterface::setScore(int score) {
	_score = score;
	if(!_scoreView) return;
	std::XSTRINGSTREAM text;
	text << "Score: " << score;
	_scoreView->setText(text.str());
}

void GameInterface::setLives(int lives) {
	_lives = lives;
	if(!_livesView) return;
	std::XSTRINGSTREAM text;
	text << "Lives: " << lives;
	_livesView->setText(text.str());
}

void GameInterface::onPauseClick(ButtonView *button) {

}

void GameInterface::draw() const {
	Interface::draw();
	//_font->render(_T("Draw cache"), 10.0f, 50.0f);
}
