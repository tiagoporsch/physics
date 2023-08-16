#pragma once

#include <SFML/Graphics.hpp>

#include "solver.h"

class Renderer {
public:
	static constexpr int HEIGHT = 720;
	static constexpr int WIDTH = 16 * HEIGHT / 9;
	static constexpr int SCALE = 2;
private:
	sf::RenderTarget& render_target;
	sf::CircleShape circle_shape;
public:
	Renderer(sf::RenderTarget& render_target);
	void render(const Solver& solver);
};