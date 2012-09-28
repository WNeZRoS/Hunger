#include "Intelligence.h"
#include "Api/compatibility.h"

Intelligence::Intelligence() {
	this->_map = NULL;
	this->_target = NULL;
	this->_rolesMutex = new Mutex;
	Log::Debug << "Intelligence created";
}

Intelligence::~Intelligence() {

}

void Intelligence::clear() {
	_rolesMutex->lock();
	_roles.clear();
	_rolesMutex->unlock();
}

void Intelligence::setTarget(Player *target) {
	_target = target;
	_lastTargetPosition = _target->getPosition();
	_map = static_cast<LevelMap*>(target->getWorld()->getMap());
	_motionSense = _map->getOne() / 10.0f;
}

void Intelligence::addMonster(Monster *npc) {
	Log::Debug << "Add monster intelligence " << npc;
	this->addMonster(npc, true);
}

Intelligence::NpcRole Intelligence::addMonster(Monster *npc, bool lockMutex) {
	NpcRole role = { npc, NONE };

	if(!this->_rolesMutex || !_map) return role;
	if(lockMutex) _rolesMutex->lock();

	for(std::vector<NpcRole>::iterator it = _roles.begin(); it != _roles.end(); it++) {
		if((*it).npc == npc) role.role = (*it).role;
	}

	if(role.role == NONE) {
		role.role = static_cast<RoleType>(FRONT_ATTACK + static_cast<RoleType>(_roles.size() % 4));
		_roles.push_back(role);
	}
	if(lockMutex) _rolesMutex->unlock();

	return role;
}

void Intelligence::removeMonster(Monster *npc) {
	if(!this->_rolesMutex) return;

	_rolesMutex->lock();
	for(std::vector<NpcRole>::iterator it = _roles.begin(); it != _roles.end(); it++) {
		if((*it).npc == npc) {
			_roles.erase(it);
			break;
		}
	}
	_rolesMutex->unlock();
}

void Intelligence::whatMeDo(Monster *npc) {
	if(!_target) return;
	if(_target->getWorld() != npc->getWorld()) return;
	NpcRole role = addMonster(npc, false);
	if(role.role == NONE) return;

	//Log::Debug << "What me do???";

	Point tPos = _target->getPosition();
	Point mPos = npc->getPosition();
	Point targetPos(0, 0);

	if(_target->isAngry()) { // Run
		// if delta > 7: move around this point
		// if on player way: find other way
		// find far point
		// find path to point
		// if no path: move in player direction or stop
		// say: move in direction in stop at turn position.
	} else if( (tPos - mPos).length() <= _map->getOne() ) {
		npc->moveTo(tPos);
		return;
	} else {
		targetPos = tPos;
	}

	// if no path: go random tile around mPos.
	if(!_map->getPathFinder()->findPath(mPos, targetPos, role.path)) {
		int x = Random::rand(-1, 1), y = Random::rand(-1, 1);
		while(x == 0 && y == 0) {
			x = Random::rand(-1, 1);
			y = Random::rand(-1, 1);
		}
		npc->moveInDirection(x, y);
		return;
	} else {
		Array<PathFinder::PathElement> path;
		path.copy(role.path.items(), role.path.size() + 1);
		path[role.path.size()].pos = tPos - (_map->getOne() / 2.0f);

		// say: move in direction with stop at (turn position)
		npc->moveByPath(path);
	}
}

void Intelligence::onExit() {
	delete _rolesMutex;
	_rolesMutex = NULL;
}

void Intelligence::onEvent(int command, void *param) {
	if(command == WHAT_ME_DO) {
		this->whatMeDo(static_cast<Monster*>(param));
	}
}

bool Intelligence::run() {
	if(_target) {
		bool needNewPath = (_lastTargetPosition - _target->getPosition()).abs().length() > _motionSense;
		_rolesMutex->lock();
		for(std::vector<NpcRole>::iterator it = _roles.begin(); it != _roles.end(); it++) {
			if(needNewPath) {
				this->whatMeDo((*it).npc);
			}
		}
		_rolesMutex->unlock();
		if(needNewPath) _lastTargetPosition = _target->getPosition();
	}
	msSleep(2);
	return true;
}
