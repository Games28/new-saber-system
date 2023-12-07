#include "Force.h"

float Force::clamp(float val, float min, float max)
{
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

Vec2f Force::GenerateDragForce(const Body& particle, float k)
{
    Vec2f dragForce = Vec2f(0, 0);

    if (particle.velocity.MagnitudeSquared() > 0)
    {
        Vec2f dragDirection = particle.velocity.UnitVector() * -1.0f;

        float dragMagnitude = k * particle.velocity.MagnitudeSquared();

        dragForce = dragDirection * dragMagnitude;
    }

    return dragForce;
}

Vec2f Force::GenerateFrictionForce(const Body& particle, float k)
{
    Vec2f frictionForce = Vec2f(0, 0);

    Vec2f frictionDirection = particle.velocity.UnitVector() * -1.0f;

    float frictionMagnitude = k;

    frictionForce = frictionDirection * frictionMagnitude;


    return frictionForce;
}

Vec2f Force::GenerateGravitationalForce(const Body& a, const Body& b, float G, float minDistance, float maxDistance)
{
    Vec2f d = (b.position - a.position);

    float distanceSquared = d.MagnitudeSquared();

    distanceSquared = clamp(distanceSquared, minDistance, maxDistance);

    Vec2f attractionDirection = d.UnitVector();
    float attractionMagnitude = G * (a.mass * b.mass) / distanceSquared;

    Vec2f attractionForce = attractionDirection * attractionMagnitude;

    return attractionForce;
}

Vec2f Force::GenerateSpringForce(const Body& particle, Vec2f anchor, float restLength, float k)
{
    Vec2f d = particle.position - anchor;

    float displacement = d.Magnitude() - restLength;

    Vec2f springDirection = d.UnitVector();
    float spriteMagnitude = -k * displacement;

    Vec2f springForce = springDirection * spriteMagnitude;

    return springForce;
}

Vec2f Force::GenerateSpringForce(const Body& a, const Body& b, float restLength, float k)
{
    Vec2f d = a.position - b.position;

    float displacement = d.Magnitude() - restLength;

    Vec2f springDirection = d.UnitVector();
    float spriteMagnitude = -k * displacement;

    Vec2f springForce = springDirection * spriteMagnitude;

    return springForce;
}
