#include "vec2.h"

float Vec2::length() const { return sqrt(x * x + y * y); }
Vec2& Vec2::normalize() { return *this /= length(); };

Vec2 operator-(const Vec2& right) { return { -right.x, -right.y }; }
Vec2& operator+=(Vec2& left, const Vec2& right) { left.x += right.x; left.y += right.y; return left; }
Vec2& operator-=(Vec2& left, const Vec2& right) { left.x -= right.x; left.y -= right.y; return left; }
Vec2 operator+(const Vec2& left, const Vec2& right) { return { left.x + right.x, left.y + right.y }; }
Vec2 operator-(const Vec2& left, const Vec2& right) { return { left.x - right.x, left.y - right.y }; }
Vec2 operator*(const Vec2& left, float right) { return { left.x * right, left.y * right }; }
Vec2 operator*(float left, const Vec2& right) { return { left * right.x, left * right.y }; }
Vec2& operator*=(Vec2& left, float right) { left.x *= right; left.y *= right; return left; }
Vec2 operator/(const Vec2& left, float right) { return { left.x / right, left.y / right }; }
Vec2& operator/=(Vec2& left, float right) { left.x /= right; left.y /= right; return left; }
bool operator==(const Vec2& left, const Vec2& right) { return left.x == right.x && left.y == right.y; }
bool operator!=(const Vec2& left, const Vec2& right) { return left.x != right.x || left.y != right.y; }