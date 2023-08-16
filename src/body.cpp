#include "body.h"

Body::Body() {}
Body::Body(Vec2 position): position {position}, position_last {position} {}

Body& Body::add_constraint(std::shared_ptr<Constraint> constraint) {
	constraints.push_back(constraint);
	return *this;
}

void Body::apply_constraints() {
	for (auto& constraint : constraints) {
		constraint->apply(*this);
	}
}

void Body::update_position(float dt) {
	const auto velocity = position - position_last;
	position_last = position;
	position += velocity + acceleration * dt * dt;
	acceleration = {};
}

void Body::accelerate(Vec2 acc) {
	acceleration += acc;
}