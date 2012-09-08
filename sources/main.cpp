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

	Point *roads;
	int roadCount = 0;
	_map->getRoads(roads, roadCount);
	int values[4] = { 0, 1, 2, 3 };
	int chances[4] = { 75, 10, 5, 10 };
	for(int i = 0; i < roadCount; i++) {
		world->addEntity(new Food(TextureAtlas::Loader("food", 2, 2),
								  Random::rand(values, chances, 4), roads[i]));
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
