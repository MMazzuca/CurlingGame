#include "House.h"

House::House(float size, btVector3 const& initialPosition) : m_size(size), GameObject(new btBoxShape(btVector3(1, 25, 100)), 0, initialPosition, btVector3(0.0f, 0.0f, 0.0f))
{
}

House::~House()
{
}


int House::GetScore(GamePhysicsTestBed const* testBed) const
{
	GameObjects scoringRocks;

	int score = 0;

	//While still finding rocks closest to the button that are of the same team.
	while (FindClosest(testBed, scoringRocks));

	score = scoringRocks.size();

	//Check if the scoring team is blue, if so negate score
	if (!scoringRocks.empty() && static_cast<Rock*>(scoringRocks.front())->getTeam() == TEAM_BLUE)
	{
		score *= -1;
	}

	return score;
}

bool House::FindClosest(GamePhysicsTestBed const* testBed, GameObjects & scoringRocks) const
{
	bool isCanidate = false;
	float curDist = 0;
	float closestDist = -1;
	bool rtnFound = false;

	GameObject * ptr_closest = NULL;
	btMotionState * motionState = NULL;
	btTransform transform;
	btTransform houseTransform;

	m_pMotionState->getWorldTransform(houseTransform);

	GameObjects const* objects = testBed->GetGameObjects();
	for (GameObjects::const_iterator it = objects->cbegin(); it != objects->cend(); ++it)
	{
		//if this is a rock (Need rock class)
		if(Object::ObjectType::ROCK == (*it)->getType())
		{
			motionState = (*it)->GetMotionState();
			motionState->getWorldTransform(transform);

			curDist = (transform.getOrigin() - houseTransform.getOrigin()).length();

			isCanidate = curDist < m_size;

			//Check this rock isn't already in the scoring list
			for (GameObjects::const_iterator it2 = scoringRocks.cbegin(); it2 != scoringRocks.cend() && isCanidate; ++it2)
			{
				if (*it == *it2)
					isCanidate = false;
			}

			if (isCanidate && (NULL == ptr_closest || curDist < closestDist))
			{
				ptr_closest = *it;
				closestDist = curDist;
			}
		}
	}

	//If there was a next closest rock and it belongs to the same team as the closest.
	if (NULL != ptr_closest && (scoringRocks.empty() || static_cast<Rock*>(scoringRocks.front())->getTeam() == static_cast<Rock*>(ptr_closest)->getTeam()))
	{
		scoringRocks.push_back(ptr_closest);
		rtnFound = true;
	}

	return rtnFound;
}