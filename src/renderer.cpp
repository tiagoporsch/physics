#include "renderer.h"

Renderer::Renderer(sf::RenderTarget& render_target): render_target {render_target} {
	circle_shape.setFillColor(sf::Color::Black);
	circle_shape.setRadius(1.0f);
}

template<typename T, typename K>
inline bool isType(const K &k) {
    return typeid(T).hash_code() == typeid(k).hash_code();
}

void Renderer::render(const Solver& solver) {
	render_target.setView(sf::View {sf::FloatRect {0, 0, WIDTH / SCALE, HEIGHT / SCALE}});
	render_target.clear(sf::Color::White);

	// Render bodies
	for (const auto& body : solver.bodies) {
		circle_shape.setPosition(body->position.x - 1.0f, body->position.y - 1.0f);

		float velocity = body->get_velocity().length() * 165;
		sf::Uint8 red_component = velocity > 100.0f ? 255 : (velocity * 255 / 100);
		circle_shape.setFillColor(sf::Color {red_component, 0, static_cast<sf::Uint8>(255u - red_component)});

		render_target.draw(circle_shape);
	}
	
	render_target.setView(render_target.getDefaultView());
}