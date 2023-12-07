#ifndef CONTACT_H
#define CONTACT_H
#include "Vec2.h"
#include "Body.h"

struct Contact
{
	Body* a;
	Body* b;

	Vec2f start;
	Vec2f end;

	Vec2f normal;
	float depth;

	Contact() = default;
	~Contact() = default;
	void ResolvePentration();
	void ResolveCollision();
};
#endif // !CONTACT_H

