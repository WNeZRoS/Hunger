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

	Intelligence();
	~Intelligence();

	virtual void clear();
	virtual void setTarget(Player *target);
	virtual RoleType addMonster(Monster *npc);
	virtual void removeMonster(Monster *npc);
	virtual void whatMeDo(Monster *npc);
	virtual void run();

protected:
	struct NpcRole {
		Monster *npc;
		RoleType role;
	};

	LevelMap *_map;
	Player *_target;
	std::vector<NpcRole> _roles;
	Mutex *_rolesMutex;
	Point _lastTargetPosition;
	float _motionSense;

	virtual RoleType addMonster(Monster *npc, bool lockMutex);
};

#endif // INTELLIGENCE_H
