#ifndef WORLD_H
#define WORLD_H
#include "olcPixelGameEngine.h"
#include "Body.h"
#include "CollisionDetection.h"
#include <iostream>
#include "Constants.h"
#include "Constraint.h"

class World
{
public:
	World(float gravity);
	~World();

	void AddBody(Body* body);
	std::vector<Body*>& GetBodies();

	void AddForce(const Vec2f& force);
	void AddTorque(float torque);

	void AddConstraint(Constraint* constraint);
	std::vector<Constraint*>& GetConstraints();

	void Update(olc::PixelGameEngine* pge, float dt);

	void CheckCollisions(olc::PixelGameEngine* ptr);

private:
	float G = 9.8f;
	std::vector<Body*> bodies;
	std::vector<Constraint*> constraints;
	std::vector<Vec2f> forces;
	std::vector<float> torques;
};

#endif // !WORLD_H





