#include "particle.h"

#include <algorithm>

Particle::Particle(Vec2 position, Vec2 velocity): position {position}, position_last {position - velocity} {}

Particle& Particle::add_constraint(std::shared_ptr<Constraint> constraint) {
	constraints.push_back(constraint);
	return *this;
}

void Particle::apply_constraints() {
	for (auto& constraint : constraints) {
		constraint->apply(*this);
	}

	// Remove broken constraints
	constraints.erase(
		std::remove_if(constraints.begin(), constraints.end(), [](auto& c) { return c->is_broken(); }),
		constraints.end()
	);
}

void Particle::update_position(float dt) {
	const auto velocity = position - position_last;
	position_last = position;
	position += velocity + acceleration * dt * dt;
	acceleration = {};
}

Vec2 Particle::get_velocity() const {
	return position_last - position;
}