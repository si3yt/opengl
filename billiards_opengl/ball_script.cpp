#include "billiards_header.h"

Ball::Ball() {}

Ball::Ball(GLdouble pos[], _MATERIAL_STRUCT material) {
	friction = _FRICTION;
	vec_threshold = _VEC_THRESHOLD;
	radius = _BALL_RADIUS;
	for (size_t i = 0; i < array_length(this->pos); i++) {
		this->pos[i] = pos[i];
		vec[i] = 0.0;
	}
	this->material = material;
}

Ball::~Ball() {}

void Ball::move() {
	for (size_t i = 0; i < array_length(vec); i++) {
		vec[i] = vec[i] * friction;
		if (abs(vec[i]) < vec_threshold) {	//Ž¸‘¬‚É‚æ‚é’âŽ~
			vec[i] = 0.0;
		}
		pos[i] += vec[i];
	}
}

void Ball::draw() {
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, material.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR,material.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, &material.shininess);
	glTranslated(pos[0], pos[1], pos[2]);	    //•½sˆÚ“®’l‚ÌÝ’è
	glutSolidSphere(radius, 20, 20);	//ˆø”F(”¼Œa, ZŽ²‚Ü‚í‚è‚Ì•ªŠ„”, ZŽ²‚É‰ˆ‚Á‚½•ªŠ„”)
	glPopMatrix();
}