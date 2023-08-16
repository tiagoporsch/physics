#pragma once

#include <cmath>
#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "body.h"
#include "grid.h"
#include "vec2.h"

struct Solver {
	Grid grid;
	std::vector<std::shared_ptr<Body>> bodies;

	Solver();

	void solve_contact(uint32_t id1, uint32_t id2);
	void process_body(uint32_t id, const Cell& cell);
	void update(float dt);
};