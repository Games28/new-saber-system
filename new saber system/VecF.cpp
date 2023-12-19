#include "VecF.h"

VecF::VecF() : x(0.0), y(0.0) {

}

VecF::VecF(float x, float y) : x(x), y(y) {

}

void VecF::Add(const VecF& v) {
	x += v.x;
	y += v.y;
}

void VecF::Sub(const VecF& v) {
	x -= v.x;
	y -= v.y;
}

void VecF::Scale(const float n) {
	x *= n;
	y *= n;
}

VecF VecF::Rotate(const float angle) const {
	VecF result;
	result.x = x * cos(angle) - y * sin(angle);
	result.y = x * sin(angle) + y * cos(angle);
	return result;
}

float VecF::Magnitude() const {
	return sqrtf(x * x + y * y);
}

float VecF::MagnitudeSquared() const {
	return (x * x + y * y);
}

VecF& VecF::Normalize() {
	float length = Magnitude();
	if (length != 0.0) {
		x /= length;
		y /= length;
	}
	return *this;
}

VecF VecF::UnitVector() const {
	VecF result = VecF(0, 0);
	float length = Magnitude();
	if (length != 0.0) {
		result.x = x / length;
		result.y = y / length;
	}
	return result;
}

VecF VecF::Normal() const {
	return VecF(y, -x).Normalize();
}

float VecF::Dot(const VecF& v) const {
	return (x * v.x) + (y * v.y);
}

float VecF::Cross(const VecF& v) const {
	return (x * v.y) - (y * v.x);
}

VecF& VecF::operator = (const VecF& v) {
	x = v.x;
	y = v.y;
	return *this;
}

bool VecF::operator == (const VecF& v) const {
	return x == v.x && y == v.y;
}

bool VecF::operator != (const VecF& v) const {
	return !(*this == v);
}

VecF VecF::operator + (const VecF& v) const {
	VecF result;
	result.x = x + v.x;
	result.y = y + v.y;
	return result;
}

VecF VecF::operator - (const VecF& v) const {
	VecF result;
	result.x = x - v.x;
	result.y = y - v.y;
	return result;
}

VecF VecF::operator * (const float n) const {
	VecF result;
	result.x = x * n;
	result.y = y * n;
	return result;
}

VecF VecF::operator / (const float n) const {
	VecF result;
	result.x = x / n;
	result.y = y / n;
	return result;
}

VecF& VecF::operator += (const VecF& v) {
	x += v.x;
	y += v.y;
	return *this;
}

VecF& VecF::operator -= (const VecF& v) {
	x -= v.x;
	y -= v.y;
	return *this;
}

VecF& VecF::operator *= (const float n) {
	x *= n;
	y *= n;
	return *this;
}

VecF& VecF::operator /= (const float n) {
	x /= n;
	y /= n;
	return *this;
}

VecF VecF::operator - () {
	VecF result;
	result.x = x * -1;
	result.y = y * -1;
	return result;
}
