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
}



void Game::UpdateScene(float dt)
{
	GamePhysicsTestBed::UpdateScene(dt);

	switch (m_curGameState)
	{
	case GameState::START:  //at the start of the game
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
		break;

	case GameState::SWEEPING: //overhead view of rcok moving down sheet
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
		isValid = newState == GameState::PLANING;
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