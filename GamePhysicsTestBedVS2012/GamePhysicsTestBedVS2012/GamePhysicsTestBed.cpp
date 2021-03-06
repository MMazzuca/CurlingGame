#include "GamePhysicsTestBed.h"

// Some constants for 3D math and the camera speed
#define RADIANS_PER_DEGREE 0.01745329f
#define CAMERA_STEP_SIZE 5.0f

GamePhysicsTestBed::GamePhysicsTestBed() 
:
m_cameraPosition(10.0f, 5.0f, 0.0f),
m_cameraTarget(0.0f, 0.0f, 0.0f),
m_cameraDistance(15.0f),
m_cameraPitch(20.0f),
m_cameraYaw(0.0f),
m_upVector(0.0f, 1.0f, 0.0f),
m_nearPlane(1.0f),
m_farPlane(1000.0f),
m_pBroadphase(0),
m_pCollisionConfiguration(0),
m_pDispatcher(0),
m_pSolver(0),
m_pWorld(0),
m_pMotionState(0),
state(STATE::START),
m_pPickedBody(0),
m_pPickConstraint(0),
m_shootAim(false),
m_mouseAimStartX(0),
m_shootAimMaxYaw(10.0f),
m_shootAimMinYaw(-10.0f),
m_shootPower(0)
{
}

GamePhysicsTestBed::~GamePhysicsTestBed() {
	// shutdown the physics system
	ShutdownPhysics();
}

void GamePhysicsTestBed::Initialize() {
	// this function is called inside glutmain() after
	// creating the window, but before handing control
	// to FreeGLUT

	// create some floats for our ambient, diffuse, specular and position
	GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f }; // dark grey
	GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // white
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // white
	GLfloat position[] = { 5.0f, 10.0f, 1.0f, 0.0f };
	
	// set the ambient, diffuse, specular and position for LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glEnable(GL_LIGHTING); // enables lighting
	glEnable(GL_LIGHT0); // enables the 0th light
	glEnable(GL_COLOR_MATERIAL); // colors materials when lighting is enabled
		
	// enable specular lighting via materials
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	glMateriali(GL_FRONT, GL_SHININESS, 15);
	
	// enable smooth shading
	glShadeModel(GL_SMOOTH);
	
	// enable depth testing to be 'less than'
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// set the backbuffer clearing color to a lightish blue
	glClearColor(0.6, 0.65, 0.85, 0);

	// initialize the physics system
	InitializePhysics();
}
void GamePhysicsTestBed::Keyboard(unsigned char key, int x, int y) {
	// This function is called by FreeGLUT whenever
	// generic keys are pressed down.
	switch (key) {
		//'z' zooms in
	case 'z': ZoomCamera(+CAMERA_STEP_SIZE); break;
		///'x' zoom out
	case 'x': ZoomCamera(-CAMERA_STEP_SIZE); break;
	}
}

void GamePhysicsTestBed::KeyboardUp(unsigned char key, int x, int y) {}

void GamePhysicsTestBed::Special(int key, int x, int y) {
	// This function is called by FreeGLUT whenever special keys
	// are pressed down, like the arrow keys, or Insert, Delete etc.
	switch(key) {
		// the arrow keys rotate the camera up/down/left/right
	case GLUT_KEY_LEFT: 
		RotateCamera(m_cameraYaw, +CAMERA_STEP_SIZE); break;
	case GLUT_KEY_RIGHT:
		RotateCamera(m_cameraYaw, -CAMERA_STEP_SIZE); break;
	case GLUT_KEY_UP:	
		RotateCamera(m_cameraPitch, +CAMERA_STEP_SIZE); break;
	case GLUT_KEY_DOWN:	
		RotateCamera(m_cameraPitch, -CAMERA_STEP_SIZE); break;
	}
}

void GamePhysicsTestBed::SpecialUp(int key, int x, int y) {}

void GamePhysicsTestBed::Reshape(int w, int h) {
	// this function is called once during application intialization
	// and again every time we resize the window

	// grab the screen width/height
	m_screenWidth = w;
	m_screenHeight = h;
	// set the viewport
	glViewport(0, 0, w, h);
	// update the camera
	UpdateCamera();
}

