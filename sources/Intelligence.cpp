#include "Intelligence.h"

Intelligence::Intelligence() {
}

Intelligence::~Intelligence() {

}

void Intelligence::clear() {
	_roles.clear();
}

void Intelligence::setTarget(Player *target) {
	_target = target;
}

void Intelligence::whatMeDo(Monster *npc) {
	if(_target->getWorld() != npc->getWorld()) return;
	if(_map != npc->getWorld()->getMap()) _map = static_cast<LevelMap*>(npc->getWorld()->getMap());
	if(!_map) return;

	Log::Debug << "What me do???";

	RoleType role = NONE;
	for(std::vector<NpcRole>::iterator it = _roles.begin(); it != _roles.end(); it++) {
		if((*it).npc == npc) role = (*it).role;
	}

	if(role == NONE) {
		role = static_cast<RoleType>(FRONT_ATTACK + static_cast<RoleType>(_roles.size() % 4));
		NpcRole npcRole = { npc, role };
		_roles.push_back(npcRole);
	}

	Point tPos = _target->getPosition();
	Point mPos = npc->getPosition();
	Point delta = (mPos - tPos).abs(); // TODO change to path length
	Point targetPos(0, 0);

	if(_target->isAngry()) { // Run
		// if delta > 7: move around this point
		// if on player way: find other way
		// find far point
		// find path to point
		// if no path: move in player direction or stop
		// say: move in direction in stop at turn position.
	} else if((delta.x == 0 && delta.y <= _map->getOne()) || (delta.x <= _map->getOne() && delta.y == 0)) { // Attack
		targetPos = tPos - (_map->getOne() / 2.0f);
		Log::Debug << "You must go to player, to " << targetPos;

		if(delta.y == 0) {
			npc->stop();
			return;
		}
	} else { // Move
		Point vector = tPos - _target->getLastPosition();
		if(vector == 0) vector = Point(0, -1);

		unsigned short targetFounded = 0;
		do {
			targetFounded++;

			switch(role) {
			case FRONT_ATTACK:
				targetPos = tPos;
				do {
					targetPos += vector;
					_map->turnToBounds(targetPos);
				} while(_map->isOneTile(tPos, targetPos));

				if(_map->isRoad(targetPos)) targetFounded = -1;
				else role = BACK_ATTACK;
				break;
			case BACK_ATTACK:
				targetPos = tPos;
				do {
					targetPos -= vector;
					_map->turnToBounds(targetPos);
				} while(_map->isOneTile(tPos, targetPos));

				if(_map->isRoad(targetPos)) targetFounded = -1;
				else role = SIDE_ATTACK;
				break;
			case SIDE_ATTACK:
				// TODO side attack
				role = GUARD;
				break;
			case GUARD:
				// TODO guard
				role = FRONT_ATTACK;
				break;
			default: break;
			}
		} while(targetFounded <= 4); // search other coordinates

		if(targetFounded != static_cast<unsigned short>(-1)) {
			targetPos = mPos;
			for(targetFounded = 0; targetFounded < 4; targetFounded++) {
				targetPos.x += _map->getOne() * Random::rand(-1, 1);
				targetPos.y += _map->getOne() * Random::rand(-1, 1);
				if(_map->isRoad(targetPos)) targetFounded = -1;
			}
		}

		if(targetFounded != static_cast<unsigned short>(-1)) {
			npc->stop();
			return;
		}
	}

	// get path to.
	Array<LevelMap::PathSegment> path;
	if(_map->isOneTile(targetPos, tPos)) _map->findPath(mPos, targetPos, path);
	else _map->findPath(mPos, targetPos, path, tPos);

	// if no path: go random tile around mPos.
	if(path.size() == 0) {
		std::vector<Point> targets;
		for(int y = -1; y <= 1; y += 2) {
			Point t = mPos; t.y += y;
			if(_map->isRoad(t)) targets.push_back(t);
		}

		for(int x = -1; x <= 1; x += 2) {
			Point t = mPos; t.x += x;
			if(_map->isRoad(t)) targets.push_back(t);
		}

		targetPos = targets.at(Random::rand(targets.size()));
		LevelMap::PathSegment segment = { targetPos };
		path.copy(&segment, 1);
	}

	// say: move in direction with stop at (turn position)
	npc->moveTo(path[0].target);
}
