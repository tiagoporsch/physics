#include "grid.h"

void Cell::add(uint32_t id) {
	bodies[body_count] = id;
	body_count += body_count < CAPACITY;
}

void Cell::remove(uint32_t id) {
	for (uint32_t i = 0; i < body_count; i++) {
		if (bodies[i] == id) {
			bodies[i] = bodies[body_count - 1];
			body_count--;
			return;
		}
	}
}

void Cell::clear() {
	body_count = 0;
}

Grid::Grid(): width {0}, height {0} {}
Grid::Grid(uint32_t width, uint32_t height): width {width}, height {height} {
	cells.resize(width * height);
};

void Grid::add(uint32_t x, uint32_t y, uint32_t id) {
	cells[x * height + y].add(id);
}

void Grid::clear() {
	for (auto& cell : cells) {
		cell.clear();
	}
}