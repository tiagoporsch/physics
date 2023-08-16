#pragma once

#include <memory>
#include <vector>

#include "vec2.h"

struct Body {
	Vec2 position;
	Vec2 position_last;
	Vec2 acceleration;

	Body(Vec2 position = {}, Vec2 velocity = {});

	void update_position(float dt);

	Vec2 get_velocity() const;
};