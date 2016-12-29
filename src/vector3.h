#pragma once

#include "import_std.h"
#include "matrix4.h"

struct V3
{
public:
    V3() { this->x = 0; this->y = 0; this->z = 0; }
    V3(float v) { this->x = v; this->y = v; this->z = v; }
    V3(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }

    V3 operator +(V3 &b) { return V3(this->x + b.x, this->y + b.y, this->z + b.z); }
    V3 operator -(V3 &b) { return V3(this->x - b.x, this->y - b.y, this->z - b.z); }
    V3 operator *(V3 &b) { return V3(this->x * b.x, this->y * b.y, this->z * b.z); }
    V3 operator /(V3 &b) { return V3(this->x / b.x, this->y / b.y, this->z / b.z); }

    void operator +=(V3 &b) { this->x += b.x; this->y += b.y; this->z += b.z; }
    void operator -=(V3 &b) { this->x -= b.x; this->y -= b.y; this->z -= b.z; }
    void operator *=(V3 &b) { this->x *= b.x; this->y *= b.y; this->z *= b.z; }
    void operator /=(V3 &b) { this->x /= b.x; this->y /= b.y; this->z /= b.z; }

    V3 operator +(float &b) { return V3(this->x + b, this->y + b, this->z + b); }
    V3 operator -(float &b) { return V3(this->x - b, this->y - b, this->z - b); }
    V3 operator *(float &b) { return V3(this->x * b, this->y * b, this->z * b); }
    V3 operator /(float &b) { return V3(this->x / b, this->y / b, this->z / b); }

    void operator +=(float &b) { this->x += b; this->y += b; this->z += b; }
    void operator -=(float &b) { this->x -= b; this->y -= b; this->z -= b; }
    void operator *=(float &b) { this->x *= b; this->y *= b; this->z *= b; }
    void operator /=(float &b) { this->x /= b; this->y /= b; this->z /= b; }

    bool operator ==(V3 &b) { return (this->x == b.x && this->y == b.y && this->z == b.z); }
    bool operator !=(V3 &b) { return (this->x != b.x || this->y != b.y || this->z != b.z); }

    float Length() { return sqrt(this->x * this->x + this->y * this->y + this->z * this->z); }
    void Normalize() { float l = this->Length(); this->x /= l; this->y /= l; this->z /= l; }

    void Transform(Matrix4 &m) 
    {
	float a = this->x;
	float b = this->y;
	float c = this->z;

	this->x = a * m.m11 + b * m.m21 + c * m.m31 + 1 * m.m41;
	this->y = a * m.m12 + b * m.m22 + c * m.m32 + 1 * m.m42;
	this->z = a * m.m13 + b * m.m23 + c * m.m33 + 1 * m.m43;
    }
	
    static float Distance(V3 &a, V3 &b)
    { 
	if (a == b) 
	    return 0; 
	return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y) + (b.z - a.z) * (b.z - a.z)); 
    }

    static float Dot(V3 &a, V3 &b)
    {
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
    }

    static const V3 Zero;

    float x, y, z;
};
