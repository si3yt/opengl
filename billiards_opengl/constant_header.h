#pragma once

/* windows */
static const int  _WINDOW_POSITION_X = 100;
static const int  _WINDOW_POSITION_Y = 100;
static const int  _WINDOW_WIDTH      = 512;
static const int  _WINDOW_HEIGHT     = 512;
static const char _WINDOW_TITLE[]    = "billiards";

/* light */
static const GLfloat _LIGHT_POSITION_0[] = { 0.0f, 0.0f, 150.0f, 1.0f };

/* color */
static const GLfloat _RED[]   = { 0.8f, 0.2f, 0.2f, 1.0f };
static const GLfloat _GREEN[] = { 0.2f, 0.8f, 0.2f, 1.0f };
static const GLfloat _BLACK[] = { 0.0f, 0.0f, 0.0f, 1.0f };
static const GLfloat _WHITE[] = { 1.0f, 1.0f, 1.0f, 1.0f };
static const GLfloat _BROWN[] = { 0.6f, 0.2f, 0.0f, 1.0f };
static const GLfloat _LIGHT_BROWN[] = { 0.8f, 0.6f, 0.4f, 1.0f };
static const GLfloat _RED_BROWN[] = { 0.8f, 0.6f, 0.6f };

/* material */
struct _MATERIAL_STRUCT {								// material用構造体
	GLfloat ambient[4];		// 環境反射
	GLfloat diffuse[4];		// 拡散反射
	GLfloat specular[4];	// 鏡面反射
	GLfloat shininess;		// 光沢
};
static const _MATERIAL_STRUCT _MS_WHITE_PLASTIC = {			//プラスチック(白)
	{ 0.0f,  0.0f,  0.0f,  1.0f },
	{ 0.55f, 0.55f, 0.55f, 1.0f },
	{ 0.70f, 0.70f, 0.70f, 1.0f },
	32.0f };
static const _MATERIAL_STRUCT _MS_RUBY = {						// ruby(ルビー)
	{ 0.1745f,   0.01175f,  0.01175f,   1.0f },
	{ 0.61424f,  0.04136f,  0.04136f,   1.0f },
	{ 0.727811f, 0.626959f, 0.626959f,  1.0f },
	76.8f };

/* hole */
static const GLdouble _HOLE_SIZE = 10.0;

/* table */
static const GLdouble _WALL_SIZE = 10.0;
static const GLdouble _FLOOR_WIDTH = 150.0;
static const GLdouble _FLOOR_HEIGHT = 300.0;