void GamePhysicsTestBed::Idle() {
	// this function is called frequently, whenever FreeGlut
	// isn't busy processing its own events. It should be used
	// to perform any updating and rendering tasks

	// clear the backbuffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		// get the time since the last iteration
	float dt = m_clock.getTimeMilliseconds();
	// reset the clock to 0
		m_clock.reset();
	// update the scene (convert ms to s)
	UpdateScene(dt / 1000.0f);

	// update the camera
	UpdateCamera();

	// draw a simple box of size 1 **/
	// also draw it red **/
	//DrawBox(btVector3(1, 1, 1), btVector3(1.0f, 0.2f, 0.2f)); 

		// render the scene
	RenderScene();

	// swap the front and back buffers
	glutSwapBuffers();
}

void GamePhysicsTestBed::Mouse(int button, int state, int x, int y) 
{

}
// ADDED IN TODAY'S LECTURE STEP 5

/*ADD*/	btVector3 GamePhysicsTestBed::GetPickingRay(int x, int y) {
/*ADD*/		// calculate the field-of-view
/*ADD*/		float tanFov = 1.0f / m_nearPlane;
/*ADD*/		float fov = btScalar(2.0) * btAtan(tanFov);
/*ADD*/	
/*ADD*/		// get a ray pointing forward from the 
/*ADD*/		// camera and extend it to the far plane
/*ADD*/		btVector3 rayFrom = m_cameraPosition;
/*ADD*/		btVector3 rayForward = (m_cameraTarget - m_cameraPosition);
/*ADD*/		rayForward.normalize();
/*ADD*/		rayForward*= m_farPlane;
/*ADD*/	
/*ADD*/		// find the horizontal and vertical vectors 
/*ADD*/		// relative to the current camera view
/*ADD*/		btVector3 ver = m_upVector;
/*ADD*/		btVector3 hor = rayForward.cross(ver);
/*ADD*/		hor.normalize();
/*ADD*/		ver = hor.cross(rayForward);
/*ADD*/		ver.normalize();
/*ADD*/		hor *= 2.f * m_farPlane * tanFov;
/*ADD*/		ver *= 2.f * m_farPlane * tanFov;
/*ADD*/	
/*ADD*/		// calculate the aspect ratio
/*ADD*/		btScalar aspect = m_screenWidth / (btScalar)m_screenHeight;
/*ADD*/	
/*ADD*/		// adjust the forward-ray based on
/*ADD*/		// the X/Y coordinates that were clicked
/*ADD*/		hor*=aspect;
/*ADD*/		btVector3 rayToCenter = rayFrom + rayForward;
/*ADD*/		btVector3 dHor = hor * 1.f/float(m_screenWidth);
/*ADD*/		btVector3 dVert = ver * 1.f/float(m_screenHeight);
/*ADD*/		btVector3 rayTo = rayToCenter - 0.5f * hor + 0.5f * ver;
/*ADD*/		rayTo += btScalar(x) * dHor;
/*ADD*/		rayTo -= btScalar(y) * dVert;
/*ADD*/	
/*ADD*/		// return the final result
/*ADD*/		return rayTo;
/*ADD*/	}
/*ADD*/	
/*ADD*/	void GamePhysicsTestBed::ShootBox(const btVector3 &direction) {
/*ADD*/		// create a new box object
/*ADD*/		GameObject* pObject = CreateGameObject(new btBoxShape(btVector3(1, 1, 1)), 1, btVector3(0.4f, 0.f, 0.4f), m_cameraPosition);
/*ADD*/		
/*ADD*/		// calculate the velocity
/*ADD*/		btVector3 velocity = direction; 
/*ADD*/		velocity.normalize();
/*ADD*/		velocity *= 25.0f;
/*ADD*/		
/*ADD*/		// set the linear velocity of the box
/*ADD*/		pObject->GetRigidBody()->setLinearVelocity(velocity);
/*ADD*/	}
/*ADD*/	
/*ADD*/	bool GamePhysicsTestBed::Raycast(const btVector3 &startPosition, const btVector3 &direction, RayResult &output) {
/*ADD*/		if (!m_pWorld) 
/*ADD*/			return false;
/*ADD*/		
/*ADD*/		// get the picking ray from where we clicked
/*ADD*/		btVector3 rayTo = direction;
/*ADD*/		btVector3 rayFrom = m_cameraPosition;
/*ADD*/		
/*ADD*/		// create our raycast callback object
/*ADD*/		btCollisionWorld::ClosestRayResultCallback rayCallback(rayFrom,rayTo);
/*ADD*/		
/*ADD*/		// perform the raycast
/*ADD*/		m_pWorld->rayTest(rayFrom,rayTo,rayCallback);
/*ADD*/		
/*ADD*/		// did we hit something?
/*ADD*/		if (rayCallback.hasHit())
/*ADD*/		{
/*ADD*/			// if so, get the rigid body we hit
/*ADD*/			btRigidBody* pBody = (btRigidBody*)btRigidBody::upcast(rayCallback.m_collisionObject);
/*ADD*/			if (!pBody)
/*ADD*/				return false;
/*ADD*/		
/*ADD*/			// prevent us from picking objects 
/*ADD*/			// like the ground plane
/*ADD*/			if (pBody->isStaticObject() || pBody->isKinematicObject()) 
/*ADD*/				return false;
/*ADD*/	    
/*ADD*/			// set the result data
/*ADD*/			output.pBody = pBody;
/*ADD*/			output.hitPoint = rayCallback.m_hitPointWorld;
/*ADD*/			return true;
/*ADD*/		}
/*ADD*/	
/*ADD*/		// we didn't hit anything
/*ADD*/		return false;
/*ADD*/	}
/*ADD*/	
/*ADD*/	void GamePhysicsTestBed::DestroyGameObject(btRigidBody* pBody) {
/*ADD*/		// we need to search through the objects in order to 
/*ADD*/		// find the corresponding iterator (can only erase from 
/*ADD*/		// an std::vector by passing an iterator)
/*ADD*/		for (GameObjects::iterator iter = m_objects.begin(); iter != m_objects.end(); ++iter) {
/*ADD*/			if ((*iter)->GetRigidBody() == pBody) {
/*ADD*/				GameObject* pObject = *iter;
/*ADD*/				// remove the rigid body from the world
/*ADD*/				m_pWorld->removeRigidBody(pObject->GetRigidBody());
/*ADD*/				// erase the object from the list
/*ADD*/				m_objects.erase(iter);
/*ADD*/				// delete the object from memory
/*ADD*/				delete pObject;
/*ADD*/				// done
/*ADD*/				return;
/*ADD*/			}
/*ADD*/		}
/*ADD*/	}

