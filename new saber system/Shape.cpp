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

void CircleShape::UpdateVertices(float angle, const Vec2f& position)
{
	return;
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



void PolygonShape::UpdateVertices(float angle, const Vec2f& position)
{
	for (int i = 0; i < localvertices.size(); i++)
	{

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

