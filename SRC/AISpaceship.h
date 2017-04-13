#ifndef __AISPACESHIP_H__
#define __AISPACESHIP_H__

#include "Spaceship.h"
#include <vector>

class AISpaceship : public Spaceship
{
public:
	AISpaceship();
	AISpaceship(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r);
	AISpaceship(const AISpaceship& s);
	virtual ~AISpaceship(void);

	virtual void Update(int t);
	virtual void Render(void);

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList &objects);

	void Activate();
	void Deactivate();
	bool CheckVicinity();
	void DefenseMode();
	void AttackMode();
	void UpdateAI();
	float GetRadius();
	void SetActive(bool activate) { active = activate; }
	bool GetActive() const { return active; }

private:
	bool active = false;
	float radius;
	vector<shared_ptr<GameObject>> asteroidsNear;
};

#endif