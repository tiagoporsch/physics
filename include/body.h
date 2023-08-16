#pragma once

#include <memory>
#include <vector>

#include "constraint.h"
#include "vec2.h"

struct Body {
	Vec2 position = {};
	Vec2 position_last = {};
	Vec2 acceleration = {};
	float radius = 0.1f;

	std::vector<std::shared_ptr<Constraint>> constraints;

	Body();
	Body(Vec2 position);
	Body(Vec2 position, Vec2 velocity);

	Body& add_constraint(std::shared_ptr<Constraint> constraint);

	void apply_constraints();
	void update_position(float dt);
	void accelerate(Vec2 acc);

	Vec2 get_velocity() const;
};