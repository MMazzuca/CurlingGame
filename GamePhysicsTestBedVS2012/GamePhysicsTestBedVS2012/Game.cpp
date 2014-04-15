#include "Game.h"


Game::Game() :
PhysicsDemo(),
m_curGameState(GameState::START),
m_activeTeam(Object::Team::TEAM_BLUE),
m_end(6.0f, btVector3(0, 0, 85.0f)),
mptr_activeRock(NULL),
m_scoreBlue(0),
m_scoreRed(0),
m_rocksBlue(0),
m_rocksRed(0),
m_scoringTimer(0),
m_limitsX(12),
m_limitsY(10),
m_limitsZ(100),
m_limitZHog(60),
m_round(0)

{
	//draw hosue and hog line
	//CreateGameObject(new btBoxShape(btVector3(1,6,6)), 0, btVector3(1.0f, 0.6f, 0.6f), btVector3(0.0f, 0.0f, 85.0f));
	//CreateGameObject(new btBoxShape(btVector3(1,12,1)), 0, btVector3(0.0f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, m_limitZHog));

	//Test boundary walls
	//CreateGameObject(new btBoxShape(btVector3(10,1,100)), 0, btVector3(0.2f, 0.6f, 1.0f), btVector3(12.0f, 0.0f, 0.0f));
	//CreateGameObject(new btBoxShape(btVector3(10,1,100)), 0, btVector3(0.2f, 0.6f, 1.0f), btVector3(-12.0f, 0.0f, 0.0f));
	//CreateGameObject(new btBoxShape(btVector3(10,12,1)), 0, btVector3(1.0f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, -100.0f));
	//CreateGameObject(new btBoxShape(btVector3(10,12,1)), 0, btVector3(1.0f, 0.0f, 0.0f), btVector3(0.0f, 0.0f, 100.0f));
	
	
	// create a ground plane
	//CreateGameObject(new btBoxShape(btVector3(1,12,100)), 0, btVector3(0.2f, 0.6f, 0.6f), btVector3(0.0f, 0.0f, 0.0f));


	ChangeState(GameState::START);
	//ChangeState(GameState::SHOOTING);
}



void Game::UpdateScene(float dt)
{
	GamePhysicsTestBed::UpdateScene(dt);

	switch (m_curGameState)
	{
	case GameState::START:  //at the start of the game
		//clean up the sheet
		RemoveRocks();

		m_startTimer += dt;
		if(m_startTimer > START_TIMER)
		{
			ChangeState(GameState::PLANING);
		}
		break;

	case GameState::END:  //at the end of the game
		break;

	case GameState::PLANING: //overlooking the house when planing where to shoot
		m_planingTimer += dt;
		if (m_planingTimer > PLANING_TIMER)
		{
			ChangeState(GameState::SHOOTING);
		}
		break;

	case GameState::SHOOTING: //first person aiming and shooting
		if(NULL != mptr_activeRock)
		{
			ChangeState(GameState::SWEEPING);
		}
		break;

	case GameState::SWEEPING: //overhead view of rcok moving down sheet
		assert(NULL != mptr_activeRock);

		if (NULL != mptr_activeRock)
		{
			//check if rock is out of bounds
			btTransform transform;
			btVector3 position;
			mptr_activeRock->GetMotionState()->getWorldTransform(transform);
			position = transform.getOrigin();

			if(std::abs(position.x()) > std::abs(m_limitsX) || std::abs(position.z()) > std::abs(m_limitsZ) || std::abs(position.y()) > std::abs(m_limitsY))
			{
				RemoveGameObject(mptr_activeRock);
				ChangeState(GameState::PLANING);
				mptr_activeRock = NULL;
			}

			//check if rock is still moving
			if (NULL != mptr_activeRock && mptr_activeRock->GetRigidBody()->getLinearVelocity().length() < ROCK_MOVE_THREASHOLD)
			{
				if(std::abs(position.z()) < std::abs(m_limitZHog))
				{
					RemoveGameObject(mptr_activeRock);
				}
				ChangeState(GameState::PLANING);
				mptr_activeRock = NULL;
			}
		}
		else
		{
			ChangeState(GameState::PLANING);
		}
			
		break;

	case GameState::SCORING: //overlooking the hosue showing rocks that are scoring
		//pause x seconds then switch
		m_scoringTimer += dt;
		if (m_scoringTimer > SCORING_TIMER)
		{
			ChangeState(GameState::START);
		}
		break;
	}

}


void Game::ChangeState(GameState newState)
{
	m_cameraYaw = 0;

	if (ValidStateChange(newState))
	{
		switch (newState)
		{
		case GameState::START:
			m_curGameState = GameState::START;
			m_rocksBlue = m_rocksRed = NUM_ROCKS;
			m_startTimer = 0;
			++m_round;
			if(m_round > MAX_ROUNDS)
			{
				ChangeState(GameState::END);
			}
			break;

		case GameState::END:
			m_curGameState = GameState::END;
			break;

		case GameState::PLANING:
			m_curGameState = GameState::PLANING;
			m_planingTimer = 0;
			SwitchTeams();
			if (GetTeamRocks(m_activeTeam) < 1)
			{
				ChangeState(GameState::SCORING);
			}
			//set camera over house
			state = STATE::THROWN;
			break;

		case GameState::SHOOTING:
			m_curGameState = GameState::SHOOTING;
			//set camera first person
			state = STATE::START;
			break;

		case GameState::SWEEPING:
			m_curGameState = GameState::SWEEPING;
			//set camera over sheet
			state = STATE::THROWN;
			break;

		case GameState::SCORING:
			m_curGameState = GameState::SCORING;
			m_scoringTimer = 0;

			int score = m_end.GetScore(this);
			if (score < 0)
				UpdateTeamScore(Object::Team::TEAM_BLUE, -score);
			else
				UpdateTeamScore(Object::Team::TEAM_RED, score);
			//set camera over house
			state = STATE::THROWN;

			break;
		}
	}

	LazyHud();
}

bool Game::ValidStateChange(GameState newState)
{
	bool isValid = false;


	switch (m_curGameState)
	{
	case GameState::START:
		isValid = newState == GameState::START|| newState == GameState::PLANING || newState == GameState::END;
		break;

	case GameState::END:
		isValid = newState == GameState::START;
		break;

	case GameState::PLANING:
		isValid = newState == GameState::SHOOTING || newState == GameState::SCORING || newState == GameState::END;
		break;

	case GameState::SHOOTING:
		isValid = newState == GameState::SWEEPING || newState == GameState::END;
		break;

	case GameState::SWEEPING:
		isValid = newState == GameState::PLANING || newState == GameState::SCORING || newState == GameState::END;
		break;

	case GameState::SCORING:
		isValid = newState == GameState::START || newState == GameState::PLANING || newState == GameState::END;
		break;
	}

	return isValid;
}


Rock * Game::ThrowRock(btVector3 &initialPosition, const btVector3 &direction, float rotation, float velocity)
{
	Rock * rock = Rock::Throw(initialPosition, m_activeTeam, direction, rotation, velocity);

	// push it to the back of the list
	m_objects.push_back(rock);

	// check if the world object is valid
	if (m_pWorld)
	{
		// add the object's rigid body to the world
		m_pWorld->addRigidBody(rock->GetRigidBody());
	}
	
	return rock;
}

void Game::SwitchTeams()
{
	switch (m_activeTeam)
	{
	case Object::Team::TEAM_BLUE:
		m_activeTeam = Object::Team::TEAM_RED;
		break;

	case Object::Team::TEAM_RED:
		m_activeTeam = Object::Team::TEAM_BLUE;
		break;
	}
}

int Game::GetTeamRocks(Object::Team team)
{
	switch (team)
	{
	case Object::Team::TEAM_BLUE:
		return m_rocksBlue;
		break;

	case Object::Team::TEAM_RED:
		return m_rocksRed;
		break;
	}

	return m_rocksBlue;
}

void Game::UpdateTeamRocks(Object::Team team, int change)
{
	switch (team)
	{
	case Object::Team::TEAM_BLUE:
		m_rocksBlue += change;
		break;

	case Object::Team::TEAM_RED:
		m_rocksRed += change;
		break;
	}
}

void Game::UpdateTeamScore(Object::Team team, int change)
{
	switch (team)
	{
	case Object::Team::TEAM_BLUE:
		m_scoreBlue += change;
		break;

	case Object::Team::TEAM_RED:
		m_scoreRed += change;
		break;
	}
}


void Game::Mouse(int button, int state, int x, int y) 
{
	GamePhysicsTestBed::Mouse(button, state, x, y);
	
	switch (m_curGameState)
	{
	case GameState::START:
		break;

	case GameState::END:
		break;

	case GameState::PLANING:
		break;

	case GameState::SHOOTING:
		MouseShooting(button, state, x, y);
		break;

	case GameState::SWEEPING:
		break;

	case GameState::SCORING:
		break;
	}
}

void Game::MouseShooting(int button, int state, int x, int y)
{
	switch (button)
	{

	case 0:  // left mouse button
		if (state == 0)
		{
			m_shootPower = 0;
			m_shootPowerStart = std::clock();
		}
		else
		{
			m_shootPower = static_cast<float>(SHOOTING_VELOCITY_FULL) / static_cast<float>(SHOOTING_TIMER_FULL)  * (std::clock() - m_shootPowerStart) / static_cast<float>(CLOCKS_PER_SEC);
			mptr_activeRock = ThrowRock(m_cameraPosition, GetPickingRay(m_screenWidth * 0.5f, m_screenWidth * 0.5f), -5, m_shootPower);
			UpdateTeamRocks(m_activeTeam, -1);
		}
		break;

	case 2: // right mouse button
		if (state == 0)
		{
			m_shootAim = true;
			m_mouseAimStartX = x;
		}
		else
		{
			m_shootAim = false;
		}

		break;
	}
}


void Game::Motion(int x, int y) 
{
	GamePhysicsTestBed::Motion(x, y);

	switch (m_curGameState)
	{
	case GameState::START:
		break;

	case GameState::END:
		break;

	case GameState::PLANING:
		break;

	case GameState::SHOOTING:
		MotionShooting(x, y);
		break;

	case GameState::SWEEPING:
		break;

	case GameState::SCORING:
		break;
	}

}

void Game::MotionShooting(int x, int y)
{
	if (m_shootAim)
	{
		m_cameraYaw += (m_mouseAimStartX - x) * 0.1f;
		m_mouseAimStartX = x;
		if (m_cameraYaw > m_shootAimMaxYaw)
			m_cameraYaw = m_shootAimMaxYaw;
		else if (m_cameraYaw < m_shootAimMinYaw)
			m_cameraYaw = m_shootAimMinYaw;
	}
}


void Game::LazyHud() const
{
	system("CLS");

	std::cout << "Welome to Curling" << std::endl;
	std::cout << "=================" << std::endl;
	std::cout << "Round: " << m_round << std::endl;
	std::cout << "Current Phase: ";
	
	switch (m_curGameState)
	{
	case GameState::START:
		std::cout << "Start";
		break;

	case GameState::END:
		std::cout << "End";
		break;

	case GameState::PLANING:
		std::cout << "Planing";
		break;

	case GameState::SHOOTING:
		std::cout << "Shooting";
		break;

	case GameState::SWEEPING:
		std::cout << "Sweeping";
		break;

	case GameState::SCORING:
		std::cout << "Scoring";
		break;
	}
	std::cout << std::endl;

	std::cout << "Active Team: " << ( Object::Team::TEAM_BLUE == m_activeTeam ? "Blue" : "Red") << std::endl << std::endl;
	std::cout << "Blue Rocks: " << m_rocksBlue << std::endl;
	std::cout << "Red Rocks:  " << m_rocksRed << std::endl;

	std::cout << std::endl;
	std::cout << "Scores: " << std::endl;
	std::cout << "Red:  " << m_scoreRed << std::endl;
	std::cout << "Blue: " << m_scoreBlue << std::endl;

}