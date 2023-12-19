#ifndef VECF_H
#define VECF_H
#include <math.h>
struct VecF {
    float x;
    float y;

    VecF();
    VecF(float x, float y);

    ~VecF() = default;

    void Add(const VecF& v);                 // v1.Add(v2)
    void Sub(const VecF& v);                 // v1.Sub(v2)
    void Scale(const float n);               // v1.Scale(n)
    VecF Rotate(const float angle) const;    // v1.Rotate(angle)

    float Magnitude() const;                 // v1.Magnitude()
    float MagnitudeSquared() const;          // v1.MagnitudeSquared()

    VecF& Normalize();                       // v1.Normalize()
    VecF UnitVector() const;                 // v1.UnitVector()
    VecF Normal() const;                     // n = v1.Normal()

    float Dot(const VecF& v) const;          // v1.Dot(v2)
    float Cross(const VecF& v) const;        // v1.Cross(v2)

    VecF& operator = (const VecF& v);        // v1 = v2
    bool operator == (const VecF& v) const;  // v1 == v2
    bool operator != (const VecF& v) const;  // v1 != v2

    VecF operator + (const VecF& v) const;   // v1 + v2
    VecF operator - (const VecF& v) const;   // v1 - v2
    VecF operator * (const float n) const;   // v1 * n
    VecF operator / (const float n) const;   // v1 / n
    VecF operator - ();                      // -v1

    VecF& operator += (const VecF& v);       // v1 += v2
    VecF& operator -= (const VecF& v);       // v1 -= v2
    VecF& operator *= (const float n);       // v1 *= n
    VecF& operator /= (const float n);       // v1 /= n
};

#endif


