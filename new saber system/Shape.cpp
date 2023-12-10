#include "Shape.h"
#include <iostream>

CircleShape::CircleShape(const float radius, const int VerticesCount)
{
	this->radius = radius;
	this->vertexCount = VerticesCount;
	vertices.push_back(Vec2f(0.0f, 0.0f));
	for (int i = 0; i < vertexCount; i++)
	{
		vertices.push_back({ cosf(i / (float)(vertexCount - 1) * 2.0f * 3.14159f), sinf(i / (float)(vertexCount - 1) * 2.0f * 3.14159f) });

	}

	
	std::cout << "circleshape constructor called" << std::endl;

}

CircleShape::~CircleShape()
{
	std::cout << "circleshape destructor called" << std::endl;
}

ShapeType CircleShape::GetType() const
{
	return CIRCLE;
}

Shape* CircleShape::Clone() const
{
	return new CircleShape(radius, vertexCount);
}

float CircleShape::GetMomentOfInertia() const
{
	return 0.5f * ( radius * radius);
}

PolygonShape::PolygonShape(const std::vector<Vec2f> vertices)
{
	for (auto vertex : vertices)
	{
		localvertices.push_back(vertex);
		worldvertices.push_back(vertex);
		
	}
}

PolygonShape::~PolygonShape()
{

	std::cout << "polygoneshape destructor called" << std::endl;
}

Vec2f PolygonShape::EdgeAt(int index) const
{
	int currVertex = index;
	int nextVertex = (index + 1) % worldvertices.size();

	return worldvertices[nextVertex] - worldvertices[currVertex];
}

bool PolygonShape::isinsidebox(int index,Vec2i point,float& Npoint)
{
	
	int currVertex = index;
	int nextVertex = (index + 1) % worldvertices.size();

	Vec2f worldV1 = worldvertices[currVertex];
	Vec2f worldV2 = worldvertices[nextVertex];

	Npoint = (worldV2.x - worldV1.x) * (point.y - worldV1.y) - (worldV2.y - worldV1.y) * (point.x - worldV1.x);
	return (worldV2.x - worldV1.x) * (point.y - worldV1.y) - (worldV2.y - worldV1.y) * (point.x - worldV1.x) > 0;
}

bool PolygonShape::isinside2(int index, Vec2i point, float& Npoint)
{

	int currVertex = index;
	int nextVertex = (index + 1) % worldvertices.size();

	Vec2f worldV1 = worldvertices[currVertex];
	Vec2f worldV2 = worldvertices[nextVertex];

	Npoint = (worldV2.x - worldV1.x) * (point.y - worldV1.y) - (worldV2.y - worldV1.y) * (point.x - worldV1.x);
	return (worldV2.x - worldV1.x) * (point.y - worldV1.y) - (worldV2.y - worldV1.y) * (point.x - worldV1.x) > 0;
}

bool PolygonShape::isright(int index, Vec2i point)
{
	int currVertex = index;
	int nextVertex = (index + 1) % worldvertices.size();

	Vec2f worldV1 = worldvertices[currVertex];
	Vec2f worldV2 = worldvertices[nextVertex];


	return (worldV2.x - worldV1.x) * (point.y - worldV1.y) - (worldV2.y - worldV1.y) * (point.x - worldV1.x) > 0;
}

float PolygonShape::FindMinSeparation(PolygonShape* other, Vec2f& axis, Vec2f& point)
{
	float separation = std::numeric_limits<float>::lowest();

	for (int i = 0; i < worldvertices.size(); i++)
	{
		Vec2f va = this->worldvertices[i];
		Vec2f normal = this->EdgeAt(i).Normal();

		float minSep = std::numeric_limits<float>::max();
		Vec2f minVertex;
		for (int j = 0; j < other->worldvertices.size(); j++)
		{
			Vec2f vb = other->worldvertices[j];
			float proj = (vb - va).Dot(normal);
			if (proj < minSep)
			{
				minSep = proj;
				minVertex = vb;
			}
			


		}
		if (minSep > separation)
		{
			separation = minSep;
			axis = this->EdgeAt(i);
			point = minVertex;
		}
	}

	return separation;
}



ShapeType PolygonShape::GetType() const
{
	return POLYGON;
}

Shape* PolygonShape::Clone() const
{
	return new PolygonShape(localvertices);
}

float PolygonShape::GetMomentOfInertia() const
{
	return 5000;
}

void PolygonShape::UpdatePolygonVertices(float angle, const Vec2f& position)
{
	for (int i = 0; i < localvertices.size(); i++)
	{
		Vec2f zero = { 0,0 };
		worldvertices[i] = localvertices[i].Rotate(angle);
		
		
		worldvertices[i] += position;
		
	}
}

BoxShape::BoxShape(float width, float height)
{
	this->width = width;
	this->height = height;

	localvertices.push_back(Vec2f(-width / 2.0f, -height / 2.0f)); 
	localvertices.push_back(Vec2f(+width / 2.0f, -height / 2.0f));
	localvertices.push_back(Vec2f(+width / 2.0f, +height / 2.0f));
	localvertices.push_back(Vec2f(-width / 2.0f, +height / 2.0f));

	worldvertices.push_back(Vec2f(-width / 2.0f, -height / 2.0f));
	worldvertices.push_back(Vec2f(+width / 2.0f, -height / 2.0f));
	worldvertices.push_back(Vec2f(+width / 2.0f, +height / 2.0f));
	worldvertices.push_back(Vec2f(-width / 2.0f, +height / 2.0f));
	
	offsetverts.push_back(Vec2f(0.0f,0.0f));
	offsetverts.push_back(Vec2f(0.0f, 0.0f));
	offsetverts.push_back(Vec2f(0.0f, 0.0f));
	offsetverts.push_back(Vec2f(0.0f, 0.0f));
	
}

BoxShape::~BoxShape()
{
}

ShapeType BoxShape::GetType() const
{
	return BOX;
}

Shape* BoxShape::Clone() const
{
	return new BoxShape(width,height);
}

float BoxShape::GetMomentOfInertia() const
{
	return (0.083333) * (width * width + height * height);
}

void BoxShape::UpdateBoxVertices(float angle, const Vec2f& position)
{
	for (int i = 0; i < localvertices.size(); i++)
	{
		Vec2f zero = { 0,0 };
		worldvertices[i] = localvertices[i].Rotate(angle) + offsetverts[i].Rotate(angle);


		worldvertices[i] += position;

	}
}
