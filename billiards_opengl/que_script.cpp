#include "billiards_header.h"

Que::Que() {
	radius = _QUE_RADIUS;
	height = _QUE_HEIGHT;
	sides = _QUE_SIDES;
	pull = 0.0;
}

Que::~Que() {}

void Que::draw_top() {
	glNormal3d(0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	for (size_t i = 0; i < sides; i++) {
		GLdouble t = M_PI * 2.0 / sides * i;
		glVertex3d(radius * cos(t), height, radius * sin(t));
	}
	glEnd();
}

void Que::draw_bottom() {
	glNormal3d(0.0, -1.0, 0.0);
	glBegin(GL_POLYGON);
	for (size_t i = 0; i < sides; i++) {
		GLdouble t = M_PI * 2.0 / sides * i;
		glVertex3d(radius * cos(t), -height, radius * sin(t));
	}
	glEnd();
}

void Que::draw_side() {
	glBegin(GL_QUAD_STRIP);
	for (size_t i = 0; i <= sides; i++) {
		GLdouble t = M_PI * 2.0 / sides * i;
		glNormal3d(cos(t), 0.0, sin(t));
		glVertex3d(radius * cos(t), -height, radius * sin(t));
		glVertex3d(radius * cos(t), height, radius * sin(t));
	}
	glEnd();
}

void Que::draw(GLdouble player_ball[]) {
	glPushMatrix();
	glTranslated(player_ball[0], player_ball[1], _QUE_POS_HEIGHT);
	glRotated(angle * 180 / M_PI + 90, 0.0, 0.0, 1.0);
	glTranslated(0, _QUE_PULL + pull, 0.0);
	glRotated(_QUE_ROTATE, 1.0, 0.0, 0.0);
	draw_top();
	draw_bottom();
	draw_side();
	glPopMatrix();
}