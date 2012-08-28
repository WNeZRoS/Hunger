#define OPENGL
#include "main.h"
#include "LevelMap.h"

#include <iostream>

using namespace std;

Main::Main() {

}

Main::~Main() {
}

void Main::keyUpEvent(const Control::Event& event, int x, int y) {
	_player->move(0, -1);
	cout << "up" << endl;
}

void Main::keyDownEvent(const Control::Event& event, int x, int y) {
	_player->move(0, 1);
	cout << "down" << endl;
}

void Main::keyLeftEvent(const Control::Event& event, int x, int y) {
	_player->move(-1, 0);
	cout << "left" << endl;
}

void Main::keyRightEvent(const Control::Event& event, int x, int y) {
	_player->move(1, 0);
	cout << "right" << endl;
}

bool Main::initialize() {
	_context = new GlWindowsContext(_T("Hunger"), 800, 640, false);
	_render = _context->getRender();

	Control::instance().addEvent(Control::STATE_PRESSED, Control::KEY_UP, this,
								   reinterpret_cast<Control::CallBackMethod>(&Main::keyUpEvent));
	Control::instance().addEvent(Control::STATE_PRESSED, Control::KEY_DOWN, this,
								   reinterpret_cast<Control::CallBackMethod>(&Main::keyDownEvent));
	Control::instance().addEvent(Control::STATE_PRESSED, Control::KEY_LEFT, this,
								   reinterpret_cast<Control::CallBackMethod>(&Main::keyLeftEvent));
	Control::instance().addEvent(Control::STATE_PRESSED, Control::KEY_RIGHT, this,
								   reinterpret_cast<Control::CallBackMethod>(&Main::keyRightEvent));

	Texture *tilesTexture = TextureManager::instance()->load("map");
	if(!tilesTexture) return false;
	LevelMap *map = LevelMap::load("test.map", tilesTexture);
	if(!map) return false;

	World *world = new World();
	world->setMap(map);
	_render->setWorld(world);

	Texture *playerTexture = TextureManager::instance()->load("player");
	if(!playerTexture) return false;
	_player = new Player(map, playerTexture);
	world->addEntity(_player);

	Texture *p1 = TextureManager::instance()->load("player");
	Texture *p2 = TextureManager::instance()->load("player");
	TextureManager::instance()->unload(p1);
	TextureManager::instance()->unload(p2);

	return true;
}

int Main::work() {
	_context->mainLoop();
	cout << "Exiting..." << endl;
	delete _context;
	return 0;
}

Main game;

int main(int argc, char *argv[]) {
	if(game.initialize())
		return game.work();
	return -1;
}
