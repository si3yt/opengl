#include "billiards_header.h"

Ball::Ball() {}

/* 初期化 */
Ball::Ball(GLdouble pos[], _MATERIAL_STRUCT material) {
	friction = _FRICTION;
	in_hole_vec = _IN_HOLE_VEC;
	vec_threshold = _VEC_THRESHOLD;
	radius = _BALL_RADIUS;
	for (size_t i = 0; i < array_length(this->pos); i++) {
		this->pos[i] = pos[i];
		vec[i] = 0.0;
	}
	this->material = material;

	/* sphere texture 設定 */
	_sphere = gluNewQuadric();
	gluQuadricDrawStyle(_sphere, GLU_FILL);
	gluQuadricNormals(_sphere, GLU_SMOOTH);
	gluQuadricTexture(_sphere, GL_TRUE);
}

Ball::~Ball() {}

/* ベクトルによるボール動作 */
void Ball::move_vec() {
	for (size_t i = 0; i < array_length(vec)-1; i++) {
		vec[i] = vec[i] * friction; //ベクトルの減衰
		if (abs(vec[i]) < vec_threshold) {	//失速による停止
			vec[i] = 0.0;
		}
	}
	for (size_t i = 0; i < array_length(vec); i++) {
		pos[i] += vec[i];
	}
}

/* ボールが穴に落ちた時のボール動作 */
void Ball::hole_move_vec() {
	for (size_t i = 0; i < array_length(vec) - 1; i++) {
		// ベクトルを一定値にする
		if (vec[i] > 0.0) vec[i] = in_hole_vec;
		if (vec[i] < 0.0) vec[i] = -in_hole_vec;
	}
	for (size_t i = 0; i < array_length(vec); i++) {
		pos[i] += vec[i];
	}
}

void Ball::move_pos(GLdouble add_pos[]) {
	for (size_t i = 0; i < array_length(pos); i++) {
		pos[i] += add_pos[i];
	}
}

void Ball::add_force(GLdouble add_vec[]) {
	for (size_t i = 0; i < array_length(vec); i++) {
		vec[i] += add_vec[i];
	}
}

void Ball::vec_rotation() {
	GLdouble rotation = 0.0;
	for (size_t i = 0; i < 2; i++) {
		rotation += abs(vec[i]) * _VEC_ROTAT_ADJUST;
		if (rotation >= 360.0) rotation -= 360.0;
	}
	rotation *= M_PI / 180.0;

	GLdouble angle = 0.0;
	if (vec[0] != 0.0 && vec[1] != 0.0) angle = atan2(vec[1],vec[0]);
	else if (vec[0] == 0.0 && vec[1] > 0.0) angle = M_PI / 2;
	else if (vec[0] == 0.0 && vec[1] < 0.0) angle = -M_PI / 2;
	else if (vec[1] == 0.0 && vec[0] > 0.0) angle = 0.0;
	else if (vec[1] == 0.0 && vec[0] < 0.0) angle = M_PI;

	GLdouble theta = rotation;
	GLdouble quat_array[]{ cos(theta), -sin(angle)*sin(theta), cos(angle)*sin(theta), 0.0 };
	quaternion.set_trans_quat(quat_array);
	quaternion.integration();
	quaternion.quat_transmat();
	quaternion.save_init();
	glMultMatrixd(quaternion.trans_mat);
}

void Ball::draw(GLuint tex_id) {
	glPushMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, _WHITE);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, tex_id);

	glTranslated(pos[0], pos[1], pos[2]);
	vec_rotation();
	gluSphere(_sphere, radius, 20, 20);	

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_ALPHA_TEST);

	glPopMatrix();
}