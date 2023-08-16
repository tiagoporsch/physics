#pragma once

#include <cstdint>
#include <vector>

struct Cell {
	static constexpr uint8_t CAPACITY = 4;

	uint32_t body_count = 0;
	uint32_t bodies[CAPACITY] = {};

	void add(uint32_t id);
	void remove(uint32_t id);
	void clear();
};

struct Grid {
	uint32_t width, height;
	std::vector<Cell> cells;

	Grid();
	Grid(uint32_t width, uint32_t height);

	void add(uint32_t x, uint32_t y, uint32_t id);
	void clear();
};