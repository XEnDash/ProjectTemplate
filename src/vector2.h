#pragma once

#include "import_std.h"
#include "matrix4.h"

struct V2
{
public:
    V2() { this->x = 0; this->y = 0; }
    V2(float v) { this->x = v; this->y = v; }
    V2(float x, float y) { this->x = x; this->y = y; }
	
    V2 operator +(V2 &b) { return V2(this->x + b.x, this->y + b.y); }
    V2 operator -(V2 &b) { return V2(this->x - b.x, this->y - b.y); }
    V2 operator *(V2 &b) { return V2(this->x * b.x, this->y * b.y); }
    V2 operator /(V2 &b) { return V2(this->x / b.x, this->y / b.y); }

    void operator +=(V2 &b) { this->x += b.x; this->y += b.y; }
    void operator -=(V2 &b) { this->x -= b.x; this->y -= b.y; }
    void operator *=(V2 &b) { this->x *= b.x; this->y *= b.y; }
    void operator /=(V2 &b) { this->x /= b.x; this->y /= b.y; }

    V2 operator +(float &b) { return V2(this->x + b, this->y + b); }
    V2 operator -(float &b) { return V2(this->x - b, this->y - b); }
    V2 operator *(float &b) { return V2(this->x * b, this->y * b); }
    V2 operator /(float &b) { return V2(this->x / b, this->y / b); }

    void operator +=(float &b) { this->x += b; this->y += b; }
    void operator -=(float &b) { this->x -= b; this->y -= b; }
    void operator *=(float &b) { this->x *= b; this->y *= b; }
    void operator /=(float &b) { this->x /= b; this->y /= b; }
	
    bool operator ==(V2 &b) { return (this->x == b.x && this->y == b.y); }
    bool operator !=(V2 &b) { return (this->x != b.x || this->y != b.y); }

    float Length() { return sqrt(this->x * this->x + this->y * this->y); }
    void Normalize() { float l = this->Length(); this->x /= l; this->y /= l; }

    void Transform(Matrix4 &m) 
    {
	float a = this->x;
	float b = this->y;

	this->x = a * m.m11 + b * m.m21 + 1 * m.m31 + 1 * m.m41;
	this->y = a * m.m12 + b * m.m22 + 1 * m.m32 + 1 * m.m42;
    }
	
    static float Distance(V2 &a, V2 &b)
    { 
	if (a == b) 
	    return 0; 
	return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y)); 
    }

    static float Dot(V2 &a, V2 &b)
    {
	return (a.x * b.x) + (a.y * b.y);
    }
	
    static float Cross(V2 &a, V2 &b)
    {
	return (b.x * a.y) - (b.y * a.x);
    }

    static const V2 Zero;

    float x, y;
};
