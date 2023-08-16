#include "renderer.h"

Renderer::Renderer(sf::RenderTarget& render_target): render_target {render_target} {
	circle_shape.setFillColor(sf::Color::Black);
}

template<typename T, typename K>
inline bool isType(const K &k) {
    return typeid(T).hash_code() == typeid(k).hash_code();
}

void Renderer::render(const Solver& solver) {
	render_target.setView(sf::View {{0, 0}, {SCALE * 16, SCALE * 9}});
	render_target.clear(sf::Color::White);
	for (const auto& body : solver.bodies) {
		// Configure position and size
		circle_shape.setPosition(body->position.x - body->radius, body->position.y - body->radius);
		circle_shape.setRadius(body->radius);

		// Configure color
		float velocity = body->get_velocity().length() * 165;
		sf::Uint8 red_component = velocity > 5.0f ? 255 : (velocity * 255 / 5);
		circle_shape.setFillColor(sf::Color {red_component, 0, static_cast<sf::Uint8>(255u - red_component)});

		render_target.draw(circle_shape);

		for (const auto& constraint : body->constraints) {
			if (!isType<BodyDistanceConstraint>(*constraint))
				continue;
			const auto bdc = std::static_pointer_cast<BodyDistanceConstraint, Constraint>(constraint);
			if (bdc->other_body == nullptr)
				continue;
			sf::Vertex lines[] = {
				sf::Vertex {sf::Vector2f {body->position.x, body->position.y}, sf::Color::Black},
				sf::Vertex {sf::Vector2f {bdc->other_body->position.x, bdc->other_body->position.y}, sf::Color::Black}
			};
			render_target.draw(lines, 2, sf::Lines);
		}
	}
	render_target.setView(render_target.getDefaultView());
}