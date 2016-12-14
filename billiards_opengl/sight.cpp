#include "billiards.h"

Sight::Sight() {
	radius = _SIGHT_RADIUS;
	for (size_t i = 0; i < array_length(vec); i++) vec[i] = _SIGHT_VEC[i];
	move_theta = 0.0;
	set_center(const_cast<GLdouble*>(_BALL_POS[0]));
	set_pos();
	pos[2] = _SIGHT_POS_Z;
}

Sight::~Sight() {}

void Sight::set_lookat() {
	gluLookAt(
		pos[0], pos[1], pos[2],
		center_pos[0], center_pos[1], center_pos[2],
		vec[0], vec[1], vec[2]
	);
}

/* move circle */
void Sight::set_pos() {
	pos[0] = center_pos[0] - radius * -sin(move_theta);
	pos[1] = center_pos[1] - radius * cos(move_theta);
}

/* center is player ball when all balls stop */
void Sight::set_center(GLdouble player_ball[]) {
	for (size_t i = 0; i < array_length(pos); i++) center_pos[i] = player_ball[i];
}

/* push space key */
void Sight::set_move_theta(bool left, bool right) {
	if (!left | !right) {
		if (left) move_theta = move_theta - _SIGHT_PLUS;
		else if (right) move_theta = move_theta + _SIGHT_PLUS;
		set_pos();
	}
}

/* angle of player ball and sight */
GLdouble Sight::get_angle() {
	return atan2(center_pos[1] - pos[1], center_pos[0] - pos[0]);
}