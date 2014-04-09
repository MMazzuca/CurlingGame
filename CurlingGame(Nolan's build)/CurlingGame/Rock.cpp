#include "Rock.h"
Rock::Rock()
{
  h = 0;
  w = 0;
}

Rock::Rock(float X, float Y)
{
	x = X;
	y = Y;
	w = 1;
	h = 1;
	visible = true;
}

void Rock::draw()
{
	glPushMatrix();
	glTranslatef(x, y, 0);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(w,h,0);
	glPopMatrix();
}

void Rock::update(float dt)
{
	switch(state)
	{
	 case IDLE:
		 if(isThrown == false)
			 x -= 1*dt;
			 moving = false;
	     break;
	 case THROWN:
		 if(x <= speed)
		 {
			 x += 1*dt;
		 }

		 else
		 {
			 state = IDLE;
		 }

		 break;
	}
}

void Rock::move()
{
	if(moving != true)
	{
		speed = x + 4;
		moving = true;
		state = THROWN;
	}
}

