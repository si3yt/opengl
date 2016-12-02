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
		if (!collision.bottom_floor(balls[i].pos)) {
			GLdouble gravity_force[] = { 0.0, 0.0, _GRAVITY };
			GLdouble *collision_floor_vec;
			collision_floor_vec = collision.floor(balls[i], gravity_force);
			balls[i].add_force(collision_floor_vec);
			GLdouble *collision_wall_vec, cwv_array[3];
			collision_wall_vec = collision.wall(balls[i], cwv_array);
			balls[i].add_force(collision_wall_vec);
			if (collision_floor_vec[2] == _GRAVITY) {
				GLdouble *collision_hole_vec, chv_array[3];
				collision_hole_vec = collision.hole(balls[i], chv_array);
				balls[i].add_force(collision_hole_vec);
			}
			for (size_t j = i + 1; j < array_length(balls); j++) {
				if (collision.ball_judge(balls[i].pos, balls[j].pos)) {
					GLdouble *ball_not_colision_pos, bncp_array[3];
					ball_not_colision_pos = collision.ball_not_collision_pos(balls[i], balls[j], bncp_array);
					balls[i].move_pos(ball_not_colision_pos);
					GLdouble **collision_ball_propaty, cbp_array_two[2][3], *cbp_array_one[3];
					for (size_t k = 0; k < 2; k++) cbp_array_one[k] = cbp_array_two[k];
					collision_ball_propaty = collision.ball(balls[i], balls[j], cbp_array_one);
					balls[i].add_force(collision_ball_propaty[0]);
					balls[j].add_force(collision_ball_propaty[1]);
				}
			}
			balls[i].move_vec();
			balls[i].draw();
		}
	}
	table.draw();
	if (ball_all_stop()) {
		sight.set_center(balls[0].pos);
		if (key_space) {
			que.pull += _QUE_ADD_PULL;
		}
		else {
			que.pull = 0.0;
		}
		que.angle = sight.get_angle();
		que.draw(balls[0].pos);
	}
}

void ObjectControl::push_for_space(GLdouble time) {
	GLdouble push = time * _TIME_TO_VEC;
	if (push > _PUSH_SPACE_LIMIT) {
		push = _PUSH_SPACE_LIMIT;
	}
	GLdouble vec_x = (balls[0].pos[0] - sight.pos[0]) * push;
	GLdouble vec_y = (balls[0].pos[1] - sight.pos[1]) * push;
	GLdouble vec[] = { vec_x, vec_y, 0.0 };
	balls[0].add_force(vec);
}

bool ObjectControl::ball_all_stop() {
	for (size_t i = 0; i < _BALL_NUM; i++) {
		if (balls[i].vec[0] != 0.0 || balls[i].vec[1] != 0.0 || balls[i].vec[2] != 0.0) {
			return false;
		}
	}
	return true;
}

