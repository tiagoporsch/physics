#include "constraint.h"

#include "body.h"

void AccelerationConstraint::apply(Body& body) {
	body.accelerate(acceleration);
}

void BodyDistanceConstraint::apply(Body& body) {
	auto axis = body.position - other_body->position;
	auto distance = axis.length();
	if (distance > max_deformation * target_distance) {
		broken = true;
		return;
	}
	axis /= distance;
	auto correction = 0.5f * stiffness * (distance - target_distance) * axis;
	body.position -= correction;
	other_body->position += correction;
}

void PointDistanceConstraint::apply(Body& body) {
	if (type == Type::FIXED) {
		body.position = point;
	} else {
		auto axis = body.position - point;
		const auto dist = axis.length();
		axis /= dist;
		if (type == Type::MAXIMUM && dist > distance - body.radius) {
			body.position = axis * (distance - body.radius);
		} else if (type == Type::MINIMUM && dist < distance + body.radius) {
			body.position = axis * (distance + body.radius);
		}
	}
}

template <typename T> static int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

void LineDistanceConstraint::apply(Body& body) {
	const auto axis = Vec2 {-sinf(angle), cosf(angle)};
	const auto dist = axis.y * (point.y - body.position.y) + axis.x * (point.x - body.position.x);
	const auto abs_dist = fabsf(dist);
	if (type == Type::FIXED) {
		body.position -= axis * (target_distance - abs_dist) * sgn(dist);
	} else if (type == Type::MAXIMUM && abs_dist > target_distance - body.radius) {
		body.position -= axis * (target_distance - abs_dist - body.radius) * sgn(dist);
	} else if (type == Type::MINIMUM && abs_dist < target_distance + body.radius) {
		body.position -= axis * (target_distance - abs_dist + body.radius) * sgn(dist);
	}
}