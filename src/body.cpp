#include "body.h"

#include <cstdio>
#include <algorithm>

Body::Body(Vec2 position, Vec2 velocity): position {position}, position_last {position - velocity}, acceleration {} {}

void Body::update_position(float dt) {
	const auto velocity = position - position_last;
	position_last = position;
	position += velocity + acceleration * dt * dt;
	acceleration = {};
}

Vec2 Body::get_velocity() const {
	return position - position_last;
}