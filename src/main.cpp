#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "renderer.h"
#include "solver.h"

int main() {
	auto window = sf::RenderWindow({Renderer::WIDTH, Renderer::HEIGHT}, "Physics");
	window.setFramerateLimit(165);

	Renderer renderer { window };
	Solver solver;

	// Make a chain
	int n = 40;
	float dx = 0.2f;
	float x = 0;
	for (int i = 0; i < n; i++) {
		Body b {{x, 0}};
		if (i == 0)
			b.add_constraint(std::make_shared<PointDistanceConstraint>(PointDistanceConstraint::Type::FIXED, Vec2 {x, 0}));
		if (i != 0)
			b.add_constraint(std::make_shared<BodyDistanceConstraint>(solver.bodies.back(), 0.2f));
		solver.add_body(b);
		x += dx;
	}

	while (window.isOpen()) {
		for (auto event = sf::Event(); window.pollEvent(event);) {
			switch (event.type) {
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::MouseButtonReleased:
					solver.add_body(Body {{
						Renderer::SCALE * 16 * (sf::Mouse::getPosition(window).x - Renderer::WIDTH / Renderer::SCALE) / (float) Renderer::WIDTH,
						Renderer::SCALE * 9 * (sf::Mouse::getPosition(window).y - Renderer::HEIGHT / Renderer::SCALE) / (float) Renderer::HEIGHT
					}});
					break;
			}
		}

		solver.update(1.0f / 165.0f);

		window.clear(sf::Color::White);
		renderer.render(solver);
		window.display();
	}

	return EXIT_SUCCESS;
}