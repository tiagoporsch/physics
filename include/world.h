#pragma once

#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "constraint.h"
#include "particle.h"
#include "renderer.h"
#include "vec2.h"

struct Cell {
	static constexpr auto CAPACITY = 8;

	uint32_t particle_ids[CAPACITY] = {};
	uint32_t particle_count = 0;

	void add(uint32_t id) {
		if (particle_count == CAPACITY) {
			std::cerr << "Cell full!\n";
			return;
		}
		particle_ids[particle_count++] = id;
	}

	void clear() {
		particle_count = 0;
	}
};

struct Grid {
	uint32_t width;
	uint32_t height;

	std::vector<Cell> cells;

	Grid(uint32_t width, uint32_t height): width {width}, height {height} {
		cells.resize(width * height);
	}

	Cell& get(uint32_t x, uint32_t y) {
		if (x >= width || y >= height) {
			std::cerr << "Grid::get(" << x << ", " << y << ")" << std::endl; 
			return cells[width + 1];
		}
		return cells[x + width * y];
	}

	void clear() {
		for (auto& cell : cells) {
			cell.clear();
		}
	}
};

struct World {
	static constexpr auto WIDTH = static_cast<float>(Renderer::WIDTH) / static_cast<float>(Renderer::SCALE);
	static constexpr auto HEIGHT = static_cast<float>(Renderer::HEIGHT) / static_cast<float>(Renderer::SCALE);
	static constexpr auto DELTA_TIME = 1.f / 165.f;

	Grid grid;
	std::vector<std::shared_ptr<Particle>> particles;

	std::shared_ptr<AccelerationConstraint> gravity;
	std::shared_ptr<LineDistanceConstraint> vertical_bounds;
	std::shared_ptr<LineDistanceConstraint> horizontal_bounds;

	World();

	void add_particle(Particle p);

	void collide_particles(uint32_t particle1_id, uint32_t particle2_id);
	void collide_particle_cell(uint32_t particle_id, uint32_t cell_id);
	void update(float dt);
};