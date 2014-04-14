#ifndef GAME_H
#define GAME_H

#include <ctime>
#include <cassert>

#include "PhysicsDemo.h"
#include "House.h"
#include "Rock.h"

#define ROCK_MOVE_THREASHOLD 0.001
#define START_TIMER 5
#define SCORING_TIMER 5
#define PLANING_TIMER 10
#define SHOOTING_TIMER_FULL 5
#define SHOOTING_VELOCITY_FULL 100

class Game : public PhysicsDemo
{
public:
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

	Rock * ThrowRock(btVector3 &initialPosition, const btVector3 &direction, float Rotation, float Velocity);

	void SwitchTeams();
	int GetTeamRocks(Object::Team team);
	
	void UpdateTeamRocks(Object::Team team, int change);
	void UpdateTeamScore(Object::Team team, int change);

	void Mouse(int button, int state, int x, int y);
	void MouseShooting(int button, int state, int x, int y);

	void Motion(int x, int y);
	void MotionShooting(int x, int y);

	void Game::LazyHud() const;

private:
	GameState m_curGameState;
	House m_end;
	GameObject * mptr_activeRock;
	Object::Team m_activeTeam;
	int m_scoreBlue, m_scoreRed;
	int m_rocksBlue, m_rocksRed;

	float m_startTimer;
	float m_scoringTimer;
	float m_planingTimer;
};

#endif