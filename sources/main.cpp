#define OPENGL
#include "main.h"
#include "LevelMap.h"
#include "Food.h"
#include "StickHud.h"

Main::Main() {
	Random::start(getCurrentTime());
	//Log::logger.openLog("log.txt");
}

Main::~Main() {
	//delete _map;
	//delete _player;
	delete _context;
}

bool Main::initialize() {
	_context = new GlWindowsContext(_T("Hunger"), 800, 640, false);
	_render = _context->getRender();

	StickHud *hud = new StickHud();
	Control::instance().addHud(hud);

	_map = LevelMap::load("test.map", "map");
	if(!_map) return false;

	World *world = new World();
	world->setMap(_map);
	_render->setWorld(world);

	_player = new Player("player");
	world->addEntity(_player);

	Array<Point> roads;
	_map->getRoads(roads);
	for(unsigned int i = 0; i < roads.size; i++) {
		world->addEntity(new Food(TextureAtlas::Loader("food", 2, 2), 0, roads[i]));
	}

	int values[3] = { 1, 2, 3 };
	int chances[3] = { 40, 20, 40 };
	Array<Point> foods = _map->getFoodSpawns();
	for(unsigned int i = 0; i < foods.size; i++) {
		world->addEntity(new Food(TextureAtlas::Loader("food", 2, 2), Random::rand(values, chances, 3), foods[i]));
	}

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
