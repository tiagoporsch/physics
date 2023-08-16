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

static const Experiment e2 = {
	.init = [](Solver& solver) {
	},
	.step = [](Solver& solver) {
		static uint64_t frame = 0;
		if (++frame % 4 == 0) {
			solver.bodies.push_back(std::make_shared<Body>(
				Vec2 { 2.f, 2.f }, Vec2 { .3f, 0.f }
			));
		}
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
	auto& experiment = e2;

	experiment.init(solver);
	while (window.isOpen()) {
		clock.restart();
		for (auto event = sf::Event(); window.pollEvent(event);) {
			switch (event.type) {
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::MouseButtonReleased:
					solver.bodies.push_back(std::make_shared<Body>(
						Vec2 {
							sf::Mouse::getPosition(window).x / (float) Renderer::SCALE,
							sf::Mouse::getPosition(window).y / (float) Renderer::SCALE
						},
						Vec2 { 1.f, 0.f }
					));
					break;
			}
		}

		experiment.step(solver);
		solver.update(ups.asSeconds() / 3.f);
		solver.update(ups.asSeconds() / 3.f);
		solver.update(ups.asSeconds() / 3.f);
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