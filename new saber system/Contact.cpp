#include "Contact.h"

void Contact::ResolvePentration()
{
	if (a->IsStatic() && b->IsStatic()) return;

	float da = depth / (a->invMass + b->invMass) * a->invMass;
	float db = depth / (a->invMass + b->invMass) * b->invMass;

	a->position -= normal * da * 0.8f;
	b->position += normal * db * 0.8f;


	a->shape->UpdateVertices(a->rotation,a->position);
	b->shape->UpdateVertices(b->rotation,b->position);
 }

void Contact::ResolveCollision()
{
	ResolvePentration();

	float e = std::min(a->restitution, b->restitution);
	float f = std::min(a->friction, b->friction);

	//const Vec2 vrel = (a->velocity - b->velocity);
	Vec2f ra = end - a->position;
	Vec2f rb = start - b->position;
	Vec2f va = a->velocity + Vec2f(-a->angularvelocity * ra.y, a->angularvelocity * ra.x);
	Vec2f vb = b->velocity + Vec2f(-b->angularvelocity * rb.y, a->angularvelocity * rb.x);
	const Vec2f vrel = va - vb;

	float vrelDotNormal = vrel.Dot(normal);
	const Vec2f impulseDirectionN = normal;
    const float impulseMagnitudeN = -(1 + e) * vrelDotNormal / ((a->invMass + b->invMass) + ra.Cross(normal) * ra.Cross(normal) * a->invInertia
		+ rb.Cross(normal) * rb.Cross(normal) * b->invInertia); 
    Vec2f jn = impulseDirectionN * impulseMagnitudeN;


	Vec2f tangent = normal.Normal();
	float vrelDotTangent = vrel.Dot(tangent);
	const Vec2f impulseDirectionT = tangent;
	const float impulseMagnitudeT = f * -(1 + e) * vrelDotTangent / ((a->invMass + b->invMass) + ra.Cross(tangent) * ra.Cross(tangent) * a->invInertia
		+ rb.Cross(tangent) * rb.Cross(tangent) * b->invInertia);
	Vec2f jt = impulseDirectionT * impulseMagnitudeT;

	Vec2f J = jn + jt;

	a->ApplyImpulseAtPoint(J,ra);
	b->ApplyImpulseAtPoint(-J,rb);

}
