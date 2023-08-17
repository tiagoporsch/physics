#include "body.h"

#include <algorithm>

Body::Body(Vec2 position, Vec2 velocity): position {position}, position_last {position - velocity} {}

Body& Body::add_constraint(std::shared_ptr<Constraint> constraint) {
	constraints.push_back(constraint);
	return *this;
}

void Body::apply_constraints() {
	for (auto& constraint : constraints) {
		constraint->apply(*this);
	}

	// Remove broken constraints
	constraints.erase(
		std::remove_if(constraints.begin(), constraints.end(), [](auto& c) { return c->is_broken(); }),
		constraints.end()
	);
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

Vec2 Body::get_velocity() const {
	return position_last - position;
}