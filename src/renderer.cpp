#include "renderer.h"

Renderer::Renderer(sf::RenderTarget& render_target): render_target {render_target} {
	render_target.setView(sf::View {{0, 0}, {SCALE * 16, SCALE * 9}});
	circle_shape.setFillColor(sf::Color::Black);
}

void Renderer::render(const Solver& solver) {
	for (const auto& body : solver.bodies) {
		circle_shape.setPosition(body->position.x - body->radius, body->position.y - body->radius);
		circle_shape.setRadius(body->radius);
		render_target.draw(circle_shape);
	}
}