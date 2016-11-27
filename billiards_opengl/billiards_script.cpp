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

class ball {
	GLdouble pos[3];		//x,y,z
	GLdouble vec[3];		//vx,vy,vz
	GLdouble friction;
	GLdouble vec_threshold;
	GLdouble pos_adjustment;
	GLdouble radius;
	GLdouble floor_width, floor_height;
public:
	ball();
	~ball();
	void add_force();
	void move();
	bool collision_ball(GLdouble opp_pos[]);
	void collision_wall();
	void draw();
};

ball::ball() {
	friction = _FRICTION;
	vec_threshold = _VEC_THRESHOLD;
	pos_adjustment = _POS_ADJUSTMENT;
	radius = _BALL_RADIUS;
	floor_width = _FLOOR_WIDTH;
	floor_height = _FLOOR_HEIGHT;
}

ball::~ball() {

}

void ball::add_force() {
	for (size_t i = 0; i < array_length(vec); i++) {
		vec[i] = vec[i] * friction;
		if (vec[i] < vec_threshold) {	//Ž¸‘¬‚É‚æ‚é’âŽ~
			vec[i] = 0.0;
		}
	}
}

void ball::move() {
	for (size_t i = 0; i < array_length(pos); i++) {
		pos[i] += vec[i] * pos_adjustment;
	}
}

bool ball::collision_ball(GLdouble opp_pos[]) {
	GLdouble distance = 0;
	for (size_t i = 0; i < array_length(pos); i++) {
		distance += pow(pos[i] - opp_pos[i], 2.0);
	}
	if (distance <= pow(radius, 2.0)) {
		return true;
	}
	return false;
}

void ball::collision_wall() {
	if (abs(pos[0]) > floor_width/2 - radius) {
		vec[0] = -vec[0];
	}
	if (abs(pos[1]) > floor_height/2 - radius) {
		vec[1] = -vec[1];
	}
}

void ball::draw() {

}