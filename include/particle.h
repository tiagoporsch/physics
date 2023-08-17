#pragma once

#include <memory>
#include <vector>

#include "constraint.h"
#include "vec2.h"

struct Particle {
	static constexpr auto RADIUS = 1.f;

	Vec2 position = {};
	Vec2 position_last = {};
	Vec2 acceleration = {};

	std::vector<std::shared_ptr<Constraint>> constraints;

	Particle(Vec2 position = {}, Vec2 velocity = {});

	Particle& add_constraint(std::shared_ptr<Constraint> constraint);

	void apply_constraints();
	void update_position(float dt);

	Vec2 get_velocity() const;
};