void GamePhysicsTestBed::PassiveMotion(int x, int y) 
{

}
// ADDED TO TODAY'S LECTURE STEP 10
void GamePhysicsTestBed::Motion(int x, int y) 
{
}


void GamePhysicsTestBed::Display() {}

void GamePhysicsTestBed::UpdateCamera() {
	// exit in erroneous situations
	if (m_screenWidth == 0 && m_screenHeight == 0)
		return;
	
	// select the projection matrix
	glMatrixMode(GL_PROJECTION);
	// set it to the matrix-equivalent of 1
	glLoadIdentity();
	// determine the aspect ratio of the screen
	float aspectRatio = m_screenWidth / (float)m_screenHeight;
	// create a viewing frustum based on the aspect ratio and the
	// boundaries of the camera
	glFrustum (-aspectRatio * m_nearPlane, aspectRatio * m_nearPlane, -m_nearPlane, m_nearPlane, m_nearPlane, m_farPlane);
	// the projection matrix is now set

	// select the view matrix
	glMatrixMode(GL_MODELVIEW);
	// set it to '1'
	glLoadIdentity();

	// our values represent the angles in degrees, but 3D 
	// math typically demands angular values are in radians.
	float pitch = m_cameraPitch * RADIANS_PER_DEGREE;
	float yaw = m_cameraYaw * RADIANS_PER_DEGREE;

	// create a quaternion defining the angular rotation 
	// around the up vector
	btQuaternion rotation(m_upVector, yaw);

	// set the camera's position to 0,0,0, then move the 'z' 
	// position to the current value of m_cameraDistance.
	btVector3 cameraPosition(0,0,0);
	cameraPosition[2] = -m_cameraDistance;

	// create a Bullet Vector3 to represent the camera 
	// position and scale it up if its value is too small.
	btVector3 forward(cameraPosition[0], cameraPosition[1], cameraPosition[2]);
	if (forward.length2() < SIMD_EPSILON) {
		forward.setValue(1.f,0.f,0.f);
	}

	// figure out the 'right' vector by using the cross 
	// product on the 'forward' and 'up' vectors
	btVector3 right = m_upVector.cross(forward);

	// create a quaternion that represents the camera's roll
	btQuaternion roll(right, - pitch);

	// turn the rotation (around the Y-axis) and roll (around 
	// the forward axis) into transformation matrices and 
	// apply them to the camera position. This gives us the 
	// final position
	cameraPosition = btMatrix3x3(rotation) * btMatrix3x3(roll) * cameraPosition;

	// save our new position in the member variable, and 
	// shift it relative to the target position (so that we 
	// orbit it)
	m_cameraPosition[0] = cameraPosition.getX();
	m_cameraPosition[1] = cameraPosition.getY();
	m_cameraPosition[2] = cameraPosition.getZ();
	m_cameraPosition += m_cameraTarget;

	// create a view matrix based on the camera's position and where it's
	// looking
	//Code added to allow us to switch to a different camera when the rock is thrown
	switch(state)
	{
	case START:
    //if(isThrown == false)
	 gluLookAt(m_cameraPosition[0], m_cameraPosition[1], m_cameraPosition[2], m_cameraTarget[0], m_cameraTarget[1], m_cameraTarget[2], m_upVector.getX(), m_upVector.getY(), m_upVector.getZ());
	 m_cameraPitch = 10.0f;
	break;

	case THROWN:
		//if(isThrown == true)
			gluLookAt(m_cameraPosition[0], m_cameraPosition[1]+90, m_cameraPosition[2], m_cameraTarget[0], m_cameraTarget[1], m_cameraTarget[2], m_upVector.getX(), m_upVector.getY(), m_upVector.getZ());
		    m_cameraPitch = 90.0f;
			break;
	}
	// the view matrix is now set
}


