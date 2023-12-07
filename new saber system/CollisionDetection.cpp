#include "CollisionDetection.h"

bool CollisionDetection::IsColliding(Body* a, Body* b, Contact& contact)
{
    bool aIsCircle = a->shape->GetType() == CIRCLE;
    bool bIsCircle = b->shape->GetType() == CIRCLE;
    bool aIsPolygon = a->shape->GetType() == POLYGON || a->shape->GetType() == BOX;
    bool bIsPolygon = b->shape->GetType() == POLYGON || b->shape->GetType() == BOX;

    if (aIsCircle && bIsCircle)
    {
        return IsCollidingCirclesToCircle(a, b, contact);
    }
    if (aIsPolygon && bIsPolygon)
    {
        return IsCollidingPolygonToPolygon(a, b, contact);
    }
    if (aIsPolygon && bIsCircle)
    {
        return IsCollidingPolygonToCircle(a, b, contact);
    }
    if (bIsPolygon && aIsCircle)
    {
        return IsCollidingPolygonToCircle(b, a, contact);
    }
}

bool CollisionDetection::IsCollidingCirclesToCircle(Body* a, Body* b, Contact& contact)
{
    CircleShape* aCircleShape = (CircleShape*)a->shape;
    CircleShape* bCircleShape = (CircleShape*)b->shape;

    const Vec2f ab = b->position - a->position;
    const float radiusSum = aCircleShape->radius + bCircleShape->radius;

    bool isColliding = ab.MagnitudeSquared() <= (radiusSum * radiusSum);

    if (!isColliding)
    {
        return false;
    }
    
    contact.a = a;
    contact.b = b;
    contact.normal = ab;
    contact.normal.Normalize();
    contact.start = b->position - contact.normal * bCircleShape->radius;
    contact.end = a->position + contact.normal * aCircleShape->radius;
    contact.depth = (contact.end - contact.start).Magnitude();

    return true;
   
}

bool CollisionDetection::IsCollidingPolygonToPolygon(Body* a, Body* b, Contact& contact)
{
    PolygonShape* aPolygonShape = (PolygonShape*)a->shape;
    PolygonShape* bPolygonShape = (PolygonShape*)b->shape;

    Vec2f aAxis, bAxis;
    Vec2f aPoint, bPoint;

    float abSeparation = aPolygonShape->FindMinSeparation(bPolygonShape, aAxis, aPoint);
    float baSeparation = bPolygonShape->FindMinSeparation(aPolygonShape,bAxis,bPoint);

    if (abSeparation >= 0)
        return false;
    if( baSeparation >= 0)
        return false;

    contact.a = a;
    contact.b = b;

    if (abSeparation > baSeparation)
    {
        contact.depth = -abSeparation;
        contact.normal = aAxis.Normal();
        contact.start = aPoint;
        contact.end = aPoint + contact.normal * contact.depth;
    }
    else
    {
        contact.depth = -baSeparation;
        contact.normal = -bAxis.Normal();
        contact.start = bPoint - contact.normal * contact.depth;
        contact.end = bPoint;
    }

    return true;

}

bool CollisionDetection::IsCollidingPolygonToCircle(Body* polygon, Body* circle,Contact& contact)
{
    const PolygonShape* polygonShape = (PolygonShape*)polygon->shape;
    const CircleShape* circleShape = (CircleShape*)circle->shape;
    const std::vector<Vec2f>& polygonVertices = polygonShape->worldvertices;

    bool isOutside = false;
    Vec2f minCurrVertex;
    Vec2f minNextVertex;
    float distanceCircleEdge = std::numeric_limits<float>::lowest();

    for (int i = 0; i < polygonVertices.size(); i++)
    {
        int currVertex = i;
        int nextVertex = (i + 1) % polygonVertices.size();
        Vec2f edge = polygonShape->EdgeAt(currVertex);
        Vec2f normal = edge.Normal();

        Vec2f circleCenter = circle->position - polygonVertices[currVertex];
        float projection = circleCenter.Dot(normal);

        if (projection > 0)
        {
            distanceCircleEdge = projection;
            minCurrVertex = polygonShape->worldvertices[currVertex];
            minNextVertex = polygonShape->worldvertices[nextVertex];
            isOutside = true;

            break;
        }
        else
        {
            if (projection > distanceCircleEdge)
            {
                distanceCircleEdge = projection;
                minCurrVertex = polygonVertices[currVertex];
                minNextVertex = polygonVertices[nextVertex];
            }
        }

    }
    if (isOutside)
    {
        Vec2f v1 = circle->position - minCurrVertex;
        Vec2f v2 = minNextVertex - minCurrVertex;

        if (v1.Dot(v2) < 0)
        {
            if (v1.Magnitude() > circleShape->radius)
            {
                return false;
            }
            else
            {
                contact.a = polygon;
                contact.b = circle;
                contact.depth = circleShape->radius - v1.Magnitude();
                contact.normal = v1.Normalize();
                contact.start = circle->position + (contact.normal * -circleShape->radius);
                contact.end = contact.start + (contact.normal * contact.depth);
            }
        }
        else
        {
            v1 = circle->position - minNextVertex;
            v2 = minCurrVertex - minNextVertex;
            if (v1.Dot(v2) < 0)
            {
                if (v1.Magnitude() > circleShape->radius)
                {
                    return false;
                }
                else
                {
                    contact.a = polygon;
                    contact.b = circle;
                    contact.depth = circleShape->radius - v1.Magnitude();
                    contact.normal = v1.Normalize();
                    contact.start = circle->position + (contact.normal * -circleShape->radius);
                    contact.end = contact.start + (contact.normal * contact.depth);
                }
            }
            else
            {
                if (distanceCircleEdge > circleShape->radius)
                {
                    return false;
                }
                else
                {
                    contact.a = polygon;
                    contact.b = circle;
                    contact.depth = circleShape->radius - distanceCircleEdge;
                    contact.normal = (minNextVertex - minCurrVertex).Normal();
                    contact.start = circle->position - (contact.normal * circleShape->radius);
                    contact.end = contact.start + (contact.normal * contact.depth);
                }
            }
        }
    }
    else
    {
        contact.a = polygon;
        contact.b = circle;
        contact.depth = circleShape->radius - distanceCircleEdge;
        contact.normal = (minNextVertex - minCurrVertex).Normal();
        contact.start = circle->position - (contact.normal * circleShape->radius);
        contact.end = contact.start + (contact.normal * contact.depth);
    }

    return true;
}


