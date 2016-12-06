#include "billiards_header.h"

Ball::Ball() {}

/* ������ */
Ball::Ball(GLdouble pos[], _MATERIAL_STRUCT material) {
	friction = _FRICTION;
	in_hole_vec = _IN_HOLE_VEC;
	vec_threshold = _VEC_THRESHOLD;
	radius = _BALL_RADIUS;
	for (size_t i = 0; i < array_length(this->pos); i++) {
		this->pos[i] = pos[i];
		vec[i] = 0.0;
		rotation[i] = 0.0;
		if (i == 1) rotation[i] = 45.0;
	}
	this->material = material;

	/* sphere texture �ݒ� */
	_sphere = gluNewQuadric();
	gluQuadricDrawStyle(_sphere, GLU_FILL);
	gluQuadricNormals(_sphere, GLU_SMOOTH);
	gluQuadricTexture(_sphere, GL_TRUE);
}

Ball::~Ball() {}

/* �x�N�g���ɂ��{�[������ */
void Ball::move_vec() {
	for (size_t i = 0; i < array_length(vec)-1; i++) {
		vec[i] = vec[i] * friction; //�x�N�g���̌���
		if (abs(vec[i]) < vec_threshold) {	//�����ɂ���~
			vec[i] = 0.0;
		}
	}
	for (size_t i = 0; i < array_length(vec); i++) {
		pos[i] += vec[i];
	}
}

/* �{�[�������ɗ��������̃{�[������ */
void Ball::hole_move_vec() {
	for (size_t i = 0; i < array_length(vec) - 1; i++) {
		// �x�N�g�������l�ɂ���
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

void Ball::draw_material() {
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR,material.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, &material.shininess);
	glTranslated(pos[0], pos[1], pos[2]);	    //���s�ړ��l�̐ݒ�
	glutSolidSphere(radius, 20, 20);	//�����F(���a, Z���܂��̕�����, Z���ɉ�����������)
	glPopMatrix();
}

void Ball::vec_rotation() {
	for (size_t i = 0; i < 2; i++) {
		rotation[i] += vec[i] * _VEC_ROTAT_ADJUST;
	}
	glRotated(rotation[0], 0.0, 1.0, 0.0);
	glRotated(rotation[1], -1.0, 0.0, 0.0);
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