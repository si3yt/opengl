#include "billiards_header.h"

Box::Box() {}

Box::Box(GLdouble width, GLdouble height, GLdouble depth, GLdouble center[], GLfloat color[]) {
	set_face();
	set_normal();
	set_vertex(width, height, depth, center);
	this->color = color;
}

Box::~Box() {}

void Box::set_vertex(GLdouble width, GLdouble height, GLdouble depth, GLdouble center[]) {
	for (size_t i = 0; i < 8; i++) {
		if (i==0 || i==3 || i==4 || i==7) {
			vertex[i][0] = center[0] - width / 2;
		} else {
			vertex[i][0] = center[0] + width / 2;
		}
		if (i==0 || i==1 || i==4 || i==5) {
			vertex[i][1] = center[1] - height / 2;
		} else {
			vertex[i][1] = center[1] + height / 2;
		}
		if (i/4 == 0) {
			vertex[i][2] = center[2] - depth / 2;
		} else {
			vertex[i][2] = center[2] + depth / 2;
		}
	}
}

void Box::set_face() {
	GLint face_temp[6][4] = {
		{ 0, 1, 2, 3 },
		{ 1, 5, 6, 2 },
		{ 5, 4, 7, 6 },
		{ 4, 0, 3, 7 },
		{ 4, 5, 1, 0 },
		{ 3, 2, 6, 7 }
	};

	for (size_t i = 0; i < 6; i++) {
		for (size_t j = 0; j < 4; j++) {
			face[i][j] = face_temp[i][j];
		}
	}
}

void Box::set_normal() {
	GLdouble normal_temp[6][3] = {
		{  0.0,  0.0, -1.0 },
		{  1.0,  0.0,  0.0 },
		{  0.0,  0.0,  1.0 },
		{ -1.0,  0.0,  0.0 },
		{  0.0, -1.0,  0.0 },
		{  0.0,  1.0,  0.0 }
	};

	for (size_t i = 0; i < 6; i++) {
		for (size_t j = 0; j < 4; j++) {
			normal[i][j] = normal_temp[i][j];
		}
	}
}

void Box::draw() {
	glPushMatrix();
	glBegin(GL_QUADS);
	glColor4fv(color);
	for (size_t i = 0; i < 6; i++) {
		for (size_t j = 0; j < 4; j++) {
			glVertex3dv(vertex[face[i][j]]);
		}
	}
	glEnd();
	glPopMatrix();
}
