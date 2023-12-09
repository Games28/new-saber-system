#include "Body.h"
#include <iostream>

Body::Body(const Shape& shape,int index,float x, float y, float mass)
{
	this->shape = shape.Clone();
	this->bodyindex = index;
	//linear motion
	this->position = Vec2f(x, y);
	this->velocity = Vec2f(0, 0);
	this->acceleration = Vec2f(0, 0);
	//angular motion
	this->rotation = 0.0f;
	this->angularvelocity = 0.0f;
	this->angularacceleration = 0.0f;
	this->restitution = 1.0f;
	this->friction = 0.7f;
	this->sumForces = Vec2f{ 0,0 };
	this->sumTorque = 0.0f;
	this->mass = mass;
	if (mass != 0.0f)
	{
		this->invMass = 1.0f / mass; 
	}
	else
	{
		this->invMass = 0.0f;
	}

	Inertia = shape.GetMomentOfInertia() * mass;

	if (Inertia != 0.0f)
	{
		this->invInertia = 1.0f / Inertia;
	}
	else
	{
		this->invInertia = 0.0f;
	}

	std::cout << "body constructed!" << std::endl;
}

Body::~Body()
{
	delete &shape;
	std::cout << "body destroyed!" << std::endl;
}

void Body::integrateLinear(float deltatime)
{
	if (movementstatic)
	{
		return;
	}

	acceleration = sumForces * invMass;
	
	velocity += acceleration * deltatime;

	position += velocity * deltatime;

	ClearForces();
}

void Body::integrateAngular(float deltatime)
{
	if (rotationstatic)
	{
		return;
	}

	angularacceleration = sumTorque * invInertia;

	angularvelocity += angularacceleration * deltatime;

	rotation += angularvelocity * deltatime;

	ClearTorgue();
}

void Body::AddForce(const Vec2f& force)
{
	sumForces += force;
}

void Body::AddTorque(float torque)
{
	sumTorque += torque;
}

void Body::ClearTorgue()
{
	sumTorque = 0;
}

void Body::ClearForces()
{
	sumForces = Vec2f(0.0f, 0.0f);
}

bool Body::IsStatic() const 
{
	const float epsilon = 0.005f;
	bool result = fabs(invMass - 0.0f) < epsilon;
	 
	return result;
}

void Body::ApplyImpulse(const Vec2f& j)
{
	if (movementstatic) return;

	velocity += j * invMass;
}

void Body::ApplyImpulse(const Vec2f& j, const Vec2f& r)
{
	if (movementstatic || rotationstatic) return;

	velocity += j * invMass;
	angularvelocity += r.Cross(j) * invInertia;
}

void Body::SetTexture(const char* textureFileName)
{
	sprite = new olc::Sprite(textureFileName);
	decal = new olc::Decal(sprite);
}

void Body::Update(float deltatime, int index)
{
	integrateLinear(deltatime);
	integrateAngular(deltatime);
	bool isPolygon = shape->GetType() == POLYGON;
	bool isBox = shape->GetType() == BOX;

	if (isPolygon)
	{
		PolygonShape* polygonshape = (PolygonShape*)shape;
		
		polygonshape->UpdatePolygonVertices(rotation, position);
	}
	if (isBox)
	{
		BoxShape* boxshape = (BoxShape*)shape;
		boxshape->UpdateBoxVertices(rotation, position);
	}
}

void Body::Cutline(olc::PixelGameEngine* pge, std::vector<LineData>& linedata)
{
	BoxShape* boxshape = (BoxShape*)shape;
	
	int i = linedata.size() - 1;
	Vec2i mousepos = { linedata[i].x, linedata[i].y };
	olc::Pixel p = olc::BLUE;
	if (this->isleft(0, mousepos))
	{
		p = olc::WHITE;
	}
	float newpoint1;
	if (this->isleft(1, mousepos))
	{
		p = olc::WHITE;
	}
	float newpoint2;
	if (this->isleft(2, mousepos))
	{
		p = olc::WHITE;
	}
	float newpoint3;
	if (this->isleft(3, mousepos))
	{
		p = olc::WHITE;
	}

	pge->DrawLine(pge->ScreenWidth() / 2, pge->ScreenHeight() / 2, mousepos.x + 10, mousepos.y, p);

}

bool Body::isleft(int index, Vec2i point)
{
	BoxShape* boxshape = (BoxShape*)shape;
	int currVertex = index;
	int nextVertex = (index + 1) % boxshape->worldvertices.size();

	Vec2f worldV1 = boxshape->worldvertices[currVertex];
	Vec2f worldV2 = boxshape->worldvertices[nextVertex];

	
	return (worldV2.x - worldV1.x) * (point.y - worldV1.y) - (worldV2.y - worldV1.y) * (point.x - worldV1.x) < 0;
}
