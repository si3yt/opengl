#include "billiards_header.h"

/* 初期化 */
ObjectControl::ObjectControl() {
	set_ball();
	key_space = false;
	key_left = false;
	key_right = false;
	//tex_id = texture.create_texture();
}

ObjectControl::~ObjectControl() {}

void ObjectControl::set_ball() {
	for (size_t i = 0; i < _BALL_NUM; i++) {
		ball_propaty[i].pos = const_cast<GLdouble*>(_BALL_POS[i]);
		if (i == 0) ball_propaty[i].material = _MS_WHITE_PLASTIC;
		else ball_propaty[i].material = _MS_RUBY;
	}

	for (size_t i = 0; i < array_length(ball_propaty); i++) {
		Ball ball(ball_propaty[i].pos, ball_propaty[i].material);
		balls[i] = ball;
	}	
}
/* ここまで初期化 */
#define FRAMES 360
/* main draw */
void ObjectControl::draw() {
	if (key_left | key_right) {
		sight.set_move_theta(key_left, key_right);
	}
	sight.set_lookat();

	draw_ball();

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
	
#if 0
	/* フレーム数をカウントして時間として使う */
	static int frame = 0;                      /* フレーム数　　　　　　　 */
	double t = (double)frame / (double)FRAMES; /* 時間とともに 0→1 に変化 */

	if (++frame >= FRAMES) frame = 0;

	/* テクスチャ行列の設定 */
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glTranslated(0.5, 0.5, 0.0);
	glRotated(t * 360.0, 0.0, 0.0, 1.0);

	/* モデルビュー変換行列の設定 */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	for (size_t i = 0; i < _BALL_NUM; i++) {
		glTranslated(0.0, 0.0, -5.0);
		/* 材質の設定 */
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, _WHITE);

		/* アルファテスト開始 */
		glEnable(GL_ALPHA_TEST);

		/* テクスチャマッピング開始 */
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, tex_id[i]);

		/* テクスチャ座標の自動生成を有効にする */
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);

		/* ティーポットを描く */
		glutSolidSphere(1.0, 32, 16);

		/* テクスチャ座標の自動生成を無効にする */
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);

		/* テクスチャマッピング終了 */
		glDisable(GL_TEXTURE_2D);

		/* アルファテスト終了 */
		glDisable(GL_ALPHA_TEST);
	}
#else
#endif
}
void ObjectControl::draw_ball() {
	for (size_t i = 0; i < array_length(balls); i++) {
		if (!collision.bottom_floor(balls[i].pos)) {
			/* 床上重力反発 */
			GLdouble gravity_force[] = { 0.0, 0.0, _GRAVITY };
			GLdouble *collision_floor_vec;
			collision_floor_vec = collision.floor(balls[i], gravity_force);
			balls[i].add_force(collision_floor_vec);
			/* 壁反射 */
			GLdouble *collision_wall_vec, cwv_array[3];
			collision_wall_vec = collision.wall(balls[i], cwv_array);
			balls[i].add_force(collision_wall_vec);
			/* ボール同士 */
			for (size_t j = i + 1; j < array_length(balls); j++) {
				if (collision.ball_judge(balls[i].pos, balls[j].pos)) {
					/* コリジョン解除 */
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
			/* 穴内部における速度調整 */
			if (collision_floor_vec[2] == _GRAVITY) {
				GLdouble *collision_hole_vec, chv_array[3];
				collision_hole_vec = collision.hole(balls[i], chv_array);
				balls[i].add_force(collision_hole_vec);
				balls[i].hole_move_vec();
			}else {
				balls[i].move_vec();
			}
			balls[i].draw(tex_id[i]);
		} else { //プレイヤーボールの復活
			if (i == 0 && ball_all_stop()) {
				Ball ball(ball_propaty[i].pos, ball_propaty[i].material);
				balls[i] = ball;
			}
		}
	}
}

/* スペースキー入力によって与えられる add force */
void ObjectControl::push_for_space(GLdouble time) {
	GLdouble push = time * _TIME_TO_VEC;
	if (push > _PUSH_SPACE_LIMIT) push = _PUSH_SPACE_LIMIT;
	
	GLdouble vec_x = (balls[0].pos[0] - sight.pos[0]) * push;
	GLdouble vec_y = (balls[0].pos[1] - sight.pos[1]) * push;
	GLdouble vec[] = { vec_x, vec_y, 0.0 };
	balls[0].add_force(vec);
}

/* ゲーム再開用判定 */
bool ObjectControl::ball_all_stop() {
	for (size_t i = 0; i < _BALL_NUM; i++) {
		if (!collision.bottom_floor(balls[i].pos)) {
			if (balls[i].vec[0] != 0.0 || balls[i].vec[1] != 0.0 || balls[i].vec[2] != 0.0) {
				return false;
			}
		}
	}
	return true;
}

void ObjectControl::texture_init() {
	glGenTextures(_BALL_NUM, tex_id);
	for (size_t i = 0; i < _BALL_NUM; i++) {
		tex_id[i] = texture.create_texture(tex_id[i], const_cast<char*>(_BALL_TEXTURE[i]));
	}
}

