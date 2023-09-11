#include "Vector3.h"

#include <cmath>

Vector3::Vector3()
{
	x_ = 0;
	y_ = 0;
	z_ = 0;
}

Vector3::Vector3(float x, float y, float z) : x_(x), y_(y), z_(z) {}

void Vector3::operator=(Vector3 other)
{
	x_ = other.x_;
	y_ = other.y_;
	z_ = other.z_;
}

bool Vector3::operator==(Vector3 other)
{
	return (x_ == other.x_ && y_ == other.y_ && z_ == other.z_);
}

Vector3 Vector3::operator+(Vector3 other)
{
	return Vector3(x_ + other.x_, y_ + other.y_, z_ + other.z_);
}

Vector3 Vector3::operator-(Vector3 other)
{
	return Vector3(x_ - other.x_, y_ - other.y_, z_ - other.z_);
}

Vector3 Vector3::operator*(float scalar)
{
	return Vector3(x_ * scalar, y_ * scalar, z_ * scalar);
}

Vector3 Vector3::operator/(float divisor)
{
	return Vector3(x_ / divisor, y_ / divisor, z_ / divisor);
}

void Vector3::operator+=(Vector3 other)
{
	x_ += other.x_;
	y_ += other.y_;
	z_ += other.z_;
}

Vector3 Vector3::Add(Vector3 other)
{
	return Vector3(x_ + other.x_, y_ + other.y_, z_ + other.z_);
}

Vector3 Vector3::Subtract(Vector3 other)
{
	return Vector3(x_ - other.x_, y_ - other.y_, z_ - other.z_);
}

Vector3 Vector3::Scale(float scalar)
{
	return Vector3(x_ * scalar, y_ * scalar, z_ * scalar);
}

Vector3 Vector3::Normalise()
{
	float magnitude = Magnitude();
	return Vector3(x_ / magnitude, y_ / magnitude, z_ / magnitude);
}

float Vector3::Magnitude()
{
	return sqrt(x_ * x_ + y_ * y_ + z_ * z_);
}

float Vector3::DotProduct(Vector3 other)
{
	return (x_ * other.x_ + y_ * other.y_ + z_ * other.z_);
}

Vector3 Vector3::CrossProduct(Vector3 other)
{
	float x = y_ * other.z_ - z_ * other.y_;
	float y = z_ * other.x_ - x_ * other.z_;
	float z = x_ * other.y_ - y_ * other.x_;
	return Vector3(x, y, z);
}

float Vector3::AngleBetween(Vector3 other, Vector3 normal)
{
	Vector3 a = *this;
	Vector3 b = other;
	normal = normal.Normalise();
	return atan2(CrossProduct(other).DotProduct(normal), DotProduct(other));
}
