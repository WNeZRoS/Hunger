#ifndef INTELLIGENCE_H
#define INTELLIGENCE_H

#include "Player.h"
#include "Monster.h"
#include "LevelMap.h"
#include <vector>

// Artificial Intelligence
class Intelligence : public Thread
{
public:
	enum RoleType
	{
		NONE, FRONT_ATTACK, BACK_ATTACK, SIDE_ATTACK, GUARD
	};

	enum Events {
		WHAT_ME_DO = 1,
	};

	Intelligence();
	~Intelligence();

	void clear();
	void setTarget(Player *target);
	void addMonster(Monster *npc);
	void removeMonster(Monster *npc);
	void whatMeDo(Monster *npc);

protected:
	struct NpcRole {
		Monster *npc;
		RoleType role;
		Array<PathFinder::PathElement> path;
	};

	LevelMap *_map;
	Player *_target;
	std::vector<NpcRole> _roles;
	Mutex *_rolesMutex;
	Point _lastTargetPosition;
	float _motionSense;

	NpcRole addMonster(Monster *npc, bool lockMutex);

	bool run();
	void onExit();
	void onEvent(int command, void *param);
};

#endif // INTELLIGENCE_H
