#define OPENGL
#include "main.h"
#include "LevelMap.h"
#include "Food.h"
#include "StickHud.h"
#include "Monster.h"
#include <stdexcept>

Main::Main() {
	Random::start(getCurrentTime());
	//Log::logger.openLog("log.txt");

	_gameInterface = NULL;
	_context = NULL;
	_intelligence = NULL;
}

Main::~Main() {
	//delete _map;
	//delete _player;
	if(_intelligence) {
		_intelligence->stop();
		_intelligence->wait();
		if(_world) delete _world;
		delete _intelligence;
	} else if(_world) delete _world;
	if(_gameInterface) delete _gameInterface;
	if(_context) delete _context;

	delete Log::Logger::instance_pointer();
}

bool Main::initialize() {
#ifdef ANDROID_NDK
	FileManager::instance().changeDirectory("/sdcard/Android/data/com.wnezros.hunger/files/");
#endif

	_context = new AppContext(_T("Hunger"), 800, 640);
	_render = _context->getRender();

	StickHud *hud = new StickHud();
	Control::instance().addHud(hud);

	_map = LevelMap::load("test.map", "map");
	if(!_map) return false;

	_world = new GameWorld();
	_world->setMap(_map);
	_render->setWorld(_world);

	Player::KillCallback killCallback = { reinterpret_cast<Controller*>(this), reinterpret_cast<Player::KillFunc>(&Main::onKilled) };
	_player = new Player("player", killCallback);
	_world->addEntity(_player);

	_intelligence = new Intelligence();
	_intelligence->setTarget(_player);

	Monster *monster = new Monster("monster", _intelligence, 15);
	_world->addEntity(monster);

	Monster *monster2 = new Monster("monster2", _intelligence, 16);
	_world->addEntity(monster2);

	Array<Point> roads;
	_map->getRoads(roads);
	Food::EatCallback eatCallback = { reinterpret_cast<Controller*>(this), reinterpret_cast<Food::EatFunc>(&Main::onEat) };
	for(unsigned int i = 0; i < roads.size(); i++) {
		_world->addEntity(new Food(TextureAtlas::Loader("food", 2, 2), 0, roads[i], eatCallback));
	}

	int values[3] = { 1, 2, 3 };
	int chances[3] = { 40, 20, 40 };
	Array<Point> foods = _map->getFoodSpawns();
	for(unsigned int i = 0; i < foods.size(); i++) {
		_world->addEntity(new Food(TextureAtlas::Loader("food", 2, 2), Random::rand(values, chances, 3),
								  foods[i], eatCallback));
	}

	_gameInterface = new GameInterface();
	_gameInterface->setLives(3);
	_gameInterface->setScore(0);
	_render->setInterface(_gameInterface);
	return true;
}

int Main::work() {
	_context->mainLoop();
	Log::logger << Log::info << "Exiting...";
	cleanup();
	return 0;
}

void Main::cleanup() {

}

void Main::onEat(int type) {
	Log::Debug << "On eat " << type;
	_gameInterface->setScore(_gameInterface->getScore() + (type + 1) * 10);
}

void Main::onKilled() {
	Log::Debug << "Player killed";
	_gameInterface->setLives(_gameInterface->getLives() - 1);
	_world->restart();
}

Main game;

int main(int argc, char *argv[]) {
	try {
		if(game.initialize())
			return game.work();
	} catch(std::runtime_error e) {
		std::cerr << "Exception: " << e.what();
	}

	return -1;
}
