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
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <functional>

class GameObject;
class Spaceship;
class GUILabel;
class GameWindow;
class GameDisplay;

class Asteroids : public GameSession, public IKeyboardListener, public IGameWorldListener, public IScoreListener, public IPlayerListener
{
public:
	Asteroids(int argc, char *argv[]);
	virtual ~Asteroids(void);

	virtual void Start(void);
	virtual void Stop(void);
	//player killed
	virtual void OnPlayerKilled(int lives_left);

	// Declaration of IKeyboardListener interface ////////////////////////////////

	void OnKeyPressed(uchar key, int x, int y);
	void OnKeyReleased(uchar key, int x, int y);
	void OnSpecialKeyPressed(int key, int x, int y);
	void OnSpecialKeyReleased(int key, int x, int y);

	// declare the  OnScoreChanged
	virtual void OnScoreChanged(int score);

	// Declaration of IGameWorldListener interface //////////////////////////////

	void OnWorldUpdated(GameWorld* world) {}
	void OnObjectAdded(GameWorld* world, shared_ptr<GameObject> object) {}
	void OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object);
	//GUI
	void CreateGUI();
	// Override the default implementation of ITimerListener ////////////////////
	void OnTimer(int value);
	void WriteToFile(string filename);
	void ReadFromFile(string filename);
	void CreateScoresLabel();
	void CreateNameLabel();
	void handleHighScore();
	void timer_start(std::function<shared_ptr<Spaceship>(void)> func, unsigned int interval);
	bool AI_MODE_ENABLED(bool status);
	bool AI_MODE;


private:
	void displayLetter(uchar letter);
	shared_ptr<Spaceship> mSpaceship;
	list<shared_ptr<GameObject>>asteroids;
	uint mLevel;
	uint mAsteroidCount;
	uint mScore;
	uint mLivesLeft;
	bool flag, gettingUserName;
	string name;
	map<int, string> scorelist;
	float lableOffset = 0.5f;

	//void ResetSpaceship();
	shared_ptr<GameObject> CreateSpaceship();
	shared_ptr<GameObject> CreateExplosion();
	void CreateAsteroids(const uint num_asteroids);
	void DeleteAsteroid();

	const static uint SHOW_GAME_OVER = 0;
	const static uint CLEAN_GAME_OVER = 3;
	const static uint START_NEXT_LEVEL = 1;
	const static uint CREATE_NEW_PLAYER = 2;

	ScoreKeeper mScoreKeeper;
	Player mPlayer;

	shared_ptr <GUILabel> mScoreLabel;
	shared_ptr <GUILabel> mLivesLabel;
	shared_ptr<GUILabel> mGameOverLabel;
	shared_ptr<GUILabel> mStartGameLabel; 
	shared_ptr<GUILabel> mEnterNameLabel;
	shared_ptr<GUILabel> mLetterLabel;


	vector<shared_ptr <GUILabel>> highScoreLabels;
	vector<shared_ptr <GUILabel>> EnterNameLabels;
	vector<shared_ptr <GUILabel>> UserNameLabels;
protected:
	GameDisplay* mGameDisplayScores;
	GameWindow* mGameWindowScores;
	GameWorld* mGameWorldScores;
};


#endif