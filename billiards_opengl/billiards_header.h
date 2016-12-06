#ifndef _INC_BILLIARDS_H_
#define _INC_BILLIARDS_H_

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
#include <windows.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "glext.h"
#include "constant_header.h"
#include "opencv.h"
using namespace std;

/* get array length */
template
<
	typename TYPE,
	std::size_t SIZE
>
std::size_t array_length(const TYPE(&)[SIZE])
{
	return SIZE;
}

/* class */
/* Timer class */
class Timer {
	double start;
public:
	Timer();
	~Timer();
	void restart();
	double elapsed(); //seconds
};

/* Box class */
class Box {
	GLint face[6][4];
	GLdouble normal[6][3];
	void set_face();
	void set_normal();
public:
	GLdouble vertex[8][3];
	GLfloat *color;
	Box();
	Box(GLdouble width, GLdouble height, GLdouble depth, GLdouble center[], GLfloat color[]);
	~Box();
	void set_vertex(GLdouble width, GLdouble height, GLdouble depth, GLdouble center[]);
	void draw();
};

/* Floor class */
class Floor {
	GLint face[3][4];
	GLdouble vertex[12][3];
	GLfloat *color;
	void floor_init();
	void set_face();
	void set_vertex();
public:
	Floor();
	~Floor();
	void draw();
};

/* Tbale class */
class Table {
	struct box_propaty {
		GLdouble height, width, depth;
		GLdouble *center;
		GLfloat  *color;
	}box_propaty[5];
	Box table_parts[5];
	Floor table_floor;
	void table_init();
public:
	Table();
	~Table();
	void draw();
};

/* Ball(Sphere) class */
class Ball {
	GLdouble friction, in_hole_vec;
	GLdouble vec_threshold;
	GLdouble pos_adjustment;
	GLdouble radius;
	void draw_material();
public:
	GLdouble pos[3];
	GLdouble vec[3];
	_MATERIAL_STRUCT material;
	Ball();
	Ball(GLdouble pos[], _MATERIAL_STRUCT material);
	~Ball();
	void move_vec();
	void hole_move_vec();
	void move_pos(GLdouble add_pos[]);
	void add_force(GLdouble add_vec[]);
	void draw(GLuint tex_id);
};

/* Sight(Camera) class */
class Sight {
	GLdouble center_pos[3];
	GLdouble vec[3];
	GLdouble radius;
public:
	GLdouble pos[3];
	GLdouble move_theta;
	Sight();
	~Sight();
	void set_lookat();
	void set_pos();
	void set_center(GLdouble player[]);
	void set_move_theta(bool left, bool right);
	GLdouble get_angle();
};

/* Que(Cylinder) class */
class Que {
	GLdouble radius, height;
	size_t sides;
public:
	GLdouble angle;
	GLdouble pull;
	Que();
	~Que();
	void draw_top();
	void draw_bottom();
	void draw_side();
	void draw(GLdouble player_ball[]);
};

/* Collision class */
class Collision {
	bool in_hole;
	GLdouble floor_height, floor_width;
	GLdouble ball_radius;
	GLdouble hole_size;
public:
	Collision();
	~Collision();
	bool floor_judge(GLdouble pos[]);
	GLdouble *floor(Ball ball, GLdouble *vec);
	bool hole_lrside_judge(GLdouble pos[]);
	bool hole_tdside_judge(GLdouble pos[]);
	GLdouble *hole(Ball ball, GLdouble *vec);
	bool wall_lrside_judge(GLdouble pos_x);
	bool wall_tdside_judge(GLdouble pos_y);
	GLdouble *wall(Ball ball, GLdouble *vec);
	bool ball_judge(GLdouble target_pos[], GLdouble opponent_pos[]);
	GLdouble *ball_process(Ball target, Ball opponent, GLdouble *vec);
	GLdouble *ball_not_collision_pos(Ball target, Ball opponent, GLdouble *pos);
	GLdouble **ball(Ball target, Ball opponent, GLdouble **propaty);
	bool bottom_floor(GLdouble pos[]);
};

class Texture {
public:
	Texture();
	~Texture();
	GLuint create_texture(GLuint tex_id, char *texture);
};



/* ObjectControl class (manager) */
class ObjectControl {
	Ball balls[_BALL_NUM];
	struct ball_propaty {
		GLdouble *pos;
		_MATERIAL_STRUCT material;
	}ball_propaty[_BALL_NUM];
	Table table;
	Sight sight;
	Que que;
	Collision collision;
	bool ball_all_stop();
	void draw_ball();
	Texture texture;
	GLuint tex_id[_BALL_NUM];
public:
	bool key_space, key_left, key_right;
	ObjectControl();
	~ObjectControl();
	void set_ball();
	void draw();
	void push_for_space(GLdouble time);
	void texture_init();
};

#endif
