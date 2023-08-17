#pragma once

#include <SFML/Graphics.hpp>

class World;

class Renderer {
public:
	static constexpr int WIDTH = 1920;
	static constexpr int HEIGHT = 1080;
	static constexpr int SCALE = 4;
private:
	sf::RenderTarget& render_target;
	sf::CircleShape circle_shape;
public:
	Renderer(sf::RenderTarget& render_target);
	void render(const World& world);
};