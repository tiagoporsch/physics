#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "renderer.h"
#include "world.h"

static uint64_t frame = 0;

struct Experiment {
	void (*init)(World&);
	void (*step)(World&);
};

static const Experiment e1 = {
	.init = [](World& world) {
		// Make a chain
		{
			float link_distance = 2.0f;
			float stiffness = 1.0f;
			float max_deformation = 2.0f;
			int links = 35;
			for (int i = 0; i < links; i++) {
				Particle b {{World::WIDTH / 2 + link_distance * (i - links / 2), World::HEIGHT / 6}};
				if (i == 0 || i == links - 1)
					b.add_constraint(std::make_shared<PointDistanceConstraint>(PointDistanceConstraint::Type::FIXED, b.position));
				if (i != 0)
					b.add_constraint(std::make_shared<ParticleDistanceConstraint>(world.particles[world.particles.size() - 1], link_distance, stiffness, max_deformation));
				world.add_particle(b);
			}
		}

		// Make a tower
		{
			float p_radius = 1.f;
			float p_distance = 5.0f;
			float stiffness = .6f;
			float max_deformation = 1.05f;
			int width = 16, height = 32;
			for (int row = 0; row < height; row++) {	
				if (row == 0) {
					for (int col = 0; col < width; col++) {
						if (col == 0) {
							world.add_particle(Particle {{World::WIDTH / 2 + (col - width / 2) * p_distance, World::HEIGHT - p_distance * row - p_radius}});
						} else {
							world.add_particle(Particle {{World::WIDTH / 2 + (col - width / 2) * p_distance, World::HEIGHT - p_distance * row - p_radius}}
								.add_constraint(std::make_shared<ParticleDistanceConstraint>(world.particles[world.particles.size() - 1], p_distance, stiffness, max_deformation))
							);
						}
					}
				} else {
					for (int col = 0; col < width; col++) {
						if (col == 0) {
							world.add_particle(Particle {{World::WIDTH / 2 + (col - width / 2) * p_distance, World::HEIGHT - p_distance * row - p_radius}}
								.add_constraint(std::make_shared<ParticleDistanceConstraint>(world.particles[world.particles.size() - width], p_distance, stiffness, max_deformation))
							);
						} else {
							world.add_particle(Particle {{World::WIDTH / 2 + (col - width / 2) * p_distance, World::HEIGHT - p_distance * row - p_radius}}
								.add_constraint(std::make_shared<ParticleDistanceConstraint>(world.particles[world.particles.size() - 1], p_distance, stiffness, max_deformation))
								.add_constraint(std::make_shared<ParticleDistanceConstraint>(world.particles[world.particles.size() - width], p_distance, stiffness, max_deformation))
								.add_constraint(std::make_shared<ParticleDistanceConstraint>(world.particles[world.particles.size() - width - 1], fsqrt(2) * p_distance, stiffness, max_deformation))
							);
						}
					}
				}
			}
		}
	},
	.step = [](World& world) {
		if ((frame < 135 * (165 / 10)) && (++frame % (165 / 10) == 0)) {
			world.add_particle(Particle {
				{World::WIDTH / 2 - 2.f, World::HEIGHT / 10}, {0, 2.0f / (165.f / 10.f)},
			});
			world.add_particle(Particle {
				{World::WIDTH / 2, World::HEIGHT / 10}, {0, 2.0f / (165.f / 10.f)},
			});
			world.add_particle(Particle {
				{World::WIDTH / 2 + 2.f, World::HEIGHT / 10}, {0, 2.0f / (165.f / 10.f)},
			});
		}
	}
};

static const Experiment e2 = {
	.init = [](World& world) {
	},
	.step = [](World& world) {
		world.add_particle(Particle {{8.f, 8.f}, {2.f, 0.f}});
		world.add_particle(Particle {{8.f, 6.f}, {2.f, 0.f}});
	}
};

int main() {
	sf::RenderWindow window {{Renderer::WIDTH, Renderer::HEIGHT}, "Physics"};

	sf::Font font;
	font.loadFromFile("RobotoMono-Regular.ttf");

	sf::Text step_time_text;
	step_time_text.setFont(font);
	step_time_text.setCharacterSize(12);
	step_time_text.setFillColor(sf::Color::White);

	sf::Text p_count_text;
	p_count_text.setFont(font);
	p_count_text.setCharacterSize(12);
	p_count_text.setFillColor(sf::Color::White);
	p_count_text.setPosition({0, 12});

	sf::Clock clock;
	sf::Time accumulator = sf::Time::Zero;
	sf::Time ups = sf::seconds(World::DELTA_TIME);

	Renderer renderer { window };
	World world;
	auto& experiment = e1;
	bool paused = true;

	experiment.init(world);
	while (window.isOpen()) {
		clock.restart();
		for (auto event = sf::Event(); window.pollEvent(event);) {
			switch (event.type) {
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::MouseButtonReleased:
					world.add_particle(Particle {{
						sf::Mouse::getPosition(window).x / static_cast<float>(Renderer::SCALE),
						sf::Mouse::getPosition(window).y / static_cast<float>(Renderer::SCALE)
					}});
					break;
				case sf::Event::KeyReleased:
					if (event.key.code == sf::Keyboard::P) {
						paused = !paused;
					} else if (event.key.code == sf::Keyboard::R) {
						world.particles.clear();
						experiment.init(world);
						frame = 0;
					} else if (event.key.code == sf::Keyboard::S && paused) {
						experiment.step(world);
						world.update(ups.asSeconds());
					}
					break;
			}
		}

		if (!paused) {
			experiment.step(world);
			world.update(ups.asSeconds());
		}
		p_count_text.setString(std::to_string(world.particles.size()) + " bodies");

		renderer.render(world);
		window.draw(step_time_text);
		window.draw(p_count_text);
		window.display();

		auto elapsed = clock.restart();
		step_time_text.setString(std::to_string(elapsed.asMicroseconds()) + " us");
		if (elapsed < ups) {
			step_time_text.setFillColor(sf::Color::White);
			sf::sleep(ups - elapsed);
		} else {
			step_time_text.setFillColor(sf::Color::Red);
		}
	}

	return EXIT_SUCCESS;
}