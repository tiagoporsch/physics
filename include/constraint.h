#pragma once

#include <memory>

#include "vec2.h"

// Base class
class Particle;
class Constraint {
protected:
	bool broken = false;
public:
	virtual void apply(Particle& p) {};
	bool is_broken() const { return broken; }
};

// Constant acceleration
class AccelerationConstraint : public Constraint {
	Vec2 acceleration;
public:
	AccelerationConstraint(Vec2 acceleration): acceleration {acceleration} {}
	void apply(Particle& p);
};

// Distance to p
struct ParticleDistanceConstraint : public Constraint {
	float target_distance;
	float stiffness;
	float max_deformation;
	std::shared_ptr<Particle> other_p;

	ParticleDistanceConstraint(std::shared_ptr<Particle> other_p, float distance, float stiffness = 1.0f, float max_deformation = 1e10f):
		other_p {other_p}, target_distance {distance}, stiffness {stiffness}, max_deformation {max_deformation} {}
	void apply(Particle& p);
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
	void apply(Particle& p);
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
	void apply(Particle& p);
};