void GamePhysicsTestBed::DrawBox(const btVector3 &halfSize) {

	// push the transform onto the stack
	//glPushMatrix();
	//glMultMatrixf(transform);

	float halfWidth = halfSize.x();
	float halfHeight = halfSize.y();
	float halfDepth = halfSize.z();

	// set the object's color
	//glColor3f(color.x(), color.y(), color.z());

	// create the vertex positions
	btVector3 vertices[8]={	
	btVector3(halfWidth,halfHeight,halfDepth),
	btVector3(-halfWidth,halfHeight,halfDepth),
	btVector3(halfWidth,-halfHeight,halfDepth),	
	btVector3(-halfWidth,-halfHeight,halfDepth),	
	btVector3(halfWidth,halfHeight,-halfDepth),
	btVector3(-halfWidth,halfHeight,-halfDepth),	
	btVector3(halfWidth,-halfHeight,-halfDepth),	
	btVector3(-halfWidth,-halfHeight,-halfDepth)};

	// create the indexes for each triangle, using the 
	// vertices above. Make it static so we don't waste 
	// processing time recreating it over and over again
	static int indices[36] = {
		0,1,2,
		3,2,1,
		4,0,6,
		6,0,2,
		5,1,4,
		4,1,0,
		7,3,1,
		7,1,5,
		5,4,7,
		7,4,6,
		7,2,3,
		7,6,2};

	// start processing vertices as triangles
	glBegin (GL_TRIANGLES);

	// increment the loop by 3 each time since we create a 
	// triangle with 3 vertices at a time.

	for (int i = 0; i < 36; i += 3) {
		// get the three vertices for the triangle based
		// on the index values set above
		// use const references so we don't copy the object
		// (a good rule of thumb is to never allocate/deallocate
		// memory during *every* render/update call. This should 
		// only happen sporadically)
		const btVector3 &vert1 = vertices[indices[i]];
		const btVector3 &vert2 = vertices[indices[i+1]];
		const btVector3 &vert3 = vertices[indices[i+2]];

		// create a normal that is perpendicular to the 
		// face (use the cross product)
		btVector3 normal = (vert3-vert1).cross(vert2-vert1);
		normal.normalize ();

		// set the normal for the subsequent vertices
		glNormal3f(normal.getX(),normal.getY(),normal.getZ());

		// create the vertices
		glVertex3f (vert1.x(), vert1.y(), vert1.z());
		glVertex3f (vert2.x(), vert2.y(), vert2.z());
		glVertex3f (vert3.x(), vert3.y(), vert3.z());
	}

	// stop processing vertices
	glEnd();

		// pop the transform from the stack in preparation
		// for the next object
		//glPopMatrix();
}

void GamePhysicsTestBed::RotateCamera(float &angle, float value) {
	// change the value (it is passed by reference, so we
	// can edit it here)
	angle -= value; 
	// keep the value within bounds
	if (angle < 0) angle += 360; 
	if (angle >= 360) angle -= 360;
	// update the camera since we changed the angular value
	UpdateCamera(); 
}

void GamePhysicsTestBed::ZoomCamera(float distance) {
	// change the distance value
	m_cameraDistance -= distance;
	// prevent it from zooming in too far
	if (m_cameraDistance < 0.1f) m_cameraDistance = 0.1f;
	// update the camera since we changed the zoom distance
	UpdateCamera();
}

	void GamePhysicsTestBed::RenderScene() {
		// create an array of 16 floats (representing a 4x4 matrix)
		btScalar transform[16];
		//iterate through all of the game objects in our world
		for(GameObjects::iterator i = m_objects.begin(); i != m_objects.end(); ++i)
		{
			//get the object from the iterator
			GameObject* pObj = *i;
			// read the transform
			pObj->GetTransform(transform);
			// get the data from the object and draw it
			DrawShape(transform, pObj->GetShape(), pObj->GetColor());
		}
		
	}

	void GamePhysicsTestBed::UpdateScene(float dt) {
		// check if the world object exists
		if (m_pWorld) {
		// step the simulation through time. This is called
			// every update and the amount of elasped time was 
			// determined back in ::Idle() by our clock object.
			m_pWorld->stepSimulation(dt);
		}
	}
	void GamePhysicsTestBed::DrawShape(btScalar* transform, const btCollisionShape* pShape, const btVector3 &color) {
	// set the color
	glColor3f(color.x(), color.y(), color.z());

	// push the matrix stack
	glPushMatrix();
	glMultMatrixf(transform);

	// make a different draw call based on the object type
	switch(pShape->getShapeType()) {
		// an internal enum used by Bullet for boxes
	case BOX_SHAPE_PROXYTYPE:
		{
			// assume the shape is a box, and typecast it
			const btBoxShape* box = static_cast<const btBoxShape*>(pShape);
			// get the 'halfSize' of the box
			btVector3 halfSize = box->getHalfExtentsWithMargin();
			// draw the box
			DrawBox(halfSize);
			break;
		}
	default:
		// unsupported type
		break;
	}

	// pop the stack
	glPopMatrix();
}

	GameObject* GamePhysicsTestBed::CreateGameObject(btCollisionShape* pShape, const float &mass, const btVector3 &color, const btVector3 &initialPosition, const btQuaternion &initialRotation) {
	// create a new game object
	GameObject* pObject = new GameObject(pShape, mass, color, initialPosition, initialRotation);

	// push it to the back of the list
	m_objects.push_back(pObject);

	// check if the world object is valid
	if (m_pWorld) {
		// add the object's rigid body to the world
		m_pWorld->addRigidBody(pObject->GetRigidBody());
	}
	return pObject;
}
// ADDED TO TODAY'S LECTURE STEP 11
void GamePhysicsTestBed::CreatePickingConstraint(int x, int y)
{
		/*ADD*/		if (!m_pWorld) 
/*ADD*/			return;
/*ADD*/	
/*ADD*/		// perform a raycast and return if it fails
/*ADD*/		RayResult output;
/*ADD*/		if (!Raycast(m_cameraPosition, GetPickingRay(x, y), output))
/*ADD*/			return;
/*ADD*/	
/*ADD*/		// store the body for future reference
/*ADD*/		m_pPickedBody = output.pBody;
/*ADD*/	
/*ADD*/		// prevent the picked object from falling asleep
/*ADD*/		m_pPickedBody->setActivationState(DISABLE_DEACTIVATION);
/*ADD*/	
/*ADD*/		// get the hit position relative to the body we hit 
/*ADD*/		btVector3 localPivot = m_pPickedBody->getCenterOfMassTransform().inverse() * output.hitPoint;
/*ADD*/	
/*ADD*/		// create a transform for the pivot point
/*ADD*/		btTransform pivot;
/*ADD*/		pivot.setIdentity();
/*ADD*/		pivot.setOrigin(localPivot);
/*ADD*/	
/*ADD*/		// create our constraint object
/*ADD*/		btGeneric6DofConstraint* dof6 = new btGeneric6DofConstraint(*m_pPickedBody, pivot, true);
/*ADD*/		bool bLimitAngularMotion = true;
/*ADD*/		if (bLimitAngularMotion) {
/*ADD*/			dof6->setAngularLowerLimit(btVector3(0,0,0));
/*ADD*/			dof6->setAngularUpperLimit(btVector3(0,0,0));
/*ADD*/		}
/*ADD*/	
/*ADD*/		// add the constraint to the world
/*ADD*/		m_pWorld->addConstraint(dof6,true);
/*ADD*/	
/*ADD*/		// store a pointer to our constraint
/*ADD*/		m_pPickConstraint = dof6;
/*ADD*/	
/*ADD*/		// define the 'strength' of our constraint (each axis)
/*ADD*/		float cfm = 0.5f;
/*ADD*/		dof6->setParam(BT_CONSTRAINT_STOP_CFM,cfm,0);
/*ADD*/		dof6->setParam(BT_CONSTRAINT_STOP_CFM,cfm,1);
/*ADD*/		dof6->setParam(BT_CONSTRAINT_STOP_CFM,cfm,2);
/*ADD*/		dof6->setParam(BT_CONSTRAINT_STOP_CFM,cfm,3);
/*ADD*/		dof6->setParam(BT_CONSTRAINT_STOP_CFM,cfm,4);
/*ADD*/		dof6->setParam(BT_CONSTRAINT_STOP_CFM,cfm,5);
/*ADD*/	
/*ADD*/		// define the 'error reduction' of our constraint (each axis)
/*ADD*/		float erp = 0.5f;
/*ADD*/		dof6->setParam(BT_CONSTRAINT_STOP_ERP,erp,0);
/*ADD*/		dof6->setParam(BT_CONSTRAINT_STOP_ERP,erp,1);
/*ADD*/		dof6->setParam(BT_CONSTRAINT_STOP_ERP,erp,2);
/*ADD*/		dof6->setParam(BT_CONSTRAINT_STOP_ERP,erp,3);
/*ADD*/		dof6->setParam(BT_CONSTRAINT_STOP_ERP,erp,4);
/*ADD*/		dof6->setParam(BT_CONSTRAINT_STOP_ERP,erp,5);
/*ADD*/	
/*ADD*/		// save this data for future reference
/*ADD*/		m_oldPickingDist  = (output.hitPoint - m_cameraPosition).length();
/*ADD*/	}
// ADDED TO TODAY'S LECTURE STEP 12
/*ADD*/	void GamePhysicsTestBed::RemovePickingConstraint() {
/*ADD*/		// exit in erroneous situations
/*ADD*/		if (!m_pPickConstraint || !m_pWorld) 
/*ADD*/			return;
/*ADD*/	
/*ADD*/		// remove the constraint from the world
/*ADD*/		m_pWorld->removeConstraint(m_pPickConstraint);
/*ADD*/	
/*ADD*/		// delete the constraint object
/*ADD*/		delete m_pPickConstraint;
/*ADD*/	
/*ADD*/		// reactivate the body
/*ADD*/		m_pPickedBody->forceActivationState(ACTIVE_TAG);
/*ADD*/		m_pPickedBody->setDeactivationTime( 0.f );
/*ADD*/	
/*ADD*/		// clear the pointers
/*ADD*/		m_pPickConstraint = 0;
/*ADD*/		m_pPickedBody = 0;
/*ADD*/	}
//ADDED TO TODAY'S LECTURE STEP 12



//Functions created to handle objects and remove them.
GameObjects const* GamePhysicsTestBed::GetGameObjects() const
{
	return &m_objects;
}


void GamePhysicsTestBed::RemoveGameObject(GameObject * obj)
{
	GameObjects::iterator it;

	for(it = m_objects.begin(); it != m_objects.end() && *it != obj; ++it);

	if(it != m_objects.end())
	{
		m_pWorld->removeRigidBody((*it)->GetRigidBody());
		delete *it;
		m_objects.erase(it);
	}
}

void GamePhysicsTestBed::RemoveRocks()
{
	for(GameObjects::iterator it = m_objects.begin(); it != m_objects.end();)
	{
		if(Object::ObjectType::ROCK == (*it)->getType())
		{
			m_pWorld->removeRigidBody((*it)->GetRigidBody());
			delete *it;
			it = m_objects.erase(it);
		}
		else
			++it;
	}
}