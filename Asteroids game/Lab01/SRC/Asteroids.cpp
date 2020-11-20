#include "Asteroids.h"
#include "Animation.h"
#include "AnimationManager.h"
#include "GameUtil.h"
#include "GameWindow.h"
#include "GameWorld.h"
#include "GameDisplay.h"
#include "Spaceship.h"
#include "BoundingShape.h"
#include "Asteroid.h"
#include "BoundingSphere.h"
#include "GUILabel.h"
#include "Explosion.h"
#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <list>
#include <algorithm>
#include <iterator>
#include <vector>
#include "GlutSession.h"
#include <math.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <functional>


// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/** Constructor. Takes arguments from command line, just in case. */
Asteroids::Asteroids(int argc, char *argv[])
	: GameSession(argc, argv)
{
	mLevel = 0;
	mAsteroidCount = 0;
	mScore = 0;
	mLivesLeft = 0;
	flag = false;
	list<shared_ptr<GameObject>>asteroids;
	name = "";
	gettingUserName = false;
}
/** Destructor. */
Asteroids::~Asteroids(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////


void Asteroids::timer_start(std::function<shared_ptr<Spaceship>(void)> func, unsigned int interval)
{
	std::thread([func, interval]()
	{
		while (true)
		{
			shared_ptr<Spaceship> spaceship = func();
			if (spaceship->AI_MODE) {
				auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(interval);
				shared_ptr<Spaceship> spaceship = func();
				std::this_thread::sleep_until(x);
				
			}
			else {
				break;
			}
			
		}
	}).detach();
}

shared_ptr<Spaceship> AIMode(shared_ptr<Spaceship> mSpaceship) {

	mSpaceship->Shoot();
	return mSpaceship;
	


	//f.open("output.txt");
	//f << "before loop\n";
	//while (flag == false) {
		//f << "in the loop\n";
	/*	vector<float> position;
		map<float, shared_ptr<GameObject>> PositionAsteroid;
		int i = 2;
		for (auto i = asteroids.begin(); i != asteroids.end(); i++) {
			double x2 = pow((*i)->GetPosition().x - mSpaceship->GetPosition().x, 2);
			double y2 = pow((*i)->GetPosition().y - mSpaceship->GetPosition().y, 2);
			double distance = sqrt(x2 + y2);
			position.push_back(distance);
			PositionAsteroid.insert({ distance, *i });
		}
		sort(position.begin(), position.end());
		while (i > 0) {

			auto it = PositionAsteroid.find(position[0]);
			shared_ptr<GameObject> closestAsteroid = it->second;
			GLfloat spacePosX = mSpaceship->GetPosition().x;
			GLfloat spacePosY = mSpaceship->GetPosition().y;
			GLfloat astPosX = closestAsteroid->GetPosition().y;
			GLfloat astPosY = closestAsteroid->GetPosition().y;
			GLfloat spaceshipAngle = mSpaceship->GetAngle();
			GLfloat diffY = abs(astPosY - spacePosY);
			GLfloat diffX = abs(astPosX - spacePosX);
			double degrees = atan(diffY / diffX) * 180 / M_PI;
			mSpaceship->SetAngle(degrees);

			mSpaceship->Shoot();
			mSpaceship->Shoot();
			mSpaceship->Shoot();
			PositionAsteroid.erase(position[0]);
			position.erase(position.begin());
			i--;
		}*/


		//positions i a vector,also map to get the asteroid by position
		// sorting vector to detect closest

		// scan postition of all astrs
		// detect the closest one
		// calculate the angle towards that
		// rotate by that angle
		// shoot
	//}
	//f.close();
}

/** Start an asteroids game. */
void Asteroids::Start()
{
	// Create a shared pointer for the Asteroids game object - DO NOT REMOVE
	shared_ptr<Asteroids> thisPtr = shared_ptr<Asteroids>(this);
	// Add this class as a listener of the game world
	mGameWorld->AddListener(thisPtr.get());
	// Add a score keeper to the game world
	mGameWorld->AddListener(&mScoreKeeper);
	// Add this class as a listener of the score keeper
	mScoreKeeper.AddListener(thisPtr);
	// Add this as a listener to the world and the keyboard
	mGameWindow->AddKeyboardListener(thisPtr);
	CreateGUI();
	// Add a player (watcher) to the game world
	mGameWorld->AddListener(&mPlayer);
	// Add this class as a listener of the player
	mPlayer.AddListener(thisPtr);
	// Create an ambient light to show sprite textures
	GLfloat ambient_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat diffuse_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glEnable(GL_LIGHT0);
	Animation *explosion_anim
		= AnimationManager::GetInstance().CreateAnimationFromFile("explosion", 64, 1024,
			64, 64, "explosion_fs.png");
	Animation *asteroid1_anim = AnimationManager::GetInstance().CreateAnimationFromFile("asteroid1",
		128, 8192, 128, 128, "asteroid1_fs.png");
	Animation *spaceship_anim = AnimationManager::GetInstance().CreateAnimationFromFile("spaceship",
		128, 128, 128, 128, "spaceship_fs.png");
	// Create a spaceship and add it to the world
	//mGameWorld->AddObject(CreateSpaceship());
	// Create some asteroids and add them to the world
	CreateAsteroids(10);
	//shared_ptr<Spaceship> demoSpaceship = CreateSpaceship();
	//demoSpaceship->AIMode();
	mGameWorld -> AddObject(CreateSpaceship());
	//AIMode();
	// Start the game
	mSpaceship->Rotate(300);
	timer_start(std::bind(AIMode,mSpaceship), 100);
	GameSession::Start();
	
	
	

}
/** Stop the current game. */
void Asteroids::Stop()
{
	// Stop the game
	GameSession::Stop();
}
// PUBLIC INSTANCE METHODS IMPLEMENTING IKeyboardListener /////////////////////
void Asteroids::OnKeyPressed(uchar key, int x, int y)
{
	if (!gettingUserName) {
		switch (key)
		{

		case ' ':
			mSpaceship->Shoot();
			break;
		case 'a':
			//mGameWorld->FlagForRemoval(mSpaceship);
			mStartGameLabel->SetVisible(false);
			mSpaceship->AI_MODE = false;
			mSpaceship->Rotate(0);
			//mGameWorld->AddObject(CreateSpaceship());
			mSpaceship->Reset();
			mScoreKeeper = ScoreKeeper();
			CreateAsteroids(10);
			break;
		case 'd':
			break;

		}
	}
	else {
		if (key == '\r') {
			gettingUserName = false;
			for (auto &lable : UserNameLabels) {
				lable->SetVisible(false);
			}
			mEnterNameLabel->SetVisible(false);
			handleHighScore();
		}
		else {
			name += key;
			displayLetter(key);
		}
		
	}
}
void Asteroids::displayLetter(uchar letter) {
	// Create a new GUILabel and wrap it up in a shared_ptr
	mLetterLabel = shared_ptr<GUILabel>(new GUILabel(string (1,static_cast<char>(letter))));
	// Set the horizontal alignment of the label to GUI_HALIGN_CENTER
	mLetterLabel->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	// Set the vertical alignment of the label to GUI_VALIGN_MIDDLE
	mLetterLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_MIDDLE);
	// Set the visibility of the label to false (hidden)
	mLetterLabel->SetVisible(true);
	// Add the GUILabel to the GUIContainer
	shared_ptr<GUIComponent> Letter_component
		= static_pointer_cast<GUIComponent>(mLetterLabel);
	mGameDisplay->GetContainer()->AddComponent(Letter_component, GLVector2f(lableOffset, 0.4f));
	UserNameLabels.push_back(mLetterLabel);
	lableOffset += 0.04;
}
void Asteroids::OnKeyReleased(uchar key, int x, int y) 
{
}
void Asteroids::OnSpecialKeyPressed(int key, int x, int y)
{
	switch (key)
	{
	// If up arrow key is pressed start applying forward thrust
	case GLUT_KEY_UP: mSpaceship->Thrust(10); break;
	// If left arrow key is pressed start rotating anti-clockwise
	case GLUT_KEY_LEFT: mSpaceship->Rotate(90); break;
	// If right arrow key is pressed start rotating clockwise
	case GLUT_KEY_RIGHT: mSpaceship->Rotate(-90); break;
	// Default case - do nothing
	default: break;
	}
}
void Asteroids::OnSpecialKeyReleased(int key, int x, int y)
{
	switch (key)
	{
	// If up arrow key is released stop applying forward thrust
	case GLUT_KEY_UP: mSpaceship->Thrust(0); break;
	// If left arrow key is released stop rotating
	case GLUT_KEY_LEFT: mSpaceship->Rotate(0); break;
	// If right arrow key is released stop rotating
	case GLUT_KEY_RIGHT: mSpaceship->Rotate(0); break;
	// Default case - do nothing
	default: break;
	} 
}
void Asteroids::OnScoreChanged(int score)
{
	// Format the score message using an string-based stream
	std::ostringstream msg_stream;
	msg_stream << "Score: " << score;
	mScore = score;
	// Get the score message as a string
	std::string score_msg = msg_stream.str();
	mScoreLabel->SetText(score_msg);
}
void Asteroids::CreateAsteroids(const uint num_asteroids)
{
	mAsteroidCount = num_asteroids;
	for (uint i = 0; i < num_asteroids; i++)
	{
		Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("asteroid1");
		shared_ptr<Sprite> asteroid_sprite
			= make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
		asteroid_sprite->SetLoopAnimation(true);
		shared_ptr<GameObject> asteroid = make_shared<Asteroid>();
		asteroids.push_back(asteroid);
		asteroid->SetBoundingShape(make_shared<BoundingSphere>(asteroid->GetThisPtr(), 10.0f));
		asteroid->SetSprite(asteroid_sprite);
		asteroid->SetScale(0.2f);
		mGameWorld->AddObject(asteroid);
	}
}
void Asteroids::DeleteAsteroid() {
	for (auto i = asteroids.begin(); i != asteroids.end(); i++) {
		mGameWorld->FlagForRemoval(*i);
	}
}
void Asteroids::CreateGUI()
{
	// Add a (transparent) border around the edge of the game display
	mGameDisplay->GetContainer()->SetBorder(GLVector2i(10, 10));
	// Create a new GUILabel and wrap it up in a shared_ptr
	mScoreLabel = make_shared<GUILabel>("Score: 0");
	// Set the vertical alignment of the label to GUI_VALIGN_TOP
	mScoreLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_TOP);
	// Add the GUILabel to the GUIComponent
	shared_ptr<GUIComponent> score_component
		= static_pointer_cast<GUIComponent>(mScoreLabel);
	mGameDisplay->GetContainer()->AddComponent(score_component, GLVector2f(0.0f, 1.0f));

	// Create a new GUILabel and wrap it up in a shared_ptr
	mLivesLabel = make_shared<GUILabel>("Lives: 3");
	// Set the vertical alignment of the label to GUI_VALIGN_BOTTOM
	mLivesLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_BOTTOM);
	// Add the GUILabel to the GUIComponent
	shared_ptr<GUIComponent> lives_component = static_pointer_cast<GUIComponent>(mLivesLabel);
	mGameDisplay->GetContainer()->AddComponent(lives_component, GLVector2f(0.0f, 0.0f));

	// Create a new GUILabel and wrap it up in a shared_ptr
	mGameOverLabel = shared_ptr<GUILabel>(new GUILabel("GAME OVER"));
	// Set the horizontal alignment of the label to GUI_HALIGN_CENTER
	mGameOverLabel->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	// Set the vertical alignment of the label to GUI_VALIGN_MIDDLE
	mGameOverLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_MIDDLE);
	// Set the visibility of the label to false (hidden)
	mGameOverLabel->SetVisible(false);
	// Add the GUILabel to the GUIContainer
	shared_ptr<GUIComponent> game_over_component
		= static_pointer_cast<GUIComponent>(mGameOverLabel);
	mGameDisplay->GetContainer()->AddComponent(game_over_component, GLVector2f(0.5f, 0.6f));

	// Create a new GUILabel and wrap it up in a shared_ptr for start 
	mStartGameLabel = shared_ptr<GUILabel>(new GUILabel("Press A to start"));
	// Set the horizontal alignment of the label to GUI_HALIGN_CENTER
	mStartGameLabel->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	// Set the vertical alignment of the label to GUI_VALIGN_MIDDLE
	mStartGameLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_MIDDLE);
	// Set the visibility of the label to false (hidden)
	mStartGameLabel->SetVisible(true);
	// Add the GUILabel to the GUIContainer
	shared_ptr<GUIComponent> start_game_component
		= static_pointer_cast<GUIComponent>(mStartGameLabel);
	mGameDisplay->GetContainer()->AddComponent(start_game_component, GLVector2f(0.5f, 0.5f));
}
void Asteroids::handleHighScore() {
	ofstream f;
	f.open("output.txt");
	f << "enter detected\n";
	f.close();
	WriteToFile("score.txt");
	ReadFromFile("score.txt");
	CreateScoresLabel();
	DeleteAsteroid();
	mLivesLabel->SetVisible(false);
	mScoreLabel->SetVisible(false);
	SetTimer(2000, CLEAN_GAME_OVER);
}
void Asteroids::OnPlayerKilled(int lives_left)
{
	mLivesLeft = lives_left;
	shared_ptr<GameObject> explosion = CreateExplosion();
	explosion->SetPosition(mSpaceship->GetPosition());
	explosion->SetRotation(mSpaceship->GetRotation());
	mGameWorld->AddObject(explosion);
	// Format the lives left message using an string-based stream
	std::ostringstream msg_stream;
	msg_stream << "Lives: " << lives_left;
	// Get the lives left message as a string
	std::string lives_msg = msg_stream.str();
	mLivesLabel->SetText(lives_msg);
	if (lives_left > 0) { SetTimer(1000, CREATE_NEW_PLAYER); }
	if (lives_left == 0) { 
		SetTimer(500, SHOW_GAME_OVER);
		gettingUserName = true;
		CreateNameLabel();
	}
}
void Asteroids::CreateScoresLabel() {
	float yOffset = 1.0f;
	int numHighScores = scorelist.size() < 10 ? scorelist.size() : 10;
	for (auto it = scorelist.rbegin(); it != scorelist.rend(); ++it) {
		string lableStr = to_string(it->first) + " " + it->second;
		highScoreLabels.push_back(shared_ptr<GUILabel>(new GUILabel(lableStr)));
		highScoreLabels.back()->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
		highScoreLabels.back()->SetVerticalAlignment(GUIComponent::GUI_VALIGN_MIDDLE);
		highScoreLabels.back()->SetVisible(true);
		shared_ptr<GUIComponent> place_component
			= static_pointer_cast<GUIComponent>(highScoreLabels.back());
		mGameDisplay->GetContainer()->AddComponent(place_component, GLVector2f(0.5f, yOffset));
		yOffset -= 0.1f;
	}
	
 }
void Asteroids::CreateNameLabel() {
	mEnterNameLabel = shared_ptr<GUILabel>(new GUILabel("Enter your name:"));
	mEnterNameLabel->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	mEnterNameLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_MIDDLE);
	mEnterNameLabel->SetVisible(true);
	shared_ptr<GUIComponent> enter_name_component
		= static_pointer_cast<GUIComponent>(mEnterNameLabel);
	mGameDisplay->GetContainer()->AddComponent(enter_name_component, GLVector2f(0.5f, 0.5f));
	/*float xOffset = 0.5f;
	for (int i = 0; i < 5; i++) {
		EnterNameLabels.push_back(shared_ptr<GUILabel>(new GUILabel(name[i]+" ")));
		EnterNameLabels[i]->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
		EnterNameLabels[i]->SetVerticalAlignment(GUIComponent::GUI_VALIGN_MIDDLE);
		EnterNameLabels[i]->SetVisible(true);
		shared_ptr<GUIComponent> name_component
			= static_pointer_cast<GUIComponent>(EnterNameLabels[i]);
		mGameDisplay->GetContainer()->AddComponent(name_component, GLVector2f(xOffset, 0.4f));
		xOffset += 0.02f;
	}*/
}





// PUBLIC INSTANCE METHODS IMPLEMENTING IGameWorldListener ////////////////////

void Asteroids::OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object)
{
	if (object->GetType() == GameObjectType("Asteroid"))
	{
		shared_ptr<GameObject> explosion = CreateExplosion();
		explosion->SetPosition(object->GetPosition());
		explosion->SetRotation(object->GetRotation());
		mGameWorld->AddObject(explosion);
		mAsteroidCount--;
		if (mAsteroidCount <= 0 && mLivesLeft>0)
		{
			SetTimer(500, START_NEXT_LEVEL);
		}
	}
}

// PUBLIC INSTANCE METHODS IMPLEMENTING ITimerListener ////////////////////////

void Asteroids::OnTimer(int value)
{
	if (value == CREATE_NEW_PLAYER)
	{
		mSpaceship->Reset();
		mGameWorld->AddObject(mSpaceship);
	}

	if (value == START_NEXT_LEVEL)
	{
		mLevel++;
		int num_asteroids = 10 + 2 * mLevel;
		CreateAsteroids(num_asteroids);
	}
	if (value == SHOW_GAME_OVER)
	{
		mGameOverLabel->SetVisible(true);
	}
	if (value == CLEAN_GAME_OVER)
	{
		mGameOverLabel->SetVisible(false);
	}
	if (value == AI_MODE)
	{
		mSpaceship->Shoot();
	}
}
void Asteroids::WriteToFile(string filename)
{
	// Create and open a text file
	ofstream MyFile(filename, ios_base::app);
	// Write to the file
	MyFile << mScore << " " << name << "\n";
	// Close the file
	MyFile.close();
}
void Asteroids::ReadFromFile(string filename)
{
	// Create a text string, which is used to output the text file
	string line;
	// Read from the text file
	ifstream MyReadFile(filename);
	// Use a while loop together with the getline() function to read the file line by line
	int score;
	string name = "";
	string token = "";
	while (getline(MyReadFile, line)) {
		// Output the text from the file
		int i = 0;
		while (line[i] != ' ') {
			token += line[i];
			i++;
		}
		score = stoi(token);
		name = line.substr(i + 1);

		scorelist.insert(pair <int,string> (score, name));
		name = "";
		token = "";
	}
	//sort(scorelist.begin(), scorelist.end());
	//reverse(scorelist.begin(), scorelist.end());
	// Close the file
	MyReadFile.close();
}


// PROTECTED INSTANCE METHODS /////////////////////////////////////////////////
shared_ptr<GameObject> Asteroids::CreateSpaceship()
{
	// Create a raw pointer to a spaceship that can be converted to
	// shared_ptrs of different types because GameWorld implements IRefCount
	mSpaceship = make_shared<Spaceship>();
	mSpaceship->SetBoundingShape(make_shared<BoundingSphere>(mSpaceship->GetThisPtr(), 4.0f));
	shared_ptr<Shape> bullet_shape = make_shared<Shape>("bullet.shape");
	mSpaceship->SetBulletShape(bullet_shape);
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("spaceship");
	shared_ptr<Sprite> spaceship_sprite =
		make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	mSpaceship->SetSprite(spaceship_sprite);
	mSpaceship->SetScale(0.1f);
	// Reset spaceship back to centre of the world
	mSpaceship->Reset();
	// Return the spaceship so it can be added to the world
	return mSpaceship;
}
shared_ptr<GameObject> Asteroids::CreateExplosion()
{
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("explosion");
	shared_ptr<Sprite> explosion_sprite =
		make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	explosion_sprite->SetLoopAnimation(false);
	shared_ptr<GameObject> explosion = make_shared<Explosion>();
	explosion->SetSprite(explosion_sprite);
	explosion->Reset();
	return explosion;
}
