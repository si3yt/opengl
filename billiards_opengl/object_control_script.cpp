#include "billiards_header.h"

ObjectControl::ObjectControl() {
	set_ball();
	key_space = false;
	key_left = false;
	key_right = false;
}

ObjectControl::~ObjectControl() {}

void ObjectControl::set_ball() {
	/* player ball */
	ball_propaty[0].pos = const_cast<GLdouble*>(_PLAYER_POS);
	ball_propaty[0].material = _MS_WHITE_PLASTIC;
	/* other ball */
	ball_propaty[1].pos = const_cast<GLdouble*>(_N1_POS);
	ball_propaty[1].material = _MS_RUBY;
	ball_propaty[2].pos = const_cast<GLdouble*>(_N2_POS);
	ball_propaty[2].material = _MS_RUBY;
	ball_propaty[3].pos = const_cast<GLdouble*>(_N3_POS);
	ball_propaty[3].material = _MS_RUBY;
	ball_propaty[4].pos = const_cast<GLdouble*>(_N4_POS);
	ball_propaty[4].material = _MS_RUBY;
	ball_propaty[5].pos = const_cast<GLdouble*>(_N5_POS);
	ball_propaty[5].material = _MS_RUBY;
	ball_propaty[6].pos = const_cast<GLdouble*>(_N6_POS);
	ball_propaty[6].material = _MS_RUBY;
	ball_propaty[7].pos = const_cast<GLdouble*>(_N7_POS);
	ball_propaty[7].material = _MS_RUBY;
	ball_propaty[8].pos = const_cast<GLdouble*>(_N8_POS);
	ball_propaty[8].material = _MS_RUBY;
	ball_propaty[9].pos = const_cast<GLdouble*>(_N9_POS);
	ball_propaty[9].material = _MS_RUBY;

	for (size_t i = 0; i < array_length(ball_propaty); i++) {
		Ball ball(ball_propaty[i].pos, ball_propaty[i].material);
		balls[i] = ball;
	}	
}

void ObjectControl::draw() {
	if (key_left | key_right) {
		sight.set_move_theta(key_left, key_right);
	}
	sight.set_lookat();

	for (size_t i = 0; i < array_length(balls); i++) {
		balls[i].move();
		balls[i].draw();
	}
	table.draw();
	if (key_space) {
		que.pull += _QUE_ADD_PULL;
	}
	else {
		que.pull = 0.0;
	}
	que.angle = sight.get_angle();
	que.draw(balls[0].pos);
}

void ObjectControl::push_for_space(GLdouble time) {
	GLdouble push = time * _TIME_TO_VEC;
	if (push > _PUSH_SPACE_LIMIT) {
		push = _PUSH_SPACE_LIMIT;
	}
	balls[0].vec[0] += (balls[0].pos[0] - sight.pos[0]) * push;
	balls[0].vec[1] += (balls[0].pos[1] - sight.pos[1]) * push;
}

