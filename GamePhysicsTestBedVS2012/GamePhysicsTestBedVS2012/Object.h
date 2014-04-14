#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "btBulletDynamicsCommon.h"
#include "OpenGLMotionState.h"

class Object
{
public: 
	Object();
	~Object();
	enum ObjectType{BASE, ROCK};
	OpenGLMotionState*  m_pMotionState;
	virtual ObjectType getType() = 0;
};

#endif