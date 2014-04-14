#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "btBulletDynamicsCommon.h"
#include "OpenGLMotionState.h"

class Object
{
public: 
	enum Team
	{
		TEAM_RED,
		TEAM_BLUE
	};
	enum ObjectType
	{
		BASE,
		ROCK
	};

	Object();
	~Object();
	OpenGLMotionState*  m_pMotionState;
	virtual ObjectType getType() const = 0;
};

#endif