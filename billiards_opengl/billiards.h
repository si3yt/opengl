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
#include "constant.h"
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

/** class **/
/* get elapsed time */
class Timer {
	double start;
public:
	Timer();
	~Timer();
	void restart();
	double elapsed(); //return seconds
};

/* quaternion calculation */
class Quaternion {
	GLdouble init_value[4];
	GLdouble quat[4];		
	GLdouble trans_quat[4];
public:
	GLdouble trans_mat[16]; //output
	Quaternion();
	~Quaternion();
	void quat_transmat();
	void integration();
	void set_trans_quat(GLdouble *trans);
	void save_init(); //call after rotation
};

/* box generat */
class Box {
	GLint face[6][4];
	GLdouble normal[6][3];
	GLdouble vertex[8][3];
	GLfloat *color;
	//call into constructor
	void set_face();
	void set_normal();
public:
	Box();
	//argument is vertexes
	Box(GLdouble **vertex, GLfloat color[]);
	//argument is not vertexes
	Box(GLdouble width, GLdouble height, GLdouble depth, GLdouble center[], GLfloat color[]);
	~Box();
	//call into constructor of argument is vertexes
	void set_vertex(GLdouble **vertex);
	//call into constructor of argument is not vertexes
	void set_vertex(GLdouble width, GLdouble height, GLdouble depth, GLdouble center[]);
	void draw();
};

/* table generat */
class Table {
	struct box_propaty {
		GLdouble height, width, depth;
		GLdouble *center;
		GLfloat  *color;
	}box_propaty[5];
	struct trapezoid_propaty {
		GLdouble *vertex[8];
		GLfloat  *color;
	}trapezoid_propaty[15];
	Box table_parts[20]; //include box and trapezoid
	void table_init(); //call into constructor
public:
	Table();
	~Table();
	void draw();
};

/* ball(sphere) generate */
class Ball {
	GLdouble friction, in_hole_vec;
	GLdouble vec_threshold;
	GLdouble pos_adjustment;
	GLdouble radius;
	GLUquadricObj* _sphere; //gluSphere propaty 
	Quaternion quaternion; //now rotation
public:
	GLdouble pos[3];
	GLdouble vec[3];
	Ball();
	Ball(GLdouble pos[]);
	~Ball();
	void move_vec();
	void hole_move_vec();
	void move_pos(GLdouble add_pos[]);
	void add_force(GLdouble add_vec[]);
	void vec_rotation();
	void draw(GLuint tex_id);
};

/* sight(camera) setup */
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
	void set_move_theta(bool left, bool right); //move when push key
	GLdouble get_angle(); //use que generate
};

/* que(cylinder) generate */
class Que {
	GLdouble radius, height;
	size_t sides;
public:
	GLdouble angle;
	GLdouble pull; //que pull when push space key
	Que();
	~Que();
	void draw_top();    //circle of top
	void draw_bottom(); //circle of bottom
	void draw_side();   //side of cylinder 
	void draw(GLdouble player_ball[]);
};

/* collision processing */
class Collision {
	GLdouble floor_height, floor_width;
	GLdouble ball_radius;
	GLdouble hole_size;
public:
	Collision();
	~Collision();
	/* floor(gravity) */
	bool floor_judge(GLdouble pos[]);
	GLdouble *floor(Ball ball, GLdouble *vec);
	/* wall of in hole */
	bool hole_lrside_judge(GLdouble pos[]);
	bool hole_tbside_judge(GLdouble pos[]);
	bool hole_center_judge(GLdouble pos[]);
	GLdouble *hole(Ball ball, GLdouble *vec);
	/* wall of table */
	bool wall_lrside_judge(GLdouble pos_x);
	bool wall_tbside_judge(GLdouble pos_y);
	GLdouble *wall(Ball ball, GLdouble *vec);
	/* ball to ball */
	bool ball_judge(GLdouble target_pos[], GLdouble opponent_pos[]);
	GLdouble *ball_process(Ball target, Ball opponent, GLdouble *vec);
	GLdouble *ball_not_collision_pos(Ball target, Ball opponent, GLdouble *pos);
	GLdouble **ball(Ball target, Ball opponent, GLdouble **propaty);
	/* delete floor */
	bool bottom_floor(GLdouble pos[]);
};

/* texture generate */
class Texture {
public:
	Texture();
	~Texture();
	GLuint create_texture(GLuint tex_id, char *texture);
};

/* object control manager */
class ObjectControl {
	//object propaty
	Ball balls[_BALL_NUM];
	struct ball_propaty {
		GLdouble *pos;
	}ball_propaty[_BALL_NUM];
	Table table;
	Sight sight;
	Que que;
	Collision collision;
	Texture texture;
	GLuint tex_id[_BALL_NUM];
	void set_ball();
	void draw_ball();
	void print_str(char* str, GLint length); //clear text
public:
	bool key_space, key_left, key_right; //key push now time
	ObjectControl();
	~ObjectControl();
	void draw();
	bool ball_all_stop(); //restart game
	void push_for_space(GLdouble time);
	void texture_init();
};

#endif
