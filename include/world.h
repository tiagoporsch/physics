#pragma once

#include <cmath>
#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "constraint.h"
#include "body.h"
#include "renderer.h"
#include "vec2.h"

struct World {
	static constexpr auto WIDTH = static_cast<float>(Renderer::WIDTH) / static_cast<float>(Renderer::SCALE);
	static constexpr auto HEIGHT = static_cast<float>(Renderer::HEIGHT) / static_cast<float>(Renderer::SCALE);
	static constexpr auto DELTA_TIME = 1.f / 165.f;

	std::vector<std::shared_ptr<Body>> bodies;

	std::shared_ptr<AccelerationConstraint> gravity;
	std::shared_ptr<LineDistanceConstraint> vertical_bounds;
	std::shared_ptr<LineDistanceConstraint> horizontal_bounds;

	World();

	void add_body(Body body);
	void update(float dt);
};