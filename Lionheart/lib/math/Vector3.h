#pragma once

struct Vector3
{
	Vector3();
	Vector3(float, float, float);

	float x_;
	float y_;
	float z_;

	void operator=(Vector3 other);
	bool operator==(Vector3 other);
	Vector3 operator+(Vector3 other);
	Vector3 operator-(Vector3 other);
	Vector3 operator*(float scalar);
	Vector3 operator/(float divisor);
	void operator+=(Vector3 other);

	Vector3 Add(Vector3 other);
	Vector3 Subtract(Vector3 other);
	Vector3 Scale(float scalar);
	Vector3 Normalise();
	float Magnitude();
	float DotProduct(Vector3 other);
	Vector3 CrossProduct(Vector3 other);
	float AngleBetween(Vector3 other, Vector3 normal);
};