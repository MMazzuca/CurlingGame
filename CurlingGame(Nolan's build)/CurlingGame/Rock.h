#pragma once
#include "Object.h"

enum STATE{IDLE, THROWN};

class Rock : public Object
{
  public:
	  STATE state;
	  Rock();
	  Rock(float X, float Y);
	  void draw();
	  void move();
	  void update(float dt);
	  bool moving; 
	  bool isThrown;
	  bool isCollided;
	  float speed;
};