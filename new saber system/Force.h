#ifndef FORCE_H
#define FORCE_H
#include "Vec2.h"
#include "Body.h"


class Force
{
public:
	Force() = default;
	static float clamp(float val, float min, float max);
public:
	static Vec2f GenerateDragForce(const Body& particle, float k);
	static Vec2f GenerateFrictionForce(const Body& particle, float k);
	static Vec2f GenerateGravitationalForce(const Body& a, const Body& b, float G, float minDistance, float maxDistance);
	static Vec2f GenerateSpringForce(const Body& particle, Vec2f anchor, float restLength, float k);
	static Vec2f GenerateSpringForce(const Body& a, const Body& b, float restLength, float k);
};

#endif // !FORCE_H



