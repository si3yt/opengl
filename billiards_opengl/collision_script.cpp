#include "billiards_header.h"

Collision::Collision() {
	floor_height = _FLOOR_HEIGHT;
	floor_width = _FLOOR_WIDTH;
	ball_radius = _BALL_RADIUS;
	hole_size = _HOLE_SIZE;
}

Collision::~Collision() {}

bool Collision::floor_judge(GLdouble pos[]) {
	if (floor_height / 2 > abs(pos[1]) && floor_width / 2 - hole_size > abs(pos[0]) && ball_radius <= pos[2]) {
		return true;
	}
	if (floor_height / 2 - hole_size > abs(pos[1]) && floor_width / 2 > abs(pos[0]) && ball_radius <= pos[2]) {
		return true;
	}
	return false;
}

GLdouble *Collision::floor(Ball ball, GLdouble *vec) {
	if (floor_judge(ball.pos)) {
		vec[2] += -_GRAVITY;
		return vec;
	}
	return vec;
}

bool Collision::hole_lrside_judge(GLdouble pos[]) {
	if (floor_width / 2 + ball_radius - hole_size > abs(pos[0])) {
		return true;
	}
	return false;
}

bool Collision::hole_tdside_judge(GLdouble pos[]) {
	if (floor_height / 2 + ball_radius - hole_size > abs(pos[1])) {
		return true;
	}
	return false;
}

GLdouble *Collision::hole(Ball ball, GLdouble *vec) {
	for (size_t i = 0; i < 3; i++) {
		vec[i] = 0.0;
	}
	if (in_hole) {
		if (hole_lrside_judge(ball.pos)) {
			vec[0] = -2.0 * ball.vec[0];
		}
		if (hole_tdside_judge(ball.pos)) {
			vec[1] = -2.0 * ball.vec[1];
		}
	}
	return vec;
}

bool Collision::wall_lrside_judge(GLdouble pos_x) {
	if (floor_width / 2 - ball_radius - 1.0 < abs(pos_x)) {
		return true;
	}
	return false;
}

bool Collision::wall_tdside_judge(GLdouble pos_y) {
	if (floor_height / 2 - ball_radius - 1.0 < abs(pos_y)) {
		return true;
	}
	return false;
}

GLdouble *Collision::wall(Ball ball, GLdouble *vec) {
	for (size_t i = 0; i < 3; i++) {
		vec[i] = 0.0;
	}
	if (wall_lrside_judge(ball.pos[0])) {
		vec[0] = -2.0 * ball.vec[0];
	}
	if (wall_tdside_judge(ball.pos[1])) {
		vec[1] = -2.0 * ball.vec[1];
	}

	if (ball.vec[2] != 0.0 && (vec[0] != 0.0 || vec[1] != 0.0)) {
		in_hole = true;
	}
	return vec;
}

bool Collision::ball_judge(GLdouble target_pos[], GLdouble opponent_pos[]) {
	GLdouble distance = 0.0;
	for (size_t i = 0; i < 3; i++) {
		distance += abs(target_pos[i] - opponent_pos[i]);
	}
	if (distance <= 2 * ball_radius) {
		return true;
	}
	return false;
}

GLdouble *Collision::ball_process(Ball target, Ball opponent, GLdouble *vec) {
	for (size_t i = 0; i < 4; i++) {
		vec[i] = 0.0;
	}
	GLdouble angle = atan2((opponent.pos[1] - target.pos[1]), (opponent.pos[0] - target.pos[0]));
	GLdouble diagonal = sqrt(target.vec[0] * target.vec[0] + target.vec[1] * target.vec[1]);
	GLdouble direction_travel = atan2(target.vec[1], target.vec[0]);
	GLdouble difference_angle = direction_travel - angle;
	GLdouble after_target_vec = abs(diagonal * sin(difference_angle));
	GLdouble after_opponent_vec = abs(diagonal * cos(difference_angle));

	GLdouble after_target_vecx, after_target_vecy;
	if (sin(difference_angle) < 0) {
		after_target_vecx = after_target_vec * cos(angle - M_PI / 2);
		after_target_vecy = after_target_vec * sin(angle - M_PI / 2);
	}
	else {
		after_target_vecx = after_target_vec * cos(angle + M_PI / 2);
		after_target_vecy = after_target_vec * sin(angle + M_PI / 2);
	}
	GLdouble after_opponent_vecx = after_opponent_vec * cos(angle);
	GLdouble after_opponent_vecy = after_opponent_vec * sin(angle);

	vec[0] = after_target_vecx;
	vec[1] = after_target_vecy;
	vec[2] = after_opponent_vecx;
	vec[3] = after_opponent_vecy;
	return vec;
}

GLdouble *Collision::ball_not_collision_pos(Ball target, Ball opponent, GLdouble *pos) {
	for (size_t i = 0; i < 3; i++) {
		pos[i] = 0.0;
	}
	GLdouble push_rad = atan2((target.pos[1] - opponent.pos[1]), (target.pos[0] - opponent.pos[0]));
	GLdouble pos_x = _COLLISION_PUSH * cos(push_rad);
	GLdouble pos_y = _COLLISION_PUSH * sin(push_rad);

	pos[0] = pos_x;
	pos[1] = pos_y;
	pos[2] = 0.0;

	return pos;
}

GLdouble **Collision::ball(Ball target, Ball opponent, GLdouble **propaty) {
	for (size_t i = 0; i < 3; i++) {
		propaty[0][i] = 0.0;
		propaty[1][i] = 0.0;
	}
	
	GLdouble target_vec[] = { 0.0, 0.0, 0.0 };
	GLdouble opponent_vec[] = { 0.0, 0.0, 0.0 };
	GLdouble *after_vec1, *after_vec2, av1_array[4], av2_array[4];
	after_vec1 = ball_process(target, opponent, av1_array);
	after_vec2 = ball_process(opponent, target, av2_array);

	target_vec[0] = after_vec1[0] + after_vec2[2];
	target_vec[1] = after_vec1[1] + after_vec2[3];
	opponent_vec[0] = after_vec1[2] + after_vec2[0];
	opponent_vec[1] = after_vec1[3] + after_vec2[1];

	for (size_t i = 0; i < 3; i++) {
		propaty[0][i] = target_vec[i] - target.vec[i];
		propaty[1][i] = opponent_vec[i] - opponent.vec[i];
	}

	return propaty;
}

bool Collision::bottom_floor(GLdouble pos[]) {
	if (pos[2] < _BOTTOM_FLOOR_POS[2]) {
		return true;
	}
	return false;
}