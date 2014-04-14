#ifndef HOUSE_H
#define HOUSE_H

#include "GameObject.h"
#include "GamePhysicsTestBed.h"
#include "Rock.h"

class Rock;
class Game;

class House : public GameObject
{
public:
	House(float size, btVector3 const& initialPosition = btVector3(0, 0, 0));
	~House();


	int GetScore(GamePhysicsTestBed const* testBed) const;

private:
	float m_size;

	bool House::FindClosest(GamePhysicsTestBed const* testBed, GameObjects & objects) const;
};

#endif