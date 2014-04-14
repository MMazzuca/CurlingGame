#include "Game.h"


Game::Game() :
PhysicsDemo(),
m_curGameState(GameState::START),
m_activeTeam(Team::TEAM_BLUE),
m_end(10.0f, btVector3(0, 0, 50)),
mptr_activeRock(NULL),
m_scoreBlue(0),
m_scoreRed(0),
m_rocksBlue(0),
m_rocksRed(0),
m_scoringTimer(0)
{
	ChangeState(GameState::START);
}



void Game::UpdateScene(float dt)
{
	GamePhysicsTestBed::UpdateScene(dt);

	switch (m_curGameState)
	{
	case GameState::START:  //at the start of the game
		//imediatly change 
		m_startTimer += dt;
		if(m_startTimer > START_TIMER)
		{
			ChangeState(GameState::SHOOTING);
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
			//check if rock is still moving
			if (mptr_activeRock->GetRigidBody()->getLinearVelocity().length() < ROCK_MOVE_THREASHOLD)
			{
				ChangeState(GameState::PLANING);
			}
		}
			
		break;

	case GameState::SCORING: //overlooking the hosue showing rocks that are scoring
		//pause x seconds then switch
		m_scoringTimer += dt;
		if (m_scoringTimer > SCORING_TIMER)
		{
			ChangeState(GameState::PLANING);
		}
		break;
	}

}


void Game::ChangeState(GameState newState)
{
	if (ValidStateChange(newState))
	{
		switch (newState)
		{
		case GameState::START:
			m_rocksBlue = m_rocksRed = 3;
			m_startTimer = 0;
			break;

		case GameState::END:
			break;

		case GameState::PLANING:
			m_planingTimer = 0;
			SwitchTeams();
			if (GetTeamRocks(m_activeTeam) < 1)
			{
				ChangeState(GameState::SCORING);
			}
			//set camera over house
			break;

		case GameState::SHOOTING:
			//set camera first person
			break;

		case GameState::SWEEPING:
			//set camera over sheet
			break;

		case GameState::SCORING:
			m_scoringTimer = 0;

			int score = m_end.GetScore(this);
			if (score < 0)
				UpdateTeamScore(Team::TEAM_BLUE, -score);
			else
				UpdateTeamScore(Team::TEAM_RED, score);
			//set camera over house

			break;
		}
	}
}

bool Game::ValidStateChange(GameState newState)
{
	bool isValid = false;


	switch (m_curGameState)
	{
	case GameState::START:
		isValid = newState == GameState::START|| newState == GameState::PLANING;
		break;

	case GameState::END:
		isValid = newState == GameState::START;
		break;

	case GameState::PLANING:
		isValid = newState == GameState::SHOOTING || newState == GameState::END;
		break;

	case GameState::SHOOTING:
		isValid = newState == GameState::SWEEPING || newState == GameState::END;
		break;

	case GameState::SWEEPING:
		isValid = newState == GameState::PLANING || newState == GameState::SCORING || newState == GameState::END;
		break;

	case GameState::SCORING:
		isValid = newState == GameState::PLANING || newState == GameState::END;
		break;
	}

	return isValid;
}


/*Rock * Game::ThrowRock()
{
	Rock * rock = Rock::ThrowRock();
	
	// push it to the back of the list
	m_objects.push_back(rock);

	// check if the world object is valid
	if (m_pWorld)
	{
		// add the object's rigid body to the world
		m_pWorld->addRigidBody(rock->GetRigidBody());
	}
	return rock;
}*/

void Game::SwitchTeams()
{
	switch (m_activeTeam)
	{
	case Team::TEAM_BLUE:
		m_activeTeam = Team::TEAM_RED;
		break;

	case Team::TEAM_RED:
		m_activeTeam = Team::TEAM_BLUE;
		break;
	}
}

int Game::GetTeamRocks(Team team)
{
	switch (team)
	{
	case Team::TEAM_BLUE:
		return m_rocksBlue;
		break;

	case Team::TEAM_RED:
		return m_rocksRed;
		break;
	}

	return Team::TEAM_BLUE;
}

void Game::UpdateTeamRocks(Team team, int change)
{
	switch (team)
	{
	case Team::TEAM_BLUE:
		m_rocksBlue += change;
		break;

	case Team::TEAM_RED:
		m_rocksRed += change;
		break;
	}
}

void Game::UpdateTeamScore(Team team, int change)
{
	switch (team)
	{
	case Team::TEAM_BLUE:
		m_scoreBlue += change;
		break;

	case Team::TEAM_RED:
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
			m_shootPower = (std::clock() - m_shootPowerStart) / static_cast<float>(CLOCKS_PER_SEC);
			ShootBox(GetPickingRay(m_screenWidth * 0.5f, m_screenWidth * 0.5f));
			/*mptr_activeRock = ThrowRock(pos, team, dir, rotation, velocity);*/
			

			isThrown = true;
			this->state = THROWN;
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