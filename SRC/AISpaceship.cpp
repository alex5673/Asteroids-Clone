#include "AISpaceship.h"
#include <cmath>

using namespace std;

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/**  Default constructor. */
AISpaceship::AISpaceship()
	: Spaceship()
{
}

/** Construct a spaceship with given position, velocity, acceleration, angle, and rotation. */
AISpaceship::AISpaceship(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r)
	: Spaceship(p, v, a, h, r)
{
}

/** Copy constructor. */
AISpaceship::AISpaceship(const AISpaceship& s)
	: Spaceship((Spaceship)s)
{
}

/** Destructor. */
AISpaceship::~AISpaceship(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Update this spaceship. */
void AISpaceship::Update(int t)
{
	// Call parent update function
	Spaceship::Update(t);

	static int timePast = 0;
	//cout << timePast << endl;
	timePast += t;
	if ((timePast % 25) == 0) {
		UpdateAI();
	}

	/*
	if (active == true) {
		if (CheckVicinity() == true) {
			DefenseMode();
		}
		else {
			AttackMode();
		}
	}
	*/
}

/** Render this spaceship. */
void AISpaceship::Render(void)
{
	Spaceship::Render();
}

bool AISpaceship::CollisionTest(shared_ptr<GameObject> o)
{
	return Spaceship::CollisionTest(o);
}

void AISpaceship::OnCollision(const GameObjectList &objects)
{
	Spaceship::OnCollision(objects);
}


void AISpaceship::Activate() 
{
	SetActive(true);
	//cout << "AISpaceship is active " << active << endl;
}

void AISpaceship::Deactivate() 
{
	SetActive(false);
}

////////////////// AI Algorithms //////////////////////////////////////////////

bool AISpaceship::CheckVicinity() 
{
	//cout << "Checking vicinity" << endl;
	bool inVicinity = false;
	list<shared_ptr<GameObject>>::iterator iter;
	for (iter = mWorld->GetGameObjects().begin(); iter != mWorld->GetGameObjects().end(); ++iter) {
		if (*iter != nullptr) {
			//cout << *iter << endl;
			if ((*iter)->GetType() == GameObjectType("Asteroid")) {
				//cout << "Radius = " << GetRadius() << endl;
				//cout << "AISpaceship pos x = " << GetPosition().x << endl;
				//cout << "AISpaceship pos y = " << GetPosition().y << endl;
				//cout << "Asteroid pos x = " << (*iter)->GetPosition().x << endl;
				//cout << "Asteroid pos y = " << (*iter)->GetPosition().y << endl;
				//cout << "Distance = " << sqrt(pow(((*iter)->GetPosition().x - GetPosition().x), 2) + pow(((*iter)->GetPosition().y - GetPosition().y), 2)) << endl;
				if (pow(GetRadius(), 2) >= pow(((*iter)->GetPosition().x - GetPosition().x), 2) + pow(((*iter)->GetPosition().y - GetPosition().y), 2)) {
					asteroidsNear.push_back(*iter);
				}
			}
		}
	}
	
	if (!asteroidsNear.empty()) {
		return inVicinity = true;
	}
	//cout << inVicinity << endl;
	return inVicinity;
}

float AISpaceship::GetRadius()
{
	radius = sqrt((pow((GetPosition().x - (GetPosition().x + 500)), 2)) + (pow((GetPosition().y - (GetPosition().y)), 2)));
	return radius;
}

void AISpaceship::DefenseMode()
{
	//cout << "Defense mode" << endl;
	if (!asteroidsNear.empty()) {
		vector<shared_ptr<GameObject>>::iterator iter;
		for (iter = asteroidsNear.begin(); iter != asteroidsNear.end(); iter++) {
			//cout << *iter << endl;
			if (*iter != nullptr) {
				float rotation = cos(((*iter)->GetPosition().x - GetPosition().x) / ((*iter)->GetPosition().y - GetPosition().y));
				rotation = rotation * 100;
				//cout << rotation << endl;
				Spaceship::Rotate(rotation);
				//cout << "Rotation" << endl;
				Spaceship::Shoot();
				//cout << "Shoot" << endl;
				Spaceship::Thrust(10);
				//cout << "Thrust" << endl;
			}
		}
		asteroidsNear.clear();
	}
}

void AISpaceship::AttackMode()
{
	//cout << "Attack mode" << endl;
	float distance = 0;
	shared_ptr<GameObject> nearestAsteroid;
	list<shared_ptr<GameObject>>::iterator iter;
	for (iter = mWorld->GetGameObjects().begin(); iter != mWorld->GetGameObjects().end(); iter++) {
		//cout << *iter << endl;
		if (*iter != nullptr) {
			if ((*iter)->GetType() == GameObjectType("Asteroid")) {
				if (distance != NULL) {
					//cout << "this distance = " << sqrt(pow(((*iter)->GetPosition().x - GetPosition().x), 2) + pow(((*iter)->GetPosition().y - GetPosition().y), 2)) << endl;
					if (distance > sqrt(pow(((*iter)->GetPosition().x - GetPosition().x), 2) + pow(((*iter)->GetPosition().y - GetPosition().y), 2))) {
						distance = sqrt(pow(((*iter)->GetPosition().x - GetPosition().x), 2) + pow(((*iter)->GetPosition().y - GetPosition().y), 2));
						nearestAsteroid = *iter;
					}
				}
				else {
					distance = sqrt(pow(((*iter)->GetPosition().x - GetPosition().x), 2) + pow(((*iter)->GetPosition().y - GetPosition().y), 2));
					//cout << "distance = " << distance << endl;
					nearestAsteroid = *iter;
				}
			}
		}
	}
	if (nearestAsteroid != nullptr) {
		//cout << "AISpaceship pos x = " << GetPosition().x << endl;
		//cout << "AISpaceship pos y = " << GetPosition().y << endl;
		//cout << "Asteroid pos x = " << nearestAsteroid->GetPosition().x << endl;
		//cout << "Asteroid pos y = " << nearestAsteroid->GetPosition().y << endl;
	    float rotation = cos((nearestAsteroid->GetPosition().x - GetPosition().x) / (nearestAsteroid->GetPosition().y - GetPosition().y));
		rotation = rotation * 100;
		//cout << "Rotation = " << rotation << endl;
		Spaceship::Rotate(rotation);
		//cout << "Rotation" << endl;
		Spaceship::Thrust(10);
		//cout << "Thrust" << endl;
		Spaceship::Shoot();
		//cout << "Shoot" << endl;
	}
}

void AISpaceship::UpdateAI() {
	if (active == true) {
		if (CheckVicinity() == true) {
			DefenseMode();
		}
		else {
			AttackMode();
		}
	}
}