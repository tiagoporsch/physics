#include "renderer.h"

#include "world.h"

Renderer::Renderer(sf::RenderTarget& render_target): render_target {render_target} {
	circle_shape.setRadius(Particle::RADIUS);
}

template<typename T, typename K>
static inline bool isType(const K &k) {
    return typeid(T).hash_code() == typeid(k).hash_code();
}

void Renderer::render(const World& world) {
	render_target.setView(sf::View { sf::FloatRect {0, 0, World::WIDTH, World::HEIGHT}});
	render_target.clear(sf::Color::Black);

	for (const auto& p : world.particles) {
		for (const auto& constraint : p->constraints) {
			if (!isType<ParticleDistanceConstraint>(*constraint))
				continue;

			const auto bdc = std::static_pointer_cast<ParticleDistanceConstraint, Constraint>(constraint);
			if (bdc->other_p == nullptr)
				continue;

			const auto deformation = fabsf((p->position - bdc->other_p->position).length() - bdc->target_distance) / bdc->target_distance;
			const auto stress = deformation / (bdc->max_deformation - 1.f);
			sf::Color color {static_cast<sf::Uint8>(255 * stress), 0, static_cast<sf::Uint8>(255 * (1.f - stress))};

			sf::Vertex lines[] = {
				sf::Vertex {sf::Vector2f {p->position.x, p->position.y}, color},
				sf::Vertex {sf::Vector2f {bdc->other_p->position.x, bdc->other_p->position.y}, color}
			};
			render_target.draw(lines, 2, sf::Lines);
		}
	}

	for (const auto& p : world.particles) {
		circle_shape.setPosition(p->position.x - Particle::RADIUS, p->position.y - Particle::RADIUS);

		const float velocity = p->get_velocity().length() / World::DELTA_TIME;
		const sf::Uint8 red_component = velocity > 64.0f ? 255 : (velocity * 255 / 64);
		const sf::Uint8 green_component = velocity < 64.f ? 0 : (velocity > 128.0f ? 255 : ((velocity - 64.f) * 255 / 64));
		const sf::Uint8 blue_component = 255 - red_component;
		circle_shape.setFillColor(sf::Color {red_component, green_component, blue_component});

		render_target.draw(circle_shape);
	}

	// sf::RectangleShape s;
	// s.setSize({World::WIDTH / world.grid.width, World::HEIGHT / world.grid.height});
	// s.setOutlineThickness(1.f / SCALE);
	// s.setOutlineColor(sf::Color::White);
	// s.setFillColor(sf::Color::Transparent);
	// for (uint32_t i = 0; i < world.grid.cells.size(); i++) {
	// 	uint32_t x = i % world.grid.width;
	// 	uint32_t y = i / world.grid.width;
	// 	s.setPosition({x * s.getSize().x, y * s.getSize().y});
	// 	if (world.grid.cells[i].particle_count > 0)
	// 		render_target.draw(s);
	// }

	render_target.setView(render_target.getDefaultView());
}