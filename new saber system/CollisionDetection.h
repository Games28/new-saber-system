#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H
#include "Body.h"
#include "Contact.h"

struct CollisionDetection
{
    static bool IsColliding(Body* a, Body* b, Contact & contact);
    static bool IsCollidingCirclesToCircle(Body* a, Body* b,Contact& contact);
    static bool IsCollidingPolygonToPolygon(Body* a, Body* b, Contact& contact);
    static bool IsCollidingPolygonToCircle(Body* a, Body* b,Contact& contact);
    
};
#endif // !COLLISIONDETECTION_H