/* floor */
static const GLdouble _FLOOR_CENTER_VERTEX[][3] = {
	{ -_FLOOR_WIDTH / 2 + _HOLE_SIZE, -_FLOOR_HEIGHT / 2, -_WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2 - _HOLE_SIZE,  -_FLOOR_HEIGHT / 2, -_WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2 - _HOLE_SIZE,  _FLOOR_HEIGHT / 2,  -_WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2 + _HOLE_SIZE, _FLOOR_HEIGHT / 2,  -_WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2 + _HOLE_SIZE, -_FLOOR_HEIGHT / 2, 0.0 },
	{ _FLOOR_WIDTH / 2 - _HOLE_SIZE,  -_FLOOR_HEIGHT / 2, 0.0 },
	{ _FLOOR_WIDTH / 2 - _HOLE_SIZE,  _FLOOR_HEIGHT / 2,  0.0 },
	{ -_FLOOR_WIDTH / 2 + _HOLE_SIZE, _FLOOR_HEIGHT / 2,  0.0 },
};
static const GLdouble _FLOOR_TL_VERTEX[][3] = {
	{ -_FLOOR_WIDTH / 2,              _HOLE_SIZE,                      -_WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2 + _HOLE_SIZE, 0.0,                             -_WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2 + _HOLE_SIZE, _FLOOR_HEIGHT / 2 - _HOLE_SIZE,  -_WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2,			  _FLOOR_HEIGHT / 2 - _HOLE_SIZE,  -_WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2,              _HOLE_SIZE,                      0.0 },
	{ -_FLOOR_WIDTH / 2 + _HOLE_SIZE, 0.0,                             0.0 },
	{ -_FLOOR_WIDTH / 2 + _HOLE_SIZE, _FLOOR_HEIGHT / 2 - _HOLE_SIZE,  0.0 },
	{ -_FLOOR_WIDTH / 2,			  _FLOOR_HEIGHT / 2 - _HOLE_SIZE,  0.0 },
};
static const GLdouble _FLOOR_BL_VERTEX[][3] = {
	{ -_FLOOR_WIDTH / 2 + _HOLE_SIZE, 0.0,                             -_WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2,              -_HOLE_SIZE,                     -_WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2,			  -_FLOOR_HEIGHT / 2 + _HOLE_SIZE, -_WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2 + _HOLE_SIZE, -_FLOOR_HEIGHT / 2 + _HOLE_SIZE, -_WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2 + _HOLE_SIZE, 0.0,                             0.0 },
	{ -_FLOOR_WIDTH / 2,              -_HOLE_SIZE,                     0.0 },
	{ -_FLOOR_WIDTH / 2,			  -_FLOOR_HEIGHT / 2 + _HOLE_SIZE, 0.0 },
	{ -_FLOOR_WIDTH / 2 + _HOLE_SIZE, -_FLOOR_HEIGHT / 2 + _HOLE_SIZE, 0.0 },
};
static const GLdouble _FLOOR_TR_VERTEX[][3] = {
	{ _FLOOR_WIDTH / 2 - _HOLE_SIZE,  -_FLOOR_HEIGHT / 2 + _HOLE_SIZE, -_WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2,               -_FLOOR_HEIGHT / 2 + _HOLE_SIZE, -_WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2,			      -_HOLE_SIZE,                     -_WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2 - _HOLE_SIZE,  0.0,                             -_WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2 - _HOLE_SIZE,  -_FLOOR_HEIGHT / 2 + _HOLE_SIZE, 0.0 },
	{ _FLOOR_WIDTH / 2,               -_FLOOR_HEIGHT / 2 + _HOLE_SIZE, 0.0 },
	{ _FLOOR_WIDTH / 2,			      -_HOLE_SIZE,                     0.0 },
	{ _FLOOR_WIDTH / 2 - _HOLE_SIZE,  0.0,                             0.0 },
};
static const GLdouble _FLOOR_BR_VERTEX[][3] = {
	{ _FLOOR_WIDTH / 2 - _HOLE_SIZE,  0.0,                            -_WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2,               _HOLE_SIZE,                     -_WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2,			      _FLOOR_HEIGHT / 2 - _HOLE_SIZE, -_WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2 - _HOLE_SIZE,  _FLOOR_HEIGHT / 2 - _HOLE_SIZE, -_WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2 - _HOLE_SIZE,  0.0,                            0.0 },
	{ _FLOOR_WIDTH / 2,               _HOLE_SIZE,                     0.0 },
	{ _FLOOR_WIDTH / 2,			      _FLOOR_HEIGHT / 2 - _HOLE_SIZE, 0.0 },
	{ _FLOOR_WIDTH / 2 - _HOLE_SIZE,  _FLOOR_HEIGHT / 2 - _HOLE_SIZE, 0.0 },
};

/* wall */
static const GLdouble _TRAPEZOID_SIZE = _WALL_SIZE / 2;
static const GLdouble _WALL_TOP_POS[]    = { 0.0, _FLOOR_HEIGHT / 2 + _WALL_SIZE * (3.0/2.0),    -_WALL_SIZE / 4 };
static const GLdouble _WALL_BOTTOM_POS[] = { 0.0, -(_FLOOR_HEIGHT / 2 + _WALL_SIZE * (3.0 / 2.0)), -_WALL_SIZE / 4 };
static const GLdouble _WALL_LEFT_POS[] = { -(_FLOOR_WIDTH / 2 + _WALL_SIZE * (3.0 / 2.0)), 0.0,  -_WALL_SIZE / 4 };
static const GLdouble _WALL_RIGHT_POS[]   = { _FLOOR_WIDTH / 2 + _WALL_SIZE * (3.0 / 2.0),    0.0,  -_WALL_SIZE / 4 };
static const GLdouble _TRAPEZOID_TOP_VERTEX[][3] = {
	{ -_FLOOR_WIDTH / 2 + _HOLE_SIZE, _FLOOR_HEIGHT / 2,              -_WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2 - _HOLE_SIZE,  _FLOOR_HEIGHT / 2,              -_WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2,               _FLOOR_HEIGHT / 2 + _WALL_SIZE, -_WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2,              _FLOOR_HEIGHT / 2 + _WALL_SIZE, -_WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2 + _HOLE_SIZE, _FLOOR_HEIGHT / 2,              _WALL_SIZE / 2 - 2.0 },
	{ _FLOOR_WIDTH / 2 - _HOLE_SIZE,  _FLOOR_HEIGHT / 2 ,             _WALL_SIZE / 2 - 2.0 },
	{ _FLOOR_WIDTH / 2,               _FLOOR_HEIGHT / 2 + _WALL_SIZE, _WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2,              _FLOOR_HEIGHT / 2 + _WALL_SIZE, _WALL_SIZE / 2 },
};
static const GLdouble _TRAPEZOID_BOTTOM_VERTEX[][3] = {
	{ -_FLOOR_WIDTH / 2,              -_FLOOR_HEIGHT / 2 - _WALL_SIZE, -_WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2,               -_FLOOR_HEIGHT / 2 - _WALL_SIZE, -_WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2 - _HOLE_SIZE,  -_FLOOR_HEIGHT / 2,              -_WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2 + _HOLE_SIZE, -_FLOOR_HEIGHT / 2,              -_WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2,              -_FLOOR_HEIGHT / 2 - _WALL_SIZE, _WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2,               -_FLOOR_HEIGHT / 2 - _WALL_SIZE, _WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2 - _HOLE_SIZE,  -_FLOOR_HEIGHT / 2,              _WALL_SIZE / 2 - 2.0 },
	{ -_FLOOR_WIDTH / 2 + _HOLE_SIZE, -_FLOOR_HEIGHT / 2,              _WALL_SIZE / 2 - 2.0 },
};
static const GLdouble _TRAPEZOID_TL_VERTEX[][3] = {
	{ -_FLOOR_WIDTH / 2 - _WALL_SIZE, _HOLE_SIZE,                      -_WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2,              _HOLE_SIZE,                      -_WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2,              _FLOOR_HEIGHT / 2 - _HOLE_SIZE,  -_WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2 - _WALL_SIZE, _FLOOR_HEIGHT / 2,               -_WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2 - _WALL_SIZE, _HOLE_SIZE,                      _WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2,              _HOLE_SIZE,                      _WALL_SIZE / 2 - 2.0 },
	{ -_FLOOR_WIDTH / 2,              _FLOOR_HEIGHT / 2 - _HOLE_SIZE,  _WALL_SIZE / 2 - 2.0 },
	{ -_FLOOR_WIDTH / 2 - _WALL_SIZE, _FLOOR_HEIGHT / 2,               _WALL_SIZE / 2 },
};
static const GLdouble _TRAPEZOID_BL_VERTEX[][3] = {
	{ -_FLOOR_WIDTH / 2 - _WALL_SIZE, -_FLOOR_HEIGHT / 2,              -_WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2,              -_FLOOR_HEIGHT / 2 + _HOLE_SIZE, -_WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2,              -_HOLE_SIZE,                     -_WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2 - _WALL_SIZE, -_HOLE_SIZE,                     -_WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2 - _WALL_SIZE, -_FLOOR_HEIGHT / 2,              _WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2,              -_FLOOR_HEIGHT / 2 + _HOLE_SIZE, _WALL_SIZE / 2 - 2.0 },
	{ -_FLOOR_WIDTH / 2,              -_HOLE_SIZE,                     _WALL_SIZE / 2 - 2.0 },
	{ -_FLOOR_WIDTH / 2 - _WALL_SIZE, -_HOLE_SIZE,                     _WALL_SIZE / 2 },
};
static const GLdouble _TRAPEZOID_TR_VERTEX[][3] = {
	{ _FLOOR_WIDTH / 2,              _HOLE_SIZE,                      -_WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2 + _WALL_SIZE, _HOLE_SIZE,                      -_WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2 + _WALL_SIZE, _FLOOR_HEIGHT / 2,               -_WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2,              _FLOOR_HEIGHT / 2 - _HOLE_SIZE,  -_WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2,              _HOLE_SIZE,                      _WALL_SIZE / 2 - 2.0 },
	{ _FLOOR_WIDTH / 2 + _WALL_SIZE, _HOLE_SIZE,                      _WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2 + _WALL_SIZE, _FLOOR_HEIGHT / 2,               _WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2,              _FLOOR_HEIGHT / 2 - _HOLE_SIZE,  _WALL_SIZE / 2 - 2.0 },
};
static const GLdouble _TRAPEZOID_BR_VERTEX[][3] = {
	{ _FLOOR_WIDTH / 2,              -_FLOOR_HEIGHT / 2 + _HOLE_SIZE, -_WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2 + _WALL_SIZE, -_FLOOR_HEIGHT / 2,              -_WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2 + _WALL_SIZE, -_HOLE_SIZE,                     -_WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2,              -_HOLE_SIZE,                     -_WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2,              -_FLOOR_HEIGHT / 2 + _HOLE_SIZE, _WALL_SIZE / 2 - 2.0 },
	{ _FLOOR_WIDTH / 2 + _WALL_SIZE, -_FLOOR_HEIGHT / 2,              _WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2 + _WALL_SIZE, -_HOLE_SIZE,                     _WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2,              -_HOLE_SIZE,                     _WALL_SIZE / 2 - 2.0 },
};
static const GLdouble _CORNER_BL_VERTEX[][3] = {
	{ -_FLOOR_WIDTH / 2 - _WALL_SIZE * 2, -_FLOOR_HEIGHT / 2,                  -_WALL_SIZE },
	{ -_FLOOR_WIDTH / 2,                  -_FLOOR_HEIGHT / 2 - _WALL_SIZE * 2, -_WALL_SIZE },
	{ -_FLOOR_WIDTH / 2,                  -_FLOOR_HEIGHT / 2 - _WALL_SIZE,     -_WALL_SIZE },
	{ -_FLOOR_WIDTH / 2 - _WALL_SIZE,     -_FLOOR_HEIGHT / 2,                  -_WALL_SIZE },
	{ -_FLOOR_WIDTH / 2 - _WALL_SIZE * 2, -_FLOOR_HEIGHT / 2,                  _WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2,                  -_FLOOR_HEIGHT / 2 - _WALL_SIZE * 2, _WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2,                  -_FLOOR_HEIGHT / 2 - _WALL_SIZE,     _WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2 - _WALL_SIZE,     -_FLOOR_HEIGHT / 2,                  _WALL_SIZE / 2 }
};
static const GLdouble _CORNER_BR_VERTEX[][3] = {
	{ _FLOOR_WIDTH / 2,                  -_FLOOR_HEIGHT / 2 - _WALL_SIZE * 2, -_WALL_SIZE },
	{ _FLOOR_WIDTH / 2 + _WALL_SIZE * 2, -_FLOOR_HEIGHT / 2,                  -_WALL_SIZE },
	{ _FLOOR_WIDTH / 2 + _WALL_SIZE,     -_FLOOR_HEIGHT / 2,                  -_WALL_SIZE },
	{ _FLOOR_WIDTH / 2,                  -_FLOOR_HEIGHT / 2 - _WALL_SIZE,     -_WALL_SIZE },
	{ _FLOOR_WIDTH / 2,                  -_FLOOR_HEIGHT / 2 - _WALL_SIZE * 2, _WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2 + _WALL_SIZE * 2, -_FLOOR_HEIGHT / 2,                  _WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2 + _WALL_SIZE,     -_FLOOR_HEIGHT / 2,                  _WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2,                  -_FLOOR_HEIGHT / 2 - _WALL_SIZE,     _WALL_SIZE / 2 }
};
static const GLdouble _CORNER_TL_VERTEX[][3] = {
	{ -_FLOOR_WIDTH / 2 - _WALL_SIZE * 2, _FLOOR_HEIGHT / 2,                  -_WALL_SIZE },
	{ -_FLOOR_WIDTH / 2 - _WALL_SIZE,     _FLOOR_HEIGHT / 2,                  -_WALL_SIZE },
	{ -_FLOOR_WIDTH / 2,                  _FLOOR_HEIGHT / 2 + _WALL_SIZE,     -_WALL_SIZE },
	{ -_FLOOR_WIDTH / 2,                  _FLOOR_HEIGHT / 2 + _WALL_SIZE * 2, -_WALL_SIZE },
	{ -_FLOOR_WIDTH / 2 - _WALL_SIZE * 2, _FLOOR_HEIGHT / 2,                  _WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2 - _WALL_SIZE,     _FLOOR_HEIGHT / 2,                  _WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2,                  _FLOOR_HEIGHT / 2 + _WALL_SIZE,     _WALL_SIZE / 2 },
	{ -_FLOOR_WIDTH / 2,                  _FLOOR_HEIGHT / 2 + _WALL_SIZE * 2, _WALL_SIZE / 2 }
};
static const GLdouble _CORNER_TR_VERTEX[][3] = {
	{ _FLOOR_WIDTH / 2,                  _FLOOR_HEIGHT / 2 + _WALL_SIZE,     -_WALL_SIZE },
	{ _FLOOR_WIDTH / 2 + _WALL_SIZE,     _FLOOR_HEIGHT / 2,                  -_WALL_SIZE },
	{ _FLOOR_WIDTH / 2 + _WALL_SIZE * 2, _FLOOR_HEIGHT / 2,                  -_WALL_SIZE },
	{ _FLOOR_WIDTH / 2,                  _FLOOR_HEIGHT / 2 + _WALL_SIZE * 2, -_WALL_SIZE },
	{ _FLOOR_WIDTH / 2,                  _FLOOR_HEIGHT / 2 + _WALL_SIZE,     _WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2 + _WALL_SIZE,     _FLOOR_HEIGHT / 2,                  _WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2 + _WALL_SIZE * 2, _FLOOR_HEIGHT / 2,                  _WALL_SIZE / 2 },
	{ _FLOOR_WIDTH / 2,                  _FLOOR_HEIGHT / 2 + _WALL_SIZE * 2, _WALL_SIZE / 2 }
};

/* bottom floor */
static const GLdouble _BOTTOM_FLOOR_POS[] = { 0.0, 0.0, -_WALL_SIZE * (3.0 / 4.0) };

/* ball */
static const GLdouble _FRICTION         = 0.995;
static const GLdouble _IN_HOLE_VEC      = 0.3;
static const GLdouble _VEC_THRESHOLD    = 0.01;
static const GLdouble _TIME_TO_VEC      = 0.01;
static const GLdouble _BALL_RADIUS      = 4.0;
static const GLdouble _BALL_DIAMOND_TOP = 10.0;
static const GLint    _BALL_NUM         = 10;
static const GLdouble _PUSH_SPACE_LIMIT = 0.03;
static const GLdouble _GRAVITY          = -0.02;
static const GLdouble _COLLISION_PUSH   = 0.5;
static const GLdouble _VEC_ROTAT_ADJUST = 10.0;

static const GLdouble _BALL_POS[][3] = {
	{ 0.0,                     -_FLOOR_HEIGHT / 4,                   _BALL_RADIUS },
	{ 0.0,                     _BALL_DIAMOND_TOP,                    _BALL_RADIUS },
	{ -_BALL_RADIUS - 1.0,     _BALL_DIAMOND_TOP + 2 * _BALL_RADIUS, _BALL_RADIUS },
	{ _BALL_RADIUS + 1.0,      _BALL_DIAMOND_TOP + 2 * _BALL_RADIUS, _BALL_RADIUS },
	{ -_BALL_RADIUS * 2 - 1.0, _BALL_DIAMOND_TOP + 4 * _BALL_RADIUS, _BALL_RADIUS },
	{ 0.0,                     _BALL_DIAMOND_TOP + 4 * _BALL_RADIUS, _BALL_RADIUS },
	{ _BALL_RADIUS * 2 + 1.0,  _BALL_DIAMOND_TOP + 4 * _BALL_RADIUS, _BALL_RADIUS },
	{ -_BALL_RADIUS - 1.0,     _BALL_DIAMOND_TOP + 6 * _BALL_RADIUS, _BALL_RADIUS },
	{ _BALL_RADIUS + 1.0,      _BALL_DIAMOND_TOP + 6 * _BALL_RADIUS, _BALL_RADIUS },
	{ 0.0,                     _BALL_DIAMOND_TOP + 8 * _BALL_RADIUS, _BALL_RADIUS }
};

static const char *_BALL_TEXTURE[] = {
	"Image\\BallCue.jpg",
	"Image\\Ball1.jpg",
	"Image\\Ball2.jpg",
	"Image\\Ball3.jpg",
	"Image\\Ball4.jpg",
	"Image\\Ball5.jpg",
	"Image\\Ball6.jpg",
	"Image\\Ball7.jpg",
	"Image\\Ball8.jpg",
	"Image\\Ball9.jpg"
};

/* que */
static const GLdouble _QUE_RADIUS     = 2.0;
static const GLdouble _QUE_HEIGHT     = 80.0;
static const size_t   _QUE_SIDES      = 10;
static const GLdouble _QUE_PULL       = _QUE_HEIGHT + 2.0;
static const GLdouble _QUE_POS_HEIGHT = 26.0;
static const GLdouble _QUE_ROTATE     = 15.0;
static const GLdouble _QUE_ADD_PULL   = 0.1;

/* sight */
static const GLdouble _SIGHT_RADIUS = 300.0;
static const GLdouble _SIGHT_PLUS   = 0.03; //radian
static const GLdouble _SIGHT_POS_Z  = 200.0;

static const GLdouble _SIGHT_CENTER_POS_INIT[] = { 0.0, -_FLOOR_HEIGHT / 4, _BALL_RADIUS };
static const GLdouble _SIGHT_VEC[] = { 0.0, 0.0, 1.0 };

/* keyborad ASCII Code */
static const int _KEY_SPACE = 040;
static const int _KEY_ESC   = 033;

