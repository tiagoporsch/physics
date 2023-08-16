#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "renderer.h"
#include "solver.h"

struct Experiment {
	uint64_t frame = 0;
	void (*init)(Solver&);
	void (*step)(Solver&);
};

static const Experiment e1 = {
	.init = [](Solver& solver) {
		// Make a chain
		{
			float link_distance = 0.2f;
			float stiffness = 1.0f;
			float max_deformation = 2.0f;
			int links = 40;
			for (int i = 0; i < links; i++) {
				Body b {{link_distance * i - link_distance * links / 2, -7}};
				if (i == 0 || i == links - 1)
					b.add_constraint(std::make_shared<PointDistanceConstraint>(PointDistanceConstraint::Type::FIXED, Vec2 {link_distance * i - link_distance * links / 2, -7}));
				if (i != 0)
					b.add_constraint(std::make_shared<BodyDistanceConstraint>(solver.bodies.back(), link_distance, stiffness, max_deformation));
				solver.add_body(b);
			}
		}

		// Make a tower
		{
			float body_radius = 0.1f;
			float body_distance = 1.0f;
			float stiffness = 0.25f;
			float max_deformation = 1.05f;
			int height = 8;
			for (int i = 0; i < height; i++) {	
				if (i == 0) {
					solver.add_body(Body {{ 0.5f * body_distance, 8.0f - body_radius - (float) i}});
					solver.add_body(Body {{-0.5f * body_distance, 8.0f - body_radius - (float) i}}
						.add_constraint(std::make_shared<BodyDistanceConstraint>(solver.bodies[solver.bodies.size() - 1], body_distance, stiffness, max_deformation))
					);
				} else {
					solver.add_body(Body {{ 0.5f * body_distance, 8.0f - body_radius - (float) i}}
						.add_constraint(std::make_shared<BodyDistanceConstraint>(solver.bodies[solver.bodies.size() - 2], body_distance, stiffness, max_deformation))
					);
					solver.add_body(Body {{-0.5f * body_distance, 8.0f - body_radius - (float) i}}
						.add_constraint(std::make_shared<BodyDistanceConstraint>(solver.bodies[solver.bodies.size() - 1], body_distance, stiffness, max_deformation))
						.add_constraint(std::make_shared<BodyDistanceConstraint>(solver.bodies[solver.bodies.size() - 2], body_distance, stiffness, max_deformation))
						.add_constraint(std::make_shared<BodyDistanceConstraint>(solver.bodies[solver.bodies.size() - 3], fsqrt(2) * body_distance, stiffness, max_deformation))
					);
				}
			}
		}
	},
	.step = [](Solver& solver) {
		static uint64_t frame = 0;
		if ((frame < 109 * (165 / 5)) && (++frame % (165 / 5) == 0)) {
			solver.add_body(Body {{0, -8}, {0, 0.2f / (165.f / 5.f)}});
		}
	}
};

static const Experiment e2 = {
	.init = [](Solver& solver) {
	},
	.step = [](Solver& solver) {
		static uint64_t frame = 0;
		// if (++frame % 1 == 0) {
			solver.add_body(Body {{-14, -8}, {0.2f, 0.0f}});
		// }
	}
};

int main() {
	sf::RenderWindow window {{Renderer::WIDTH, Renderer::HEIGHT}, "Physics"};

	sf::Font font;
	font.loadFromFile("RobotoMono-Regular.ttf");

	sf::Text step_time_text;
	step_time_text.setFont(font);
	step_time_text.setCharacterSize(12);
	step_time_text.setFillColor(sf::Color::Black);

	sf::Text body_count_text;
	body_count_text.setFont(font);
	body_count_text.setCharacterSize(12);
	body_count_text.setFillColor(sf::Color::Black);
	body_count_text.setPosition({0, 12});

	sf::Clock clock;
	sf::Time accumulator = sf::Time::Zero;
	sf::Time ups = sf::seconds(1.f / 165.f);

	Renderer renderer { window };
	Solver solver;
	auto& experiment = e1;

	experiment.init(solver);
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

		experiment.step(solver);
		solver.update(ups.asSeconds());
		body_count_text.setString(std::to_string(solver.bodies.size()) + " bodies");

		renderer.render(solver);
		window.draw(step_time_text);
		window.draw(body_count_text);
		window.display();

		auto elapsed = clock.restart();
		step_time_text.setString(std::to_string(elapsed.asMicroseconds()) + " us");
		if (elapsed < ups) {
			step_time_text.setFillColor(sf::Color::Black);
			sf::sleep(ups - elapsed);
		} else {
			step_time_text.setFillColor(sf::Color::Red);
		}
	}

	return EXIT_SUCCESS;
}