#include "solver.h"

#include <cstdio>

#include "renderer.h"

static constexpr float WIDTH = (float) Renderer::WIDTH / (float) Renderer::SCALE;
static constexpr float HEIGHT = (float) Renderer::HEIGHT / (float) Renderer::SCALE;

Solver::Solver(): grid {Renderer::WIDTH / Renderer::SCALE, Renderer::HEIGHT / Renderer::SCALE} {
}

void Solver::solve_contact(uint32_t id1, uint32_t id2) {
	if (id1 == id2)
		return;
	const auto& body1 = bodies[id1];
	const auto& body2 = bodies[id2];
	const auto collision_axis = body1->position - body2->position;
	const auto dist_sq = collision_axis.length_sq();
	if (dist_sq < 4.0f && dist_sq >= 1e-4) {
		const float dist = sqrt(dist_sq);
		const auto delta = 0.1f * (2.0f - dist);
		const auto response = delta * collision_axis / dist;
		body1->position += response;
		body2->position -= response;
	}
}

void Solver::process_body(uint32_t id, const Cell& cell) {
	for (uint32_t i = 0; i < cell.body_count; i++) {
		solve_contact(id, cell.bodies[i]);
	}
}

void Solver::update(float dt) {
	// Update grid
	grid.clear();
	uint32_t id = 0;
	for (const auto& body : bodies) {
		if (
			body->position.x > 1.f && body->position.x < WIDTH - 1.f &&
			body->position.y > 1.f && body->position.y < HEIGHT - 1.f) {
			grid.add(static_cast<uint32_t>(body->position.x), static_cast<uint32_t>(body->position.y), id);
		}
		id++;
	}

	// Solve collisions
	for (uint32_t i = 0; i < grid.cells.size(); i++) {
		const auto& cell = grid.cells[i];
		for (uint32_t j = 0; j < cell.body_count; j++) {
			const auto id = cell.bodies[j];
			process_body(id, grid.cells[i - 1]);
            process_body(id, grid.cells[i]);
            process_body(id, grid.cells[i + 1]);
            process_body(id, grid.cells[i + grid.height - 1]);
            process_body(id, grid.cells[i + grid.height    ]);
            process_body(id, grid.cells[i + grid.height + 1]);
            process_body(id, grid.cells[i - grid.height - 1]);
            process_body(id, grid.cells[i - grid.height    ]);
            process_body(id, grid.cells[i - grid.height + 1]);
		}
	}

	// Update positions
	for (auto& body : bodies) {
		body->acceleration.y += 100.f;

		body->update_position(dt);

		const float margin = 2.0f;
		if (body->position.x > WIDTH - margin) {
			body->position.x = WIDTH - margin;
		} else if (body->position.x < margin) {
			body->position.x = margin;
		}
		if (body->position.y > HEIGHT - margin) {
			body->position.y = HEIGHT - margin;
		} else if (body->position.y < margin) {
			body->position.y = margin;
		}
	}
}