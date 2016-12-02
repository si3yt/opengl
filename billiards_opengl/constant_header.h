#pragma once

/* windows */
const int _WINDOW_POSITION_X = 100;
const int _WINDOW_POSITION_Y = 100;
const int _WINDOW_WIDTH = 512;
const int _WINDOW_HEIGHT = 512;
const char _WINDOW_TITLE[] = "billiards";

/* light */
const GLfloat _LIGHT_POSITION_0[] = { 0.0f, 0.0f, 150.0f, 1.0f };

/* color */
const GLfloat _RED[] = { 0.8f, 0.2f, 0.2f, 1.0f };
const GLfloat _GREEN[] = { 0.2f, 0.8f, 0.2f, 1.0f };
const GLfloat _BLACK[] = { 0.0f, 0.0f, 0.0f, 1.0f };

/* material */
struct _MATERIAL_STRUCT {							// material用構造体
	GLfloat ambient[4];		// 環境反射
	GLfloat diffuse[4];		// 拡散反射
	GLfloat specular[4];	// 鏡面反射
	GLfloat shininess;		// 光沢
};
const _MATERIAL_STRUCT _MS_WHITE_PLASTIC = {				//プラスチック(白)
	{ 0.0f,  0.0f,  0.0f,  1.0f },
	{ 0.55f, 0.55f, 0.55f, 1.0f },
	{ 0.70f, 0.70f, 0.70f, 1.0f },
	32.0f };
const _MATERIAL_STRUCT _MS_RUBY = {						// ruby(ルビー)
	{ 0.1745f,   0.01175f,  0.01175f,   1.0f },
	{ 0.61424f,  0.04136f,  0.04136f,   1.0f },
	{ 0.727811f, 0.626959f, 0.626959f,  1.0f },
	76.8f };

/* floor */
const GLdouble _FLOOR_WIDTH = 100.0;
const GLdouble _FLOOR_HEIGHT = 200.0;

/* hole */
const GLdouble _HOLE_SIZE = 12.0;

/* wall */
const GLdouble _WALL_SIZE = 10.0;
const GLdouble _WALL_TOP_POS[] = { 0.0, _FLOOR_HEIGHT / 2 + _WALL_SIZE / 2,    0.0 };
const GLdouble _WALL_BOTTOM_POS[] = { 0.0, -(_FLOOR_HEIGHT / 2 + _WALL_SIZE / 2), 0.0 };
const GLdouble _WALL_LEFT_POS[] = { _FLOOR_WIDTH / 2 + _WALL_SIZE / 2,    0.0,  0.0 };
const GLdouble _WALL_RIGHT_POS[] = { -(_FLOOR_WIDTH / 2 + _WALL_SIZE / 2), 0.0,  0.0 };

/* bottom floor */
const GLdouble _BOTTOM_FLOOR_POS[] = { 0.0, 0.0, -_WALL_SIZE * (3.0 / 4.0) };

/* ball */
const GLdouble _FRICTION = 0.995;
const GLdouble _VEC_THRESHOLD = 0.01;
const GLdouble _TIME_TO_VEC = 0.01;
const GLdouble _BALL_RADIUS = 4.0;
const GLdouble _BALL_DIAMOND_TOP = 10.0;
const GLint _BALL_NUM = 10;

const GLdouble _PLAYER_POS[] = { 0.0,                     -_FLOOR_HEIGHT / 4,                   _BALL_RADIUS };
const GLdouble _N1_POS[] = { 0.0,                     _BALL_DIAMOND_TOP,                    _BALL_RADIUS };
const GLdouble _N2_POS[] = { -_BALL_RADIUS - 1.0,     _BALL_DIAMOND_TOP + 2 * _BALL_RADIUS, _BALL_RADIUS };
const GLdouble _N3_POS[] = { _BALL_RADIUS + 1.0,      _BALL_DIAMOND_TOP + 2 * _BALL_RADIUS, _BALL_RADIUS };
const GLdouble _N4_POS[] = { -_BALL_RADIUS * 2 - 1.0, _BALL_DIAMOND_TOP + 4 * _BALL_RADIUS, _BALL_RADIUS };
const GLdouble _N5_POS[] = { 0.0,                     _BALL_DIAMOND_TOP + 4 * _BALL_RADIUS, _BALL_RADIUS };
const GLdouble _N6_POS[] = { _BALL_RADIUS * 2 + 1.0,  _BALL_DIAMOND_TOP + 4 * _BALL_RADIUS, _BALL_RADIUS };
const GLdouble _N7_POS[] = { -_BALL_RADIUS - 1.0,     _BALL_DIAMOND_TOP + 6 * _BALL_RADIUS, _BALL_RADIUS };
const GLdouble _N8_POS[] = { _BALL_RADIUS + 1.0,      _BALL_DIAMOND_TOP + 6 * _BALL_RADIUS, _BALL_RADIUS };
const GLdouble _N9_POS[] = { 0.0,                     _BALL_DIAMOND_TOP + 8 * _BALL_RADIUS, _BALL_RADIUS };

const GLdouble _PUSH_SPACE_LIMIT = 0.05;
const GLdouble _GRAVITY = -0.02;
const GLdouble _COLLISION_PUSH = 0.5;

/* que */
const GLdouble _QUE_RADIUS = 2.0;
const GLdouble _QUE_HEIGHT = 80.0;
const size_t _QUE_SIDES = 10;
const GLdouble _QUE_PULL = _QUE_HEIGHT + 2.0;
const GLdouble _QUE_POS_HEIGHT = 26.0;
const GLdouble _QUE_ROTATE = 15.0;
const GLdouble _QUE_ADD_PULL = 0.1;

/* sight */
const GLdouble _SIGHT_RADIUS = 300.0;
const GLdouble _SIGHT_PLUS = 0.03; //radian
const GLdouble _SIGHT_POS_Z = 200.0;
const GLdouble _SIGHT_CENTER_POS_INIT[] = { 0.0, -_FLOOR_HEIGHT / 4, _BALL_RADIUS };
const GLdouble _SIGHT_VEC[] = { 0.0, 0.0, 1.0 };

/* keyborad */
const int _KEY_SPACE = 040;
const int _KEY_ESC = 033;

