#include "Rock.h"
Rock::Rock(btVector3 &initialPosition, Team team) : GameObject(new btBoxShape(btVector3(1, 1, 1)), 1, (Object::Team::TEAM_BLUE == team ? btVector3(0.0f, 0.0f, 1.0f) : btVector3(1.0f, 0.0f, 0.0f)), initialPosition)
{

	 
}

 Rock* Rock::Throw(btVector3 &initialPosition, Team team, const btVector3 &direction, float Rotation, float Velocity)
 {
	Rock* rock = new Rock(initialPosition, team);
	btVector3 velocity = direction; 
	velocity.normalize();
	velocity *= Velocity;
    rock->GetRigidBody()->setLinearVelocity(velocity);
	return rock;
 }

 Object::Team Rock::getTeam() const
 {
	 return team;
 }

 Object::ObjectType Rock::getType() const
 {
	 return ObjectType::ROCK;
 }

Rock::~Rock()
{

}