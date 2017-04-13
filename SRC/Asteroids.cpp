#include "Asteroid.h"
#include "Asteroids.h"
#include "Animation.h"
#include "AnimationManager.h"
#include "GameUtil.h"
#include "GameWindow.h"
#include "GameWorld.h"
#include "GameDisplay.h"
#include "Spaceship.h"
#include "BoundingShape.h"
#include "BoundingSphere.h"
#include "GUILabel.h"
#include "Explosion.h"
#include <map>
#include <iostream>

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/** Constructor. Takes arguments from command line, just in case. */
Asteroids::Asteroids(int argc, char *argv[])
	: GameSession(argc, argv)
{
	mLevel = 0;
	mAsteroidCount = 0;
}

/** Destructor. */
Asteroids::~Asteroids(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Start an asteroids game. */
void Asteroids::Start()
{
	// Create a shared pointer for the Asteroids game object - DO NOT REMOVE
	shared_ptr<Asteroids> thisPtr = shared_ptr<Asteroids>(this);

	// Add this class as a listener of the game world
	mGameWorld->AddListener(thisPtr.get());

	// Add this as a listener to the world and the keyboard
	mGameWindow->AddKeyboardListener(thisPtr);

	// Add a score keeper to the game world
	mGameWorld->AddListener(&mScoreKeeper);

	// Add this class as a listener of the score keeper
	mScoreKeeper.AddListener(thisPtr);

	// Add a player (watcher) to the game world
	mGameWorld->AddListener(&mPlayer);

	// Add this class as a listener of the player
	mPlayer.AddListener(thisPtr);

	// Create lighting for animation effects
	CreateLighting();

	// Create the animations
	CreateAnimation();

	//////    /* Initial Start Display */ ///////////////

	// Add a (transparent) border around the edge of the game display
	mGameDisplay->GetContainer()->SetBorder(GLVector2i(10, 10));

	// Create a new GUILabel and wrap it up in a shared_ptr
	mTitleLabel = make_shared<GUILabel>("Asteroids");
	// Set the vertical alignment of the label to GUI_VALIGN_MIDDLE
	mTitleLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_MIDDLE);
	// Set the horizontal alignment of the label to GUI_HALIGN_CENTER
	mTitleLabel->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> title_component
		= static_pointer_cast<GUIComponent>(mTitleLabel);
	mGameDisplay->GetContainer()->AddComponent(title_component, GLVector2f(0.5f, 0.6f));

	// Create a new GUILabel and wrap it up in a shared_ptr
	mEnterNameLabel = make_shared<GUILabel>("Enter name: ");
	// Set the vertical alignment of the label to GUI_VALIGN_MIDDLE
	mEnterNameLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_MIDDLE);
	// Set the horizontal alignment of the label to GUI_HALIGN_CENTER
	mEnterNameLabel->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> enter_name_component
		= static_pointer_cast<GUIComponent>(mEnterNameLabel);
	mGameDisplay->GetContainer()->AddComponent(enter_name_component, GLVector2f(0.5f, 0.4f));

	// Create a new GUILabel and wrap it up in a shared_ptr
	mStartMessageLabel = make_shared<GUILabel>("press s to start");
	// Set the vertical alignment of the label to GUI_VALIGN_MIDDLE
	mStartMessageLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_MIDDLE);
	// Set the horizontal alignment of the label to GUI_HALIGN_CENTER
	mStartMessageLabel->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	// Don't show start label until player has entered their name
	mStartMessageLabel->SetVisible(false);
	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> start_message_component
		= static_pointer_cast<GUIComponent>(mStartMessageLabel);
	mGameDisplay->GetContainer()->AddComponent(start_message_component, GLVector2f(0.5f, 0.4f));

	// Create a new GUILabel and wrap it up in a shared_ptr
	mShowHighScoresLabel = make_shared<GUILabel>("press h to show high score table");
	// Set the vertical alignment of the label to GUI_VALIGN_MIDDLE
	mShowHighScoresLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_MIDDLE);
	// Set the horizontal alignment of the label to GUI_HALIGN_CENTER
	mShowHighScoresLabel->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	// Don't show start label until player has entered their name
	mShowHighScoresLabel->SetVisible(false);
	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> show_high_scores_component
		= static_pointer_cast<GUIComponent>(mShowHighScoresLabel);
	mGameDisplay->GetContainer()->AddComponent(show_high_scores_component, GLVector2f(0.5f, 0.3f));

	// Start the game
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
	if (nameEntered == false) {
		if (key != 13 && key != '\b') {
			playerName += key;
			// Format the score message using an string-based stream
			std::ostringstream msg_stream;
			msg_stream << "Enter name: " << playerName;
			// Get the score message as a string
			std::string name_msg = msg_stream.str();
			mEnterNameLabel->SetText(name_msg);
		}
		// If backspace key pressed then delete the last character
		else if (key == '\b' && playerName.size() > 0) {
			playerName.erase(playerName.size() - 1);
			// Format the score message using an string-based stream
			std::ostringstream msg_stream;
			msg_stream << "Enter name: " << playerName;
			// Get the score message as a string
			std::string name_msg = msg_stream.str();
			mEnterNameLabel->SetText(name_msg);
		}
		else if(playerName.size() > 0) {
			mEnterNameLabel->SetVisible(false);
			mStartMessageLabel->SetVisible(true);
			mShowHighScoresLabel->SetVisible(true);
			nameEntered = true;
		}
	}
	else {
		if (gameStarted == false) {
			if (key == 's') {
				// Hide initial start messages and title 
				mTitleLabel->SetVisible(false);
				mStartMessageLabel->SetVisible(false);
				mShowHighScoresLabel->SetVisible(false);

				// Create a spaceship and add it to the world
				mGameWorld->AddObject(CreateSpaceship());

				// Create some asteroids and add them to the world
				CreateAsteroids(10);

				// Create the GUI
				CreateGUI();

				// Indicate that the game has started
				SetGameStarted(true);
			}
			else if (key == 'h') {
				// Hide all the start screen GUI labels
				mTitleLabel->SetVisible(false);
				mStartMessageLabel->SetVisible(false);
				mShowHighScoresLabel->SetVisible(false);
				// Create the high score GUI labels
				CreateHighScoreTableGUI();
				// Read from the file that contains all the player name and scores
				ifstream highScoreTable("HighScoreTable.txt");
				if (highScoreTable.is_open()) {
					map<string, int> scoretable;
					string player;
					string score;
					// Obtain the player name and scores from the file and put them into the scoretable map
					while (!highScoreTable.eof()) {
						getline(highScoreTable, player);
						if (player != "") {
							getline(highScoreTable, score);
							if (scoretable.count(player) == 0) {
								scoretable[player] = stoi(score);
							}
							else {
								if (stoi(score) > scoretable[player]) {
									scoretable[player] = stoi(score);
								}
							}
						}
					}
					if (!scoretable.empty()) {
						map<string, int>::iterator iter;
						int previousValue = 0;
						int highScore = 0;
						string name = "";
						map<string, int> highScoreTable;
						// Obtain the five highest scores and place them and the player's name into the highscoretable map
						if (scoretable.size() >= 5) {
							while (highScoreTable.size() < 5) {
								for (iter = scoretable.begin(); iter != scoretable.end(); iter++) {
									if (iter->second >= highScore && previousValue == 0) {
										highScore = iter->second;
										name = iter->first;
									}
									else if ((iter->second >= highScore) && (previousValue > 0) && (iter->second < previousValue)) {
										highScore = iter->second;
										name = iter->first;
									}
								}
								highScoreTable[name] = highScore;
								previousValue = highScore;
								highScore = 0;
							}
						}
						// If there isn't 5 high scores then just get the number of scores that there is
						else {
							while (highScoreTable.size() < scoretable.size()) {
								for (iter = scoretable.begin(); iter != scoretable.end(); iter++) {
									if (iter->second >= highScore && previousValue == 0) {
										highScore = iter->second;
										highScoreTable[iter->first] = iter->second;
									}
									else if ((iter->second >= highScore) && (previousValue > 0) && (iter->second < previousValue)) {
										highScore = iter->second;
										highScoreTable[iter->first] = iter->second;
									}
								}
								previousValue = highScore;
								highScore = 0;
							}
						}
						float gap = 0;
						int scoreNumber = 1;
						string name2 = "";
						int highestScore = 0;
						int previousHighScore = 0;
						int count = 0;
						// Obtain the high scores in the correct order
						while (scoreNumber < highScoreTable.size() + 1) {
							for (iter = highScoreTable.begin(); iter != highScoreTable.end(); iter++) {
								if (previousHighScore == 0) {
									if (iter->second >= highestScore) {
										highestScore = iter->second;
										name2 = iter->first;
									}
								}
								else {
									if (iter->second >= highestScore && iter->second < previousHighScore) {
										highestScore = iter->second;
										name2 = iter->first;
									}
								}
							}
							previousHighScore = highestScore;
							highestScore = 0;

							// Create a label for the high score obtained and player's name

							mHighScore1Label = make_shared<GUILabel>(to_string(scoreNumber) + ". " + name2 + " " + to_string(highScoreTable[name2]));
							// Set the vertical alignment of the label to GUI_VALIGN_TOP
							mHighScore1Label->SetVerticalAlignment(GUIComponent::GUI_VALIGN_TOP);
							// Set the vertical alignment of the label to GUI_HALIGN_CENTER
							mHighScore1Label->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
							// Add the GUILabel to the GUIComponent  
							shared_ptr<GUIComponent> high_score_component
								= static_pointer_cast<GUIComponent>(mHighScore1Label);
							mGameDisplay->GetContainer()->AddComponent(high_score_component, GLVector2f(0.5f, (0.6f - gap)));
							gap += 0.1;
							scoreNumber++;
						}
					}
					else {
						mNoHighScoreLabel->SetVisible(true);
					}
				}
				highScoreTable.close();
				// Create GUI for game but hide it for AI in the background of high score table
				CreateGUI();
				mScoreLabel->SetVisible(false);
				mLivesLabel->SetVisible(false);
				// Create an AI spaceship and add it to the world
				mGameWorld->AddObject(CreateAISpaceship());
                // Create some asteroids and add them to the world
				CreateAsteroids(10);
				// Activate AI
				mAISpaceship->Activate();
				AIActive = true;
			}
		}
		else {
			switch (key)
			{
			case ' ':
				mSpaceship->Shoot();
				break;
			default:
				break;
			}
		}
	}
}

void Asteroids::OnKeyReleased(uchar key, int x, int y) {}

void Asteroids::OnSpecialKeyPressed(int key, int x, int y)
{
	if (gameStarted == true) {
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
}

void Asteroids::OnSpecialKeyReleased(int key, int x, int y)
{
	if (gameStarted == true) {
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
		if (mAsteroidCount <= 0) 
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

	if (value == CREATE_NEW_AI)
	{
		mAISpaceship->Reset();
		mGameWorld->AddObject(mAISpaceship);
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
		ofstream highScoreTable("HighScoreTable.txt", ios::app);
		if (highScoreTable.is_open()) {
			highScoreTable << playerName << endl;
			highScoreTable << to_string(mScoreKeeper.getScore()) << endl;
			//cout << playerName << endl;
			//cout << mScoreKeeper.getScore() << endl;
			highScoreTable.close();
		}
	}

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

shared_ptr<GameObject> Asteroids::CreateAISpaceship()
{
	// Create a raw pointer to a AI spaceship that can be converted to
	// shared_ptrs of different types because GameWorld implements IRefCount
	mAISpaceship = make_shared<AISpaceship>();
	mAISpaceship->SetBoundingShape(make_shared<BoundingSphere>(mAISpaceship->GetThisPtr(), 4.0f));
	shared_ptr<Shape> bullet_shape = make_shared<Shape>("bullet.shape");
	mAISpaceship->SetBulletShape(bullet_shape);
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("spaceship");
	shared_ptr<Sprite> spaceship_sprite =
		make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	mAISpaceship->SetSprite(spaceship_sprite);
	mAISpaceship->SetScale(0.1f);
	// Reset spaceship back to centre of the world
	mAISpaceship->Reset();
	// Return the spaceship so it can be added to the world
	return mAISpaceship;
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
		asteroid->SetBoundingShape(make_shared<BoundingSphere>(asteroid->GetThisPtr(), 10.0f));
		asteroid->SetSprite(asteroid_sprite);
		asteroid->SetScale(0.2f);
		mGameWorld->AddObject(asteroid);
	}
}

void Asteroids::CreateGUI()
{
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
	mGameDisplay->GetContainer()->AddComponent(game_over_component, GLVector2f(0.5f, 0.5f));

}

void Asteroids::CreateHighScoreTableGUI()
{
	// Create a new GUILabel and wrap it up in a shared_ptr
	mHighScoreTitleLabel = make_shared<GUILabel>("High Score Table");
	// Set the vertical alignment of the label to GUI_VALIGN_TOP
	mHighScoreTitleLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_TOP);
	// Set the horizontal alignment of the label to GUI_HALIGN_CENTER
	mHighScoreTitleLabel->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> high_score_table_component
		= static_pointer_cast<GUIComponent>(mHighScoreTitleLabel);
	mGameDisplay->GetContainer()->AddComponent(high_score_table_component, GLVector2f(0.5f, 0.8f));

	// Create a new GUILabel and wrap it up in a shared_ptr
	mPlayerNameLabel = make_shared<GUILabel>("Name");
	// Set the vertical alignment of the label to GUI_VALIGN_TOP
	mPlayerNameLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_TOP);
	// Set the horizontal alignment of the label to GUI_HALIGN_CENTER
	mPlayerNameLabel->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> player_name_component
		= static_pointer_cast<GUIComponent>(mPlayerNameLabel);
	mGameDisplay->GetContainer()->AddComponent(player_name_component, GLVector2f(0.485f, 0.7f));

	// Create a new GUILabel and wrap it up in a shared_ptr
	mPlayerHighScoreLabel = make_shared<GUILabel>("Score");
	// Set the vertical alignment of the label to GUI_VALIGN_TOP
	mPlayerHighScoreLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_TOP);
	// Set the horizontal alignment of the label to GUI_HALIGN_CENTER
	mPlayerHighScoreLabel->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> player_score_component
		= static_pointer_cast<GUIComponent>(mPlayerHighScoreLabel);
	mGameDisplay->GetContainer()->AddComponent(player_score_component, GLVector2f(0.615f, 0.7f));

	// Create a new GUILabel and wrap it up in a shared_ptr
	mNoHighScoreLabel = make_shared<GUILabel>("No scores recorded yet");
	// Set the vertical alignment of the label to GUI_VALIGN_TOP
	mNoHighScoreLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_MIDDLE);
	// Set the horizontal alignment of the label to GUI_HALIGN_CENTER
	mNoHighScoreLabel->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	// Set the visibility of the label to false (hidden)
	mNoHighScoreLabel->SetVisible(false);
	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> no_high_scores_component
		= static_pointer_cast<GUIComponent>(mNoHighScoreLabel);
	mGameDisplay->GetContainer()->AddComponent(no_high_scores_component, GLVector2f(0.5f, 0.5f));
}

void Asteroids::OnScoreChanged(int score)
{
	// Format the score message using an string-based stream
	std::ostringstream msg_stream;
	msg_stream << "Score: " << score;
	// Get the score message as a string
	std::string score_msg = msg_stream.str();
	mScoreLabel->SetText(score_msg);
}

void Asteroids::OnPlayerKilled(int lives_left)
{
	if (AIActive == false) {
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

		if (lives_left > 0)
		{
			SetTimer(1000, CREATE_NEW_PLAYER);
		}
		else
		{
			SetTimer(500, SHOW_GAME_OVER);
		}
	}
	else
	{
		shared_ptr<GameObject> explosion = CreateExplosion();
		explosion->SetPosition(mAISpaceship->GetPosition());
		explosion->SetRotation(mAISpaceship->GetRotation());
		mGameWorld->AddObject(explosion);

		SetTimer(1000, CREATE_NEW_AI);
	}
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

void Asteroids::CreateLighting() {
	// Create an ambient light to show sprite textures
	GLfloat ambient_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat diffuse_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glEnable(GL_LIGHT0);
}

void Asteroids::CreateAnimation() {
	Animation *explosion_anim = AnimationManager::GetInstance().CreateAnimationFromFile("explosion", 64, 1024, 64, 64, "explosion_fs.png");
	Animation *asteroid1_anim = AnimationManager::GetInstance().CreateAnimationFromFile("asteroid1", 128, 8192, 128, 128, "asteroid1_fs.png");
	Animation *spaceship_anim = AnimationManager::GetInstance().CreateAnimationFromFile("spaceship", 128, 128, 128, 128, "spaceship_fs.png");
}




