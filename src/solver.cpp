#include "solver.h"

Solver::Solver() {
	gravity = std::make_shared<AccelerationConstraint>(Vec2 {0, 9.81f});
	vertical_bounds = std::make_shared<LineDistanceConstraint>(LineDistanceConstraint::Type::MINIMUM, Vec2 {0, 9}, 0, 1);
	horizontal_bounds = std::make_shared<LineDistanceConstraint>(LineDistanceConstraint::Type::MAXIMUM, Vec2 {0, 0}, M_PI_2, 15);
}

void Solver::add_body(Body body) {
	body.add_constraint(gravity);
	body.add_constraint(vertical_bounds);
	body.add_constraint(horizontal_bounds);
	bodies.push_back(std::make_shared<Body>(body));
}

void Solver::update(float dt) {
	// Apply constraints
	for (auto& body : bodies) {
		body->apply_constraints();
	}

	// Solve collisions
	const auto body_count = bodies.size();
	for (std::size_t i = 0; i < body_count; i++) {
		auto& body1 = bodies[i];
		for (std::size_t j = i + 1; j < body_count; j++) {
			auto& body2 = bodies[j];
			const auto collision_axis = body1->position - body2->position;
			const auto dist = collision_axis.length();
			const auto radii = body1->radius + body2->radius;
			if (dist < radii) {
				const auto n = collision_axis / dist;
				const auto delta = radii - dist;
				body1->position += 0.5f * delta * n;
				body2->position -= 0.5f * delta * n;
			}
		}
	}

	// Update positions
	for (auto& body : bodies) {
		body->update_position(dt);
	}
}