#define OPENGL
#include "main.h"
#include "LevelMap.h"
#include "Food.h"
#include "Api/Logger.h"
#include "Api/Random.h"

Main::Main() {
	_moveX = _moveY = 0;
	Random::start(getCurrentTime());
	//Log::logger.openLog("log.txt");
}

Main::~Main() {
	delete _map;
	delete _player;
	delete _context;
}

void Main::keyUpEvent(const Control::Event& event, int x, int y) {
	if(event.state == Control::STATE_NO_PRESSED) return;
	if(event.state == Control::STATE_DOWN) { _moveX = 0; _moveY = -1; }
	else if(event.state == Control::STATE_UP) _moveX = _moveY = 0;
	else if(_moveX == 0 && _moveY == 0) _moveY = -1;
	if(_moveX != 0 || _moveY != -1) return;
	Log::logger << Log::debug << "up";
	if(event.state == Control::STATE_PRESSED)
		if(!_player->move(0, -1)) _moveX = _moveY = 0;
}

void Main::keyDownEvent(const Control::Event& event, int x, int y) {
	if(event.state == Control::STATE_NO_PRESSED) return;
	if(event.state == Control::STATE_DOWN) { _moveX = 0; _moveY = 1; }
	else if(event.state == Control::STATE_UP) _moveX = _moveY = 0;
	else if(_moveX == 0 && _moveY == 0) _moveY = 1;
	if(_moveX != 0 || _moveY != 1) return;
	Log::logger << Log::debug << "down";
	if(event.state == Control::STATE_PRESSED)
		if(!_player->move(0, 1)) _moveX = _moveY = 0;
}

void Main::keyLeftEvent(const Control::Event& event, int x, int y) {
	if(event.state == Control::STATE_NO_PRESSED) return;
	if(event.state == Control::STATE_DOWN) { _moveX = -1; _moveY = 0; }
	else if(event.state == Control::STATE_UP) _moveX = _moveY = 0;
	else if(_moveX == 0 && _moveY == 0) _moveX = -1;
	if(_moveX != -1 || _moveY != 0) return;
	Log::logger << Log::debug << "left";
	if(event.state == Control::STATE_PRESSED)
		if(!_player->move(-1, 0)) _moveX = _moveY = 0;
}

void Main::keyRightEvent(const Control::Event& event, int x, int y) {
	if(event.state == Control::STATE_NO_PRESSED) return;
	if(event.state == Control::STATE_DOWN) { _moveX = 1; _moveY = 0; }
	else if(event.state == Control::STATE_UP) _moveX = _moveY = 0;
	else if(_moveX == 0 && _moveY == 0) _moveX = 1;
	if(_moveX != 1 || _moveY != 0) return;
	Log::logger << Log::debug << "right";
	if(event.state == Control::STATE_PRESSED)
		if(!_player->move(1, 0)) _moveX = _moveY = 0;
}

bool Main::initialize() {
	_context = new GlWindowsContext(_T("Hunger"), 800, 640, false);
	_render = _context->getRender();

	Control::instance().addEvent(Control::STATE_ANY, Control::KEY_UP, this,
								   reinterpret_cast<Control::CallBackMethod>(&Main::keyUpEvent));
	Control::instance().addEvent(Control::STATE_ANY, Control::KEY_DOWN, this,
								   reinterpret_cast<Control::CallBackMethod>(&Main::keyDownEvent));
	Control::instance().addEvent(Control::STATE_ANY, Control::KEY_LEFT, this,
								   reinterpret_cast<Control::CallBackMethod>(&Main::keyLeftEvent));
	Control::instance().addEvent(Control::STATE_ANY, Control::KEY_RIGHT, this,
								   reinterpret_cast<Control::CallBackMethod>(&Main::keyRightEvent));

	_map = LevelMap::load("test.map", "map");
	if(!_map) return false;

	World *world = new World();
	world->setMap(_map);
	_render->setWorld(world);

	Point *roads;
	int roadCount = 0;
	_map->getRoads(roads, roadCount);
	int values[4] = { 0, 1, 2, 3 };
	int chances[4] = { 75, 10, 5, 10 };
	for(int i = 0; i < roadCount; i++) {
		Food *food = new Food(_map, TextureAtlas::Loader("food", 2, 2), Random::rand(values, chances, 4), roads[i]);
		world->addEntity(food);
	}

	_player = new Player(_map, "player");
	world->addEntity(_player);

	return true;
}

int Main::work() {
	_context->mainLoop();
	Log::logger << Log::info << "Exiting...";
	return 0;
}

Main game;

int main(int argc, char *argv[]) {
	if(game.initialize())
		return game.work();
	return -1;
}
