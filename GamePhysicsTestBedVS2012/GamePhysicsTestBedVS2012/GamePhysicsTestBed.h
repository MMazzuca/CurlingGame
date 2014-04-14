#ifndef _GAMEPHYSICSAPP_H_
#define _GAMEPHYSICSAPP_H_

#include <Windows.h>
#include <GL/GL.h>
#include <GL/freeglut.h>
#include <iostream>
#include "OVR.h"
#include <ctime>

#include "BulletDynamics/Dynamics/btDynamicsWorld.h"

#include "OpenGLMotionState.h"
// added to this project
#include "GameObject.h"
#include <vector>
// a convient typedef to reference an stl vector of GameObjects
typedef std::vector<GameObject*> GameObjects;
// added in today's lecture
// step one
/*ADD*/	// struct to store our raycasting results
/*ADD*/	struct RayResult {
/*ADD*/		btRigidBody* pBody;
/*ADD*/		btVector3 hitPoint;
/*ADD*/	};

using namespace OVR;

enum STATE { START, THROWN };

class GamePhysicsTestBed {
public:


	GamePhysicsTestBed();
	~GamePhysicsTestBed();
	void Initialize();
	// FreeGLUT callbacks //
	STATE state;
	virtual void Keyboard(unsigned char key, int x, int y);
	virtual void KeyboardUp(unsigned char key, int x, int y);
	virtual void Special(int key, int x, int y);
	virtual void SpecialUp(int key, int x, int y);
	virtual void Reshape(int w, int h);
	virtual void Idle();
	virtual void Mouse(int button, int state, int x, int y);
	virtual void PassiveMotion(int x, int y);
	virtual void Motion(int x, int y);
	virtual void Display();
	//virtual float Approach(float flGoal,float flCurrent,float dt);

		// camera functions
	void UpdateCamera();
	void RotateCamera(float &angle, float value);
	void ZoomCamera(float distance);
	// drawing functions
	// rendering. Can be overrideen by derived classes
	virtual void RenderScene();

	// scene updating. Can be overridden by derived classes
	virtual void UpdateScene(float dt);
		
		// physics functions. Can be overrideen by derived classess
	virtual void InitializePhysics() {};
	virtual void ShutdownPhysics() {};
	// camera functions

	// drawing functions
	//void DrawBox(btScalar* transform, const btVector3 &halfSize, const btVector3 &color = btVector3(1.0f, 1.0f, 1.0f));
	// updated in this version
	
	// drawing functions
	void DrawBox(const btVector3 &halfSize);
	void DrawShape(btScalar* transform, const btCollisionShape* pShape, const btVector3 &color);

	// object functions
	GameObject* CreateGameObject(btCollisionShape* pShape, 
			const float &mass, 
			const btVector3 &color = btVector3(1.0f,1.0f,1.0f), 
			const btVector3 &initialPosition = btVector3(0.0f,0.0f,0.0f), 
			const btQuaternion &initialRotation = btQuaternion(0,0,1,1));
	// added in today's lecture step 2
	/*ADD*/		void ShootBox(const btVector3 &direction);
	/*ADD*/		void DestroyGameObject(btRigidBody* pBody);

	/*ADD*/		// picking functions
	/*ADD*/		btVector3 GetPickingRay(int x, int y);
	/*ADD*/		bool Raycast(const btVector3 &startPosition, const btVector3 &direction, RayResult &output);
	// ADDED TO TODAY'S LECTURE STEP 7 CONSTRAINT FUNCTIONS
	/*ADD*/		void CreatePickingConstraint(int x, int y);
	/*ADD*/		void RemovePickingConstraint();


	GameObjects const* GetGameObjects() const;

	void RemoveGameObject(GameObject * obj);
	void RemoveRocks();

protected:
	// ADDED TO TODAY'S LECTURE STEP 8 CONSTRAINT VARIABLES
	/*ADD*/		btRigidBody* m_pPickedBody;				// the body we picked up
	/*ADD*/		btTypedConstraint*  m_pPickConstraint;	// the constraint the body is attached to
	/*ADD*/		btScalar m_oldPickingDist;				// the distance from the camera to the hit point (so we can move the object up, down, left and right from our view)


	// camera control
	btVector3 m_cameraPosition; // the camera's current position
	btVector3 m_cameraTarget;	 // the camera's lookAt target
	float m_nearPlane; // minimum distance the camera will render
	float m_farPlane; // farthest distance the camera will render
	btVector3 m_upVector; // keeps the camera rotated correctly
	float m_cameraDistance; // distance from the camera to its target
	float m_cameraPitch; // pitch of the camera 
	float m_cameraYaw; // yaw of the camera

	bool m_shootAim;
	int m_mouseAimStartX;
	float m_shootAimMaxYaw;
	float m_shootAimMinYaw;
	float m_shootPower;
	std::clock_t m_shootPowerStart;


	int m_screenWidth;
	int m_screenHeight;

	// core Bullet components
	btBroadphaseInterface* m_pBroadphase;
	btCollisionConfiguration* m_pCollisionConfiguration;
	btCollisionDispatcher* m_pDispatcher;
	btConstraintSolver* m_pSolver;
	btDynamicsWorld* m_pWorld;

	// our custom motion state
	OpenGLMotionState* m_pMotionState;
		
	// a simple clock for counting time
	btClock m_clock;

	// added in this implementation
	GameObjects m_objects;
	

};
#endif
