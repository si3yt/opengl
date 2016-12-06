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
	}
	this->material = material;
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

void Ball::draw(GLuint tex_id) {
	glPushMatrix();
	/* �ގ��̐ݒ� */
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, _WHITE);

	/* �A���t�@�e�X�g�J�n */
	glEnable(GL_ALPHA_TEST);

	/* �e�N�X�`���}�b�s���O�J�n */
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, tex_id);

	/* �e�N�X�`�����W�̎���������L���ɂ��� */
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	/* ����`�� */
	glTranslated(pos[0], pos[1], pos[2]);	    //���s�ړ��l�̐ݒ�
	glutSolidSphere(radius, 20, 20);	//�����F(���a, Z���܂��̕�����, Z���ɉ�����������)

	/* �e�N�X�`�����W�̎��������𖳌��ɂ��� */
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	/* �e�N�X�`���}�b�s���O�I�� */
	glDisable(GL_TEXTURE_2D);

	/* �A���t�@�e�X�g�I�� */
	glDisable(GL_ALPHA_TEST);
	glPopMatrix();
}