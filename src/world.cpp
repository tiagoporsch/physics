#include "world.h"

World::World(): grid {static_cast<uint32_t>(WIDTH / 2), static_cast<uint32_t>(HEIGHT / 2)} {
	gravity = std::make_shared<AccelerationConstraint>(Vec2 {0, 98.1f});
	vertical_bounds = std::make_shared<LineDistanceConstraint>(LineDistanceConstraint::Type::MINIMUM, Vec2 {0, HEIGHT}, 0, 2.f);
	horizontal_bounds = std::make_shared<LineDistanceConstraint>(LineDistanceConstraint::Type::MAXIMUM, Vec2 {WIDTH / 2, 0}, M_PI_2, WIDTH / 2 - 1.f);
}

void World::add_particle(Particle p) {
	// p.add_constraint(gravity);
	// p.add_constraint(vertical_bounds);
	// p.add_constraint(horizontal_bounds);
	particles.push_back(std::make_shared<Particle>(p));
}

void World::collide_particles(uint32_t p1_id, uint32_t p2_id) {
	if (p1_id == p2_id)
		return;
	auto& p1 = particles[p1_id];
	auto& p2 = particles[p2_id];
	const auto collision_axis = p1->position - p2->position;
	const auto dist = collision_axis.length();
	const auto radii = 2 * Particle::RADIUS;
	if (dist < radii && dist > 1e-3f) {
		const auto n = collision_axis / dist;
		const auto delta = radii - dist;
		p1->position += 0.5f * delta * n;
		p2->position -= 0.5f * delta * n;
	}
}

void World::collide_particle_cell(uint32_t particle_id, uint32_t cell_id) {
	if (cell_id > grid.cells.size())
		return;
	const auto& cell = grid.cells[cell_id];
	for (uint32_t particle_index = 0; particle_index < cell.particle_count; particle_index++) {
		collide_particles(particle_id, cell.particle_ids[particle_index]);
	}
}

void World::update(float dt) {
	// Apply constraints
	for (auto& p : particles) {
		p->apply_constraints();
	}

	// Update grid
	grid.clear();
	for (uint32_t particle_id = 0; particle_id < particles.size(); particle_id++) {
		const auto& body = particles[particle_id];
		grid.get(static_cast<uint32_t>(body->position.x) / 2, static_cast<uint32_t>(body->position.y) / 2).add(particle_id);
	}

	// Solve collisions
	for (uint32_t cell_id = 0; cell_id < grid.cells.size(); cell_id++) {
		const auto& cell = grid.cells[cell_id];
		for (uint32_t particle_index = 0; particle_index < cell.particle_count; particle_index++) {
			const auto particle_id = cell.particle_ids[particle_index];
			collide_particle_cell(particle_id, cell_id - grid.width - 1);
			collide_particle_cell(particle_id, cell_id - grid.width    );
			collide_particle_cell(particle_id, cell_id - grid.width + 1);
			collide_particle_cell(particle_id, cell_id              - 1);
			collide_particle_cell(particle_id, cell_id                 );
			collide_particle_cell(particle_id, cell_id              + 1);
			collide_particle_cell(particle_id, cell_id + grid.width - 1);
			collide_particle_cell(particle_id, cell_id + grid.width    );
			collide_particle_cell(particle_id, cell_id + grid.width + 1);
		}
	}

	// Update positions
	for (auto& p : particles) {
		// Apply gravity
		p->acceleration += {0.f, 98.1f};

		// Integrate!
		p->update_position(dt);

		// Check boundaries
		if (p->position.x < Particle::RADIUS)
			p->position.x = Particle::RADIUS;
		if (p->position.x > WIDTH - Particle::RADIUS)
			p->position.x = WIDTH - Particle::RADIUS;
		if (p->position.y < Particle::RADIUS)
			p->position.y = Particle::RADIUS;
		if (p->position.y > HEIGHT - Particle::RADIUS)
			p->position.y = HEIGHT - Particle::RADIUS;
	}
}