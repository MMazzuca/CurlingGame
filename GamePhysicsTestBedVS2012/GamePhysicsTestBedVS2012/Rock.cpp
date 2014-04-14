#include "Rock.h"

Rock* Rock::Throw(btVector3 &initialPosition, Game::Team team, const btVector3 &direction, float Rotation, float Velocity)
{
	Rock* rock = new Rock(initialPosition, team);
	btVector3 velocity = direction; 
	velocity.normalize();
	velocity *= Velocity;
    rock->GetRigidBody()->setLinearVelocity(velocity);
	return rock;
}