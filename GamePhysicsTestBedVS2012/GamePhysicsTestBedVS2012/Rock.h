#ifndef ROCK_H
#define ROCK_H

#include "GameObject.h"

class Rock: public GameObject
{
public:
	Rock(btVector3 &initialPosition, Team team);
	~Rock();
	Team team;
	Team getTeam() const;
	static Rock* Throw(btVector3 &initialPosition, Team team, const btVector3 &direction, float Rotation, float Velocity);
	ObjectType getType() const;
};


#endif