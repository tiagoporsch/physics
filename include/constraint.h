#pragma once

#include <memory>

#include "vec2.h"

// Base class
class Body;
class Constraint {
protected:
	bool broken = false;
public:
	virtual void apply(Body& body) {};
	bool is_broken() const { return broken; }
};

// Constant acceleration
class AccelerationConstraint : public Constraint {
	Vec2 acceleration;
public:
	AccelerationConstraint(Vec2 acceleration): acceleration {acceleration} {}
	void apply(Body& body);
};

// Distance to body
class BodyDistanceConstraint : public Constraint {
	float target_distance;
	float stiffness;
	float max_deformation;
public:
	std::shared_ptr<Body> other_body;
	BodyDistanceConstraint(std::shared_ptr<Body> other_body, float distance, float stiffness = 1.0f, float max_deformation = 1e10f):
		other_body {other_body}, target_distance {distance}, stiffness {stiffness}, max_deformation {max_deformation} {}
	void apply(Body& body);
};

// Distance to point
class PointDistanceConstraint : public Constraint {
public:
	enum class Type { FIXED, MINIMUM, MAXIMUM };
private:
	Type type;
	Vec2 point;
	float distance;
public:
	PointDistanceConstraint(Type type, Vec2 point, float distance = 0): type {type}, point {point}, distance {distance} {}
	void apply(Body& body);
};

// Distance to line
class LineDistanceConstraint : public Constraint {
public:
	enum class Type { FIXED, MINIMUM, MAXIMUM };
private:
	Type type;
	Vec2 point;
	float angle;
	float target_distance;
public:
	LineDistanceConstraint(Type type, Vec2 point, float angle, float distance): type {type}, point {point}, angle {angle}, target_distance {distance} {}
	void apply(Body& body);
};