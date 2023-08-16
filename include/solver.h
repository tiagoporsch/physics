#pragma once

#include <cmath>
#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "constraint.h"
#include "body.h"
#include "vec2.h"

struct Solver {
	std::vector<std::shared_ptr<Body>> bodies;

	std::shared_ptr<AccelerationConstraint> gravity;
	std::shared_ptr<LineDistanceConstraint> vertical_bounds;
	std::shared_ptr<LineDistanceConstraint> horizontal_bounds;

	Solver();

	void add_body(Body body);
	void update(float dt);
};