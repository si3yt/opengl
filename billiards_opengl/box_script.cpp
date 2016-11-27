#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <direct.h>
#include <time.h>
#include <vector>
#include <GL/glut.h>
#include "billiards_header.h"
using namespace std;

class box {
	GLdouble vertex[8][3];
	GLint face[6][4];
	GLint normal[6][4];
	GLdouble height, width, depth;
	GLdouble center[];
public:
	box();
	~box();
	void set_vertex();
	void set_face();
	void set_normal();
};

box::box() {
	set_face();
	set_normal();
}

box::~box() {
}

void box::set_vertex() {
	for (size_t i = 0; i < 8; i++) {
		if (i==0 || i==3 || i==4 || i==7) {
			vertex[i][0] = center[0] - width / 2;
		} else {
			vertex[i][0] = center[0] + width / 2;
		}
		if (i==0 || i==1 || i==4 || i==5) {
			vertex[i][1] = center[1] - depth / 2;
		} else {
			vertex[i][1] = center[1] + depth / 2;
		}
		if (i/4 == 0) {
			vertex[i][2] = center[2] - height / 2;
		} else {
			vertex[i][1] = center[1] + height / 2;
		}
	}
}

void box::set_face() {
	GLint face_temp[6][4] = {
		{ 0, 1, 2, 3 },
		{ 1, 5, 6, 2 },
		{ 5, 4, 7, 6 },
		{ 4, 0, 3, 7 },
		{ 4, 5, 1, 0 },
		{ 3, 2, 6, 7 }
	};

	memcpy(face, face_temp, sizeof face_temp);
}

void box::set_normal() {
	GLint normal_temp[6][4] = {
		{  0.0,  0.0, -1.0 },
		{  1.0,  0.0,  0.0 },
		{  0.0,  0.0,  1.0 },
		{ -1.0,  0.0,  0.0 },
		{  0.0, -1.0,  0.0 },
		{  0.0,  1.0,  0.0 }
	};

	memcpy(normal, normal_temp, sizeof normal_temp);
}
