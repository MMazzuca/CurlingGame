#ifndef GAME_H
#define GAME_H

#include <ctime>
#include <cassert>

#include "House.h"
#include "PhysicsDemo.h"

#define ROCK_MOVE_THREASHOLD 0.001
#define START_TIMER 5
#define SCORING_TIMER 5
#define PLANING_TIMER 10

class Game : public PhysicsDemo
{
public:
	enum Team
	{
		TEAM_RED,
		TEAM_BLUE
	};
	enum GameState
	{
		START,
		PLANING,
		SHOOTING,
		SWEEPING,
		SCORING,
		END
	};

	Game();

	void UpdateScene(float dt);

	//State Machine
	void ChangeState(GameState newState);
	bool ValidStateChange(GameState newState);

	/*Rock * ThrowRock();*/

	void SwitchTeams();
	int GetTeamRocks(Team team);
	
	void UpdateTeamRocks(Team team, int change);
	void UpdateTeamScore(Team team, int change);

	void Mouse(int button, int state, int x, int y);
	void MouseShooting(int button, int state, int x, int y);

	void Motion(int x, int y);
	void MotionShooting(int x, int y);

private:
	GameState m_curGameState;
	House m_end;
	GameObject * mptr_activeRock;
	Team m_activeTeam;
	int m_scoreBlue, m_scoreRed;
	int m_rocksBlue, m_rocksRed;

	float m_startTimer;
	float m_scoringTimer;
	float m_planingTimer;
};

#endif