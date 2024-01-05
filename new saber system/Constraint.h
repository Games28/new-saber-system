#ifndef CONSTRAINT_H
#define CONSTRAINT_H
#include "Body.h"
#include "MatMN.h"
#include "VecF.h"

class Constraint
{
public:
	Body* a;
	Body* b;
	
	Vec2f aPoint;
	Vec2f bPoint;

	MatMN GetInvM() const;
	VecN GetVelocities() const;
	
	virtual void PreSolve(const float dt) {}
	virtual void Solve() {}
	virtual void PostSolve() {}
};

class JointConstraint : public Constraint
{
private:
	MatMN jacobian;
	VecN cachedLambda;
	float bias;

public:
	JointConstraint();
	JointConstraint(Body* a, Body* b, const Vec2f& anchorPoint);
	void PreSolve(const float dt) override;
	void Solve() override;
	void PostSolve() override;

};

class PenetrationConstraint : public Constraint
{
private:
	MatMN jacobian;
	VecN cachedLambda;
	float bias;
	Vec2f normal;

public:
	PenetrationConstraint();
	PenetrationConstraint(Body* a, Body* b, const Vec2f& aCollisionPoint, Vec2f& bCollisionPoint,const Vec2f& normal);
	void PreSolve(const float dt) override;
	void Solve() override;
	void PostSolve() override;
};

#endif // !CONSTRAINT_H



