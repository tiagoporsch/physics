#include "constraint.h"

#include "particle.h"

void AccelerationConstraint::apply(Particle& p) {
	p.acceleration += acceleration;
}

void ParticleDistanceConstraint::apply(Particle& p) {
	auto axis = p.position - other_p->position;
	auto distance = axis.length();
	if (distance > max_deformation * target_distance) {
		broken = true;
		return;
	}
	axis /= distance;
	auto correction = 0.5f * stiffness * (distance - target_distance) * axis;
	p.position -= correction;
	other_p->position += correction;
}

void PointDistanceConstraint::apply(Particle& p) {
	if (type == Type::FIXED) {
		p.position = point;
	} else {
		auto axis = p.position - point;
		const auto dist = axis.length();
		axis /= dist;
		if (type == Type::MAXIMUM && dist > distance - Particle::RADIUS) {
			p.position = axis * (distance - Particle::RADIUS);
		} else if (type == Type::MINIMUM && dist < distance + Particle::RADIUS) {
			p.position = axis * (distance + Particle::RADIUS);
		}
	}
}

template <typename T> static int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

void LineDistanceConstraint::apply(Particle& p) {
	const auto axis = Vec2 {-sinf(angle), cosf(angle)};
	const auto dist = axis.y * (point.y - p.position.y) + axis.x * (point.x - p.position.x);
	const auto abs_dist = fabsf(dist);
	if (type == Type::FIXED) {
		p.position -= axis * (target_distance - abs_dist) * sgn(dist);
	} else if (type == Type::MAXIMUM && abs_dist > target_distance - Particle::RADIUS) {
		p.position -= axis * (target_distance - abs_dist - Particle::RADIUS) * sgn(dist);
	} else if (type == Type::MINIMUM && abs_dist < target_distance + Particle::RADIUS) {
		p.position -= axis * (target_distance - abs_dist + Particle::RADIUS) * sgn(dist);
	}
}