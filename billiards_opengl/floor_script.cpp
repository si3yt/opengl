#include "billiards_header.h"

Floor::Floor() {
	floor_init();
}

Floor::~Floor() {}

void Floor::floor_init() {
	set_face();
	set_vertex();
	color = const_cast<GLfloat*>(_GREEN);
}

void Floor::set_face() {
	GLint face_temp[3][4] = {						//è∞ñ 
		{ 0,  1,  6,  7 },
		{ 2,  3,  4,  5 },
		{ 10, 11, 8,  9 }
	};

	for (size_t i = 0; i < 3; i++) {
		for (size_t j = 0; j < 4; j++) {
			face[i][j] = face_temp[i][j];
		}
	}
}

void Floor::set_vertex() {
	GLdouble vertex_temp[12][3] = {					//è∞í∏ì_
		{  _FLOOR_WIDTH / 2 - _HOLE_SIZE,  _FLOOR_HEIGHT / 2,			   0.0 },
		{ -_FLOOR_WIDTH / 2 + _HOLE_SIZE,  _FLOOR_HEIGHT / 2,			   0.0 },
		{ -_FLOOR_WIDTH / 2 + _HOLE_SIZE,  _FLOOR_HEIGHT / 2 - _HOLE_SIZE, 0.0 },
		{ -_FLOOR_WIDTH / 2,			   _FLOOR_HEIGHT / 2 - _HOLE_SIZE, 0.0 },
		{ -_FLOOR_WIDTH / 2,			  -_FLOOR_HEIGHT / 2 + _HOLE_SIZE, 0.0 },
		{ -_FLOOR_WIDTH / 2 + _HOLE_SIZE, -_FLOOR_HEIGHT / 2 + _HOLE_SIZE, 0.0 },
		{ -_FLOOR_WIDTH / 2 + _HOLE_SIZE, -_FLOOR_HEIGHT / 2,			   0.0 },
		{  _FLOOR_WIDTH / 2 - _HOLE_SIZE, -_FLOOR_HEIGHT / 2,			   0.0 },
		{  _FLOOR_WIDTH / 2 - _HOLE_SIZE, -_FLOOR_HEIGHT / 2 + _HOLE_SIZE, 0.0 },
		{  _FLOOR_WIDTH / 2,			  -_FLOOR_HEIGHT / 2 + _HOLE_SIZE, 0.0 },
		{  _FLOOR_WIDTH / 2,			   _FLOOR_HEIGHT / 2 - _HOLE_SIZE, 0.0 },
		{  _FLOOR_WIDTH / 2 - _HOLE_SIZE,  _FLOOR_HEIGHT / 2 - _HOLE_SIZE, 0.0 },
	};

	for (size_t i = 0; i < 12; i++) {
		for (size_t j = 0; j < 3; j++) {
			vertex[i][j] = vertex_temp[i][j];
		}
	}
}

void Floor::draw() {
	glPushMatrix();
	glColor4fv(color);
	glBegin(GL_QUADS);
	for (GLint i = 0; i < 3; ++i) {
		for (GLint j = 0; j < 4; ++j) {
			glVertex3dv(vertex[face[i][j]]);
		}
	}
	glEnd();
	glPopMatrix();
}