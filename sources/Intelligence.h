#ifndef INTELLIGENCE_H
#define INTELLIGENCE_H

#include "Player.h"
#include "Monster.h"
#include "LevelMap.h"
#include <vector>

// Artificial Intelligence
class Intelligence
{
public:
	Intelligence();
	~Intelligence();

	virtual void clear();
	virtual void setTarget(Player *target);
	virtual void whatMeDo(Monster *npc);

protected:
	enum RoleType
	{
		NONE, FRONT_ATTACK, BACK_ATTACK, SIDE_ATTACK, GUARD
	};

	struct NpcRole {
		Monster *npc;
		RoleType role;
	};

	LevelMap *_map;
	Player *_target;
	std::vector<NpcRole> _roles;

};

#endif // INTELLIGENCE_H
