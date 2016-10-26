#include "Matrix4.h"

#include <math.h>

void Matrix4::CreateIdentity()
{
	this->m11 = 1.0f;
	this->m22 = 1.0f;
	this->m33 = 1.0f;
	this->m44 = 1.0f;

	this->m12 = 0.0f;
	this->m13 = 0.0f;
	this->m14 = 0.0f;

	this->m21 = 0.0f;
	this->m23 = 0.0f;
	this->m24 = 0.0f;

	this->m31 = 0.0f;
	this->m32 = 0.0f;
	this->m34 = 0.0f;

	this->m41 = 0.0f;
	this->m42 = 0.0f;
	this->m43 = 0.0f;
}

void Matrix4::CreatePerspective(float fov, float w, float h, float near, float far)
{
	float ar = h / w;
	float tan_half_fov = tanf(fov / 2.0f);
	
	this->m11 = 1.0f / (ar * tan_half_fov);
	this->m22 = 1.0f / tan_half_fov;
	this->m33 = (far + near) / (near - far);
	this->m34 = -1.0f;
	this->m43 = -(2.0f * far * near) / (near - far);
	
	this->m41 = w / 2;
	this->m42 = h / 2;
}

void Matrix4::CreateTranslation(float x, float y, float z)
{
	this->m11 = 1.0f;
	this->m22 = 1.0f;
	this->m33 = 1.0f;
	this->m44 = 1.0f;

	this->m41 = x;
	this->m42 = y;
	this->m43 = z;
}

void Matrix4::CreateScale(float x, float y, float z)
{
	this->m11 = x;
	this->m22 = y;
	this->m33 = z;
	this->m44 = 1.0f;
}

void Matrix4::CreateRotationX(float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);

	this->m22 = c;
	this->m23 = -s;
	this->m32 = s;
	this->m33 = c;

	this->m11 = 1.0f;
	this->m44 = 1.0f;
}

void Matrix4::CreateRotationY(float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);

	this->m11 = c;
	this->m13 = s;
	this->m31 = -s;
	this->m33 = c;

	this->m22 = 1.0f;
	this->m44 = 1.0f;
}

void Matrix4::CreateRotationZ(float angle)
{
	float c = cosf(angle);
	float s = sinf(angle);

	this->m11 = c;
	this->m12 = -s;
	this->m21 = s;
	this->m22 = c;

	this->m33 = 1.0f;
	this->m44 = 1.0f;
}

void Matrix4::Multiply(Matrix4 &b)
{
	Matrix4 a = *this;
	this->m11 = a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31 + a.m14 * b.m41;
	this->m21 = a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31 + a.m24 * b.m41;
	this->m31 = a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31 + a.m34 * b.m41;
	this->m41 = a.m41 * b.m11 + a.m42 * b.m21 + a.m43 * b.m31 + a.m44 * b.m41;

	this->m12 = a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32 + a.m14 * b.m42;
	this->m22 = a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32 + a.m24 * b.m42;
	this->m32 = a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32 + a.m34 * b.m42;
	this->m42 = a.m41 * b.m12 + a.m42 * b.m22 + a.m43 * b.m32 + a.m44 * b.m42;

	this->m13 = a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33 + a.m14 * b.m43;
	this->m23 = a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33 + a.m24 * b.m43;
	this->m33 = a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33 + a.m34 * b.m43;
	this->m43 = a.m41 * b.m13 + a.m42 * b.m23 + a.m43 * b.m33 + a.m44 * b.m43;

	this->m14 = a.m11 * b.m14 + a.m12 * b.m24 + a.m13 * b.m34 + a.m14 * b.m44;
	this->m24 = a.m21 * b.m14 + a.m22 * b.m24 + a.m23 * b.m34 + a.m24 * b.m44;
	this->m34 = a.m31 * b.m14 + a.m32 * b.m24 + a.m33 * b.m34 + a.m34 * b.m44;
	this->m44 = a.m41 * b.m14 + a.m42 * b.m24 + a.m43 * b.m34 + a.m44 * b.m44;
}
