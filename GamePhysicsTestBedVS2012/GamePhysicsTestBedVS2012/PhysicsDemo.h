#ifndef PHYSICDEMO_H
#define PHYSICDEMO_H

#include "GamePhysicsTestBed.h"
#include "btBulletDynamicsCommon.h"

class PhysicsDemo : public GamePhysicsTestBed {
public:
	virtual void InitializePhysics() override;
	virtual void ShutdownPhysics() override;

		void CreateObjects();
};

#endif