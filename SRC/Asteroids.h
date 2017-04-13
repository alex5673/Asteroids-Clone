#ifndef __ASTEROIDS_H__
#define __ASTEROIDS_H__

#include "GameUtil.h"
#include "GameSession.h"
#include "IKeyboardListener.h"
#include "IGameWorldListener.h"
#include "IScoreListener.h" 
#include "ScoreKeeper.h"
#include "Player.h"
#include "IPlayerListener.h"
#include <string>
#include "AISpaceship.h"

class GameObject;
class Spaceship;
class AISpaceship;
class GUILabel;

class Asteroids : public GameSession, public IKeyboardListener, public IGameWorldListener, public IScoreListener, public IPlayerListener
{
public:
	Asteroids(int argc, char *argv[]);
	virtual ~Asteroids(void);

	virtual void Start(void);
	virtual void Stop(void);

	// Declaration of IKeyboardListener interface ////////////////////////////////

	void OnKeyPressed(uchar key, int x, int y);
	void OnKeyReleased(uchar key, int x, int y);
	void OnSpecialKeyPressed(int key, int x, int y);
	void OnSpecialKeyReleased(int key, int x, int y);

	// Declaration of IScoreListener interface //////////////////////////////////

	void OnScoreChanged(int score);

	// Declaration of the IPlayerLister interface //////////////////////////////

	void OnPlayerKilled(int lives_left);

	// Declaration of IGameWorldListener interface //////////////////////////////

	void OnWorldUpdated(GameWorld* world) {}
	void OnObjectAdded(GameWorld* world, shared_ptr<GameObject> object) {}
	void OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object);

	// Override the default implementation of ITimerListener ////////////////////
	void OnTimer(int value);


	void SetGameStarted(bool started) { gameStarted = started; }

private:

	shared_ptr<Asteroids> thisP = shared_ptr<Asteroids>(this);

	shared_ptr<Spaceship> mSpaceship;
	shared_ptr<AISpaceship> mAISpaceship;
	shared_ptr<GUILabel> mScoreLabel;
	shared_ptr<GUILabel> mLivesLabel;
	shared_ptr<GUILabel> mGameOverLabel;
	shared_ptr<GUILabel> mTitleLabel;
	shared_ptr<GUILabel> mStartMessageLabel;
	shared_ptr<GUILabel> mEnterNameLabel;
	shared_ptr<GUILabel> mHighScoreTitleLabel;
	shared_ptr<GUILabel> mShowHighScoresLabel;
	shared_ptr<GUILabel> mNoHighScoreLabel;
	shared_ptr<GUILabel> mPlayerNameLabel;
	shared_ptr<GUILabel> mPlayerHighScoreLabel;
	shared_ptr<GUILabel> mHighScore1Label;

	uint mLevel;
	uint mAsteroidCount;

	bool gameStarted = false;
	bool AIActive = false;
	bool nameEntered = false;
	string playerName = "";

	void ResetSpaceship() { if (AIActive == false) { mSpaceship->Reset(); } else { mAISpaceship->Reset(); } }
	shared_ptr<GameObject> CreateSpaceship();
	shared_ptr<GameObject> CreateAISpaceship();
	void CreateGUI();
	void CreateHighScoreTableGUI();
	void CreateAsteroids(const uint num_asteroids);
	void CreateAnimation();
	void CreateLighting();
	shared_ptr<GameObject> CreateExplosion();
	
	const static uint SHOW_GAME_OVER = 0;
	const static uint START_NEXT_LEVEL = 1;
	const static uint CREATE_NEW_PLAYER = 2;
	const static uint CREATE_NEW_AI = 3;

	ScoreKeeper mScoreKeeper;
	Player mPlayer;
};

#endif