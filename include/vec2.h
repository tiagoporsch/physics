#pragma once

#include <cmath>

struct Vec2 {
	float x, y;

	Vec2(): x {0}, y {0} {}
	Vec2(float x, float y): x {x}, y {y} {}
	Vec2(const Vec2& v): x {v.x}, y {v.y} {}

	float length() const;
	Vec2& normalize();
};

Vec2 operator-(const Vec2& right);
Vec2& operator+=(Vec2& left, const Vec2& right);
Vec2& operator-=(Vec2& left, const Vec2& right);
Vec2 operator+(const Vec2& left, const Vec2& right);
Vec2 operator-(const Vec2& left, const Vec2& right);
Vec2 operator*(const Vec2& left, float right);
Vec2 operator*(float left, const Vec2& right);
Vec2& operator*=(Vec2& left, float right);
Vec2 operator/(const Vec2& left, float right);
Vec2& operator/=(Vec2& left, float right);
bool operator==(const Vec2& left, const Vec2& right);
bool operator!=(const Vec2& left, const Vec2& right);