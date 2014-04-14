#include "GameObject.h"
#include "Game.h"

class Rock:GameObject
{
public:
	Rock(btVector3 &initialPosition, Game::Team);
	~Rock();
	Game::Team team;
	Game::Team getTeam() const;
	static Rock* Throw(btVector3 &initialPosition, Game::Team team, const btVector3 &direction, float Rotation, float Velocity);
	ObjectType getType();
};