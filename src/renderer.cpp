#include "renderer.h"

#include "solver.h"

Renderer::Renderer(sf::RenderTarget& render_target): render_target {render_target} {
}

template<typename T, typename K>
inline bool isType(const K &k) {
    return typeid(T).hash_code() == typeid(k).hash_code();
}

void Renderer::render(const World& solver) {
	render_target.setView(sf::View { sf::FloatRect {0, 0, World::WIDTH, World::HEIGHT}});
	render_target.clear(sf::Color::Black);

	for (const auto& body : solver.bodies) {
		for (const auto& constraint : body->constraints) {
			if (!isType<BodyDistanceConstraint>(*constraint))
				continue;

			const auto bdc = std::static_pointer_cast<BodyDistanceConstraint, Constraint>(constraint);
			if (bdc->other_body == nullptr)
				continue;

			const auto deformation = fabsf((body->position - bdc->other_body->position).length() - bdc->target_distance) / bdc->target_distance;
			sf::Uint8 red_component = static_cast<sf::Uint8>(255 * (deformation / (bdc->max_deformation - 1)));
			sf::Color color {red_component, 0, 255 - red_component};

			sf::Vertex lines[] = {
				sf::Vertex {sf::Vector2f {body->position.x, body->position.y}, color},
				sf::Vertex {sf::Vector2f {bdc->other_body->position.x, bdc->other_body->position.y}, color}
			};
			render_target.draw(lines, 2, sf::Lines);
		}
	}

	for (const auto& body : solver.bodies) {
		circle_shape.setPosition(body->position.x - body->radius, body->position.y - body->radius);
		circle_shape.setRadius(body->radius);

		float velocity = body->get_velocity().length() * 165;
		sf::Uint8 red_component = velocity > 64.0f ? 255 : (velocity * 255 / 64);
		sf::Uint8 green_component = velocity < 64.f ? 0 : (velocity > 128.0f ? 255 : ((velocity - 64.f) * 255 / 64));
		circle_shape.setFillColor(sf::Color {red_component, green_component, static_cast<sf::Uint8>(255u - red_component)});

		render_target.draw(circle_shape);
	}

	render_target.setView(render_target.getDefaultView());
}