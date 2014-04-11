#include "PhysicsDemo.h"

void PhysicsDemo::InitializePhysics() {
	// create the collision configuration
	m_pCollisionConfiguration = new btDefaultCollisionConfiguration();
	// create the dispatcher
	m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfiguration);
	// create the broadphase
	m_pBroadphase = new btDbvtBroadphase();
	// create the constraint solver
	m_pSolver = new btSequentialImpulseConstraintSolver();
	// create the world
	m_pWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfiguration);

	// create our scene's physics objects
	CreateObjects();
}

void PhysicsDemo::ShutdownPhysics() {
	delete m_pWorld;
	delete m_pSolver;
	delete m_pBroadphase;
	delete m_pDispatcher;
	delete m_pCollisionConfiguration;
}

	void PhysicsDemo::CreateObjects() {

	// create a ground plane
		CreateGameObject(new btBoxShape(btVector3(1,25,100)), 0, btVector3(0.2f, 0.6f, 0.6f), btVector3(0.0f, 0.0f, 0.0f));

		//Walls of the playing field
		CreateGameObject(new btBoxShape(btVector3(4.0,1,100)), 0, btVector3(1.0f, 1.0f, 1.0f), btVector3(5.0f, 1.0f, 0.0f));

		CreateGameObject(new btBoxShape(btVector3(4.0,1,100)), 0, btVector3(1.0f, 1.0f, 1.0f), btVector3(-5.0f, 1.0f, 0.0f));

		CreateGameObject(new btBoxShape(btVector3(4.0, 4.0, 1)), 0, btVector3(1.0f, 1.0f, 1.0f), btVector3(0.0f, 1.0f, 100.0f));
	}