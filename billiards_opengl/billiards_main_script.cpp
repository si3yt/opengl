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
using namespace std;

//----------------------------------------------------
// �ϐ��錾
//----------------------------------------------------
/* �E�B���h�E�ϐ� */
static int  _WINDOW_POSITION_X = 100; 
static int  _WINDOW_POSITION_Y = 100;
static int  _WINDOW_WIDTH      = 512;
static int  _WINDOW_HEIGHT     = 512; 
static char _WINDOW_TITLE[]    = "billiards";

/* �}�E�X��] */
#define SCALE (2.0 * 3.14159265358979323846)	// �}�E�X�̑��Έʒu����]�p�̊��Z�W��
int    cx, cy;									// �h���b�O�J�n�ʒu
double sx, sy;									// �}�E�X�̐�Έʒu���E�B���h�E���ł̑��Έʒu�̊��Z�W��
double cq[4] = { 1.0, 0.0, 0.0, 0.0 };			// ��]�̏����l (�N�H�[�^�j�I��)
double tq[4];									// �h���b�O���̉�] (�N�H�[�^�j�I��)
double rt[16];									// ��]�̕ϊ��s��
float  camera_z_pos = 50.0;

/* �f�B�X�v���C���X�g */
GLuint list_number;

/* ��������(�R�{���u���̍\���̂𗘗p) */
struct _MATERIAL_STRUCT {							// material�p�\����
	GLfloat ambient[4];		// ������
	GLfloat diffuse[4];		// �g�U����
	GLfloat specular[4];	// ���ʔ���
	GLfloat shininess;		// ����
};
_MATERIAL_STRUCT _MS_WHITE_PLASTIC = {				//�v���X�`�b�N(��)
	{ 0.0,  0.0,  0.0,  1.0 },
	{ 0.55, 0.55, 0.55, 1.0 },
	{ 0.70, 0.70, 0.70, 1.0 },
	32 };
_MATERIAL_STRUCT _MS_RUBY = {						// ruby(���r�[)
	{ 0.1745,   0.01175,  0.01175,   1.0 },
	{ 0.61424,  0.04136,  0.04136,   1.0 },
	{ 0.727811, 0.626959, 0.626959,  1.0 },
	76.8 };

/* �F */
GLfloat _RED[]     = { 0.8, 0.2, 0.2, 1.0 };	//�ԐF
GLfloat _GREEN[]   = { 0.2, 0.8, 0.2, 1.0 };	//�ΐF
GLfloat _BLUE[]    = { 0.2, 0.2, 0.8, 1.0 };	//�F
GLfloat _YELLOW[]  = { 0.8, 0.8, 0.2, 1.0 };	//���F
GLfloat _WHITE[]   = { 1.0, 1.0, 1.0, 1.0 };	//���F
GLfloat _BLACK[]   = { 0.0, 0.0, 0.0, 1.0 };	//���F
GLfloat _SHININESS = 30.0;						//����̋���

/* �� */
static GLfloat _LIGHT_POSITION_0[] = { 0.0, 0.0, 150.0, 1.0 }; //����0�̍��W

/* �� */
static GLfloat floor_planar[4];			//��
static GLfloat _FLOOR_S = 50.0f;		//���T�C�Y�p�W��
static GLfloat _WALL_S = 10.0f;			//�ǃT�C�Y�p�W��
static GLfloat pM[16];					//shadowMatrix�Fm[]
static GLfloat _FLOOR_OFFSET = 12.0f;	//���p���I�t�Z�b�g

typedef struct _QUADS_VERTEX {
	GLfloat v0[3];
	GLfloat v1[3];
	GLfloat v2[3];
	GLfloat v3[3];
};
static _QUADS_VERTEX _FLOOR_VER = {		//�e�`��p���ݒ�
	{ _FLOOR_S,  _FLOOR_S,  0.0f },
	{ -_FLOOR_S, _FLOOR_S,  0.0f },
	{ -_FLOOR_S, -_FLOOR_S, 0.0f },
	{ _FLOOR_S,  -_FLOOR_S, 0.0f },
};

static GLdouble _FLOOR_VERTEX[][3] = {					//�����_
	{ _FLOOR_S  -_FLOOR_OFFSET, _FLOOR_S*2,					0.0f },
	{ -_FLOOR_S +_FLOOR_OFFSET, _FLOOR_S*2,					0.0f },
	{ -_FLOOR_S +_FLOOR_OFFSET, _FLOOR_S*2  -_FLOOR_OFFSET, 0.0f },
	{ -_FLOOR_S,				_FLOOR_S*2  -_FLOOR_OFFSET, 0.0f },
	{ -_FLOOR_S,				-_FLOOR_S*2 +_FLOOR_OFFSET, 0.0f },
	{ -_FLOOR_S +_FLOOR_OFFSET, -_FLOOR_S*2 +_FLOOR_OFFSET, 0.0f },
	{ -_FLOOR_S +_FLOOR_OFFSET, -_FLOOR_S*2,				0.0f },
	{ _FLOOR_S  -_FLOOR_OFFSET, -_FLOOR_S*2,				0.0f },
	{ _FLOOR_S  -_FLOOR_OFFSET, -_FLOOR_S*2 +_FLOOR_OFFSET, 0.0f },
	{ _FLOOR_S,				    -_FLOOR_S*2 +_FLOOR_OFFSET, 0.0f },
	{ _FLOOR_S,				    _FLOOR_S*2  -_FLOOR_OFFSET, 0.0f },
	{ _FLOOR_S  -_FLOOR_OFFSET, _FLOOR_S*2  -_FLOOR_OFFSET, 0.0f },
};
static GLint _FLOOR_FACE[][4] = {						//����
	{0,  1,  6,  7},
	{2,  3,  4,  5},
	{10, 11, 8,  9}
};

/* �� */
static GLdouble _HOLE_VERTEX[][3] = {					//�����_
	{ -_FLOOR_S, -_FLOOR_S*2, -_WALL_S   },
	{ _FLOOR_S,  -_FLOOR_S*2, -_WALL_S   },
	{ _FLOOR_S,  _FLOOR_S*2,  -_WALL_S   },
	{ -_FLOOR_S, _FLOOR_S*2,  -_WALL_S   },
	{ -_FLOOR_S, -_FLOOR_S*2, -_WALL_S/2 },
	{ _FLOOR_S,  -_FLOOR_S*2, -_WALL_S/2 },
	{ _FLOOR_S,  _FLOOR_S*2,  -_WALL_S/2 },
	{ -_FLOOR_S, _FLOOR_S*2,  -_WALL_S/2 },
};

/* �� */
static GLdouble _WALL_TOP_VERTEX[][3] = {				// ��ǒ��_
	{ -_FLOOR_S, _FLOOR_S*2,		  -_WALL_S },
	{ _FLOOR_S,  _FLOOR_S*2,		  -_WALL_S },
	{ _FLOOR_S,  _FLOOR_S*2 +_WALL_S, -_WALL_S },
	{ -_FLOOR_S, _FLOOR_S*2 +_WALL_S, -_WALL_S },
	{ -_FLOOR_S, _FLOOR_S*2,		  _WALL_S  },
	{ _FLOOR_S,  _FLOOR_S*2,		  _WALL_S  },
	{ _FLOOR_S,  _FLOOR_S*2 +_WALL_S, _WALL_S  },
	{ -_FLOOR_S, _FLOOR_S*2 +_WALL_S, _WALL_S  },
};
static GLdouble _WALL_BOTTOM_VERTEX[][3] = {			// ���ǒ��_
	{ -_FLOOR_S, -_FLOOR_S*2 -_WALL_S, -_WALL_S },
	{ _FLOOR_S,  -_FLOOR_S*2 -_WALL_S, -_WALL_S },
	{ _FLOOR_S,  -_FLOOR_S*2,		   -_WALL_S },
	{ -_FLOOR_S, -_FLOOR_S*2,		   -_WALL_S },
	{ -_FLOOR_S, -_FLOOR_S*2 -_WALL_S, _WALL_S  },
	{ _FLOOR_S,  -_FLOOR_S*2 -_WALL_S, _WALL_S  },
	{ _FLOOR_S,  -_FLOOR_S*2,		   _WALL_S  },
	{ -_FLOOR_S, -_FLOOR_S*2,		   _WALL_S  },
};
static GLdouble _WALL_LEFT_VERTEX[][3] = {				// ���ǒ��_
	{ -_FLOOR_S -_WALL_S, -_FLOOR_S*2 -_WALL_S,	-_WALL_S },
	{ -_FLOOR_S,		  -_FLOOR_S*2 -_WALL_S,	-_WALL_S },
	{ -_FLOOR_S,          _FLOOR_S*2  +_WALL_S, -_WALL_S },
	{ -_FLOOR_S -_WALL_S, _FLOOR_S*2  +_WALL_S, -_WALL_S },
	{ -_FLOOR_S -_WALL_S, -_FLOOR_S*2 -_WALL_S,	_WALL_S  },
	{ -_FLOOR_S,		  -_FLOOR_S*2 -_WALL_S,	_WALL_S  },
	{ -_FLOOR_S,		  _FLOOR_S*2  +_WALL_S, _WALL_S  },
	{ -_FLOOR_S -_WALL_S, _FLOOR_S*2  +_WALL_S, _WALL_S  },
};
static GLdouble _WALL_RIGHT_VERTEX[][3] = {				// �E�ǒ��_
	{ _FLOOR_S,		     -_FLOOR_S*2 -_WALL_S, -_WALL_S },
	{ _FLOOR_S +_WALL_S, -_FLOOR_S*2 -_WALL_S, -_WALL_S },
	{ _FLOOR_S +_WALL_S, _FLOOR_S*2  +_WALL_S, -_WALL_S },
	{ _FLOOR_S,		     _FLOOR_S*2  +_WALL_S, -_WALL_S },
	{ _FLOOR_S,		     -_FLOOR_S*2 -_WALL_S, _WALL_S  },
	{ _FLOOR_S +_WALL_S, -_FLOOR_S*2 -_WALL_S, _WALL_S  },
	{ _FLOOR_S +_WALL_S, _FLOOR_S*2  +_WALL_S, _WALL_S  },
	{ _FLOOR_S,		     _FLOOR_S*2  +_WALL_S, _WALL_S  },
};

/* �l�p�` */
static GLint _QUADS_FACE[][4] = {			// ��(���_�̏���)
	{ 0, 1, 2, 3 },
	{ 1, 5, 6, 2 },
	{ 5, 4, 7, 6 },
	{ 4, 0, 3, 7 },
	{ 4, 5, 1, 0 },
	{ 3, 2, 6, 7 }
};
typedef struct _FACE_NORMAL {
	GLfloat n0[3];
	GLfloat n1[3];
	GLfloat n2[3];
	GLfloat n3[3];
	GLfloat n4[3];
	GLfloat n5[3];
};
static _FACE_NORMAL _QUADS_FACE_NOMAL = {	//�ʂ̖@���x�N�g��
	{ 0.0,  0.0,  -1.0 },
	{ 1.0,  0.0,  0.0  },
	{ 0.0,  0.0,  1.0  },
	{ -1.0, 0.0,  0.0  },
	{ 0.0,  -1.0, 0.0  },
	{ 0.0,  1.0,  0.0  }
};

/* �� */
static GLfloat  _SPHERE_RADIUS  = 4.0;		//���̔��a
static GLint    _SPHERE_NUMBER  = 10;		//���̌�
static GLdouble _FRICTION       = 0.995;	//���C�ɂ�鑬�x����
static GLdouble _POS_ADJUSTMENT = 0.01;		//�x�N�g���ʒu���f�p�W��
static GLdouble _ADD_FORCE_DT   = 0.02;		//add force�p�W��
GLdouble		add_force       = 0.0;		//player ball �����o���ϐ�
static GLdouble _GRAVITY        = -10.0;	//�d��
static GLdouble _COL_PUSH       = 0.5;		//�R���W�����������̉����o������

struct {									//�\���́Fp ���̍\����
	GLdouble x, y, z;
	GLdouble vx, vy, vz;
	GLdouble fx, fy;
	bool     h_flag, d_flag;
}p[10];
struct {									//�\���́Fcol_p ���̃R���W�������Z�p�\����
	GLdouble x, y, op_x, op_y;
}col_p[10];

std::vector<GLint> opponent_num;			//�R���W����

bool enable_play = true;					//�v���C�\

/* �L���[ */
static GLdouble _CYLINDER_RADIUS = 3.0;
static GLdouble _CYLINDER_HEIGHT = 50.0;
static GLint	_CYLINDER_SIDES  = 10;

/* ���� */
bool space_key_pressing = false;			//�X�y�[�X�L�[���������m
bool left_key_pressing  = false;			//LEFT�L�[���������m
bool right_key_pressing = false;			//RIGHT�L�[���������m
GLdouble look_x		  = 0.0;
GLdouble look_y		  = 0.0;
static GLdouble _LOOK_PLUS   = 5.0;
GLdouble look_sign    = -1.0;
static GLdouble _LOOK_RADIUS = 300.0;
GLdouble look_angle   = 0.0;

//----------------------------------------------------
// �֐��v���g�^�C�v�i��ɌĂяo���֐����ƈ����̐錾�j
//----------------------------------------------------
/* �K�v�֐� */
void Initialize(void);
void Idle(void);
void Display(void);
void Resize(int w, int h);

/* ���̕`�� */
void InitialSphere(void);
void DrawSphereOfPlayer(void);
void DrawSphereOfTarget(void);
void DrawTable(void);
void DrawWall(void);
void DrawHole(void);
void DrawFloor(void);
void DrawCylinder(void);

/* ���Z */
void SphereAddForce(GLint n);
bool CollisionSphere(GLint n);
void CollisionSphereProcess(GLint n, GLint op_n, GLdouble angle);
void NotCollisionPosition(GLint n, GLint op_n);
void CollisionWall(GLint n);
void CollisionHole(GLint n);
bool CollisionFloor(GLint n);

/* �e(�X�e���V��) */
void FindPlane(GLfloat plane[4], GLfloat v0[3], GLfloat v1[3], GLfloat v2[3]);
void ShadowMatrix(GLfloat *m, GLfloat plane[4], GLfloat light[4]);
void DrawShadow(void);

/* ���_��] */
void Qmul(double r[], const double p[], const double q[]);
void Qrot(double r[], double q[]);

/* ���� */
void Keyboard(unsigned char key, int x, int y);
void KeyboardUp(unsigned char key, int x, int y);
void SpecialFunc(int key, int x, int y);
void SpecialUpFunc(int key, int x, int y);
void MouseMotion(int x, int y);
void MouseOn(int button, int state, int x, int y);
void MouseWheel(float z);

//----------------------------------------------------
// ���C���֐�
//----------------------------------------------------
GLint main(int argc, char *argv[]) {
	srand((unsigned)time(NULL));									//�����̔���
	glutInit(&argc, argv);											//���̏�����
	glutInitWindowPosition(_WINDOW_POSITION_X, _WINDOW_POSITION_Y);	//�E�B���h�E�̈ʒu�̎w��
	glutInitWindowSize(_WINDOW_WIDTH, _WINDOW_HEIGHT);				//�E�B���h�E�T�C�Y�̎w��
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);		//�f�B�X�v���C���[�h�̎w��(RGBA,�f�v�X�o�b�t�@,�_�u���o�b�t�@)
	glutCreateWindow(_WINDOW_TITLE);								//�E�B���h�E�̍쐬
	glutDisplayFunc(Display);										//�`�掞�ɌĂяo�����֐�
	glutReshapeFunc(Resize);										//���T�C�Y���ɌĂяo�����֐�
	glutKeyboardFunc(Keyboard);										//�L�[�{�[�h���͎��ɌĂяo�����֐�
	glutKeyboardUpFunc(KeyboardUp);									//�L�[�{�[�h�������ꂽ���ɌĂяo�����֐�
	glutSpecialFunc(SpecialFunc);									//Special�L�[�{�[�h���͎��ɌĂяo�����֐�
	glutSpecialUpFunc(SpecialUpFunc);								//Special�L�[�{�[�h�������ꂽ���ɌĂяo�����֐�
	glutIgnoreKeyRepeat(GL_TRUE);									//�L�[�̌J��Ԃ����͖͂���
	glutMouseFunc(MouseOn);											//�}�E�X�N���b�N���ɌĂяo�����֐�
	glutMotionFunc(MouseMotion);									//�}�E�X�h���b�O�������ɌĂяo�����֐�

	glutIdleFunc(Idle);												//�v���O�����A�C�h����Ԏ��ɌĂяo�����֐�
	Initialize();													//�����ݒ�̊֐����Ăяo��
	glutMainLoop();													//�C�x���g�҂��󂯏��(�������[�v)
	return 0;
}
//----------------------------------------------------
// �����ݒ�̊֐�
//----------------------------------------------------
void Initialize(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);	//�w�i�F(RGBA 0.0~1.0)
	glEnable(GL_DEPTH_TEST);			//�f�v�X�o�b�t�@���g�p�FglutInitDisplayMode() �� GLUT_DEPTH ���w�肷��
	glDepthFunc(GL_LEQUAL);				//�[�x�o�b�t�@�ƐV�����s�N�Z���n�̐[�x�̔�r�֐�(GL_LEQUAL�F�i�[�[�x�ȉ��ł���Βʉ�)
	glClearDepth(1.0);

	FindPlane(floor_planar,				//�X�e���V���𒣂邽�߂̏���������
		_FLOOR_VER.v0,
		_FLOOR_VER.v1,
		_FLOOR_VER.v2);

	/* �����̐ݒ� */
	glEnable(GL_LIGHTING);									//�A�eON
	glEnable(GL_LIGHT0);									//����0�𗘗p
	glLightfv(GL_LIGHT0, GL_POSITION, _LIGHT_POSITION_0);	//����0�̈ʒu

	/* �f�B�X�v���C���X�g���쐬 */
	list_number = glGenLists(1);
	glNewList(list_number, GL_COMPILE);	//�R���p�C���̂�
	glEndList();

	/* �}�E�X�|�C���^�ʒu�̃E�B���h�E���̑��ΓI�ʒu�ւ̊��Z�p */
	sx = 1.0 / (double)512;
	sy = 1.0 / (double)512;

	/* ��]�s��̏����� */
	Qrot(rt, cq);

	/* �X�e���V���o�b�t�@�N���A�l�̐ݒ� */
	glClearStencil(0);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	/* ���ʎˉe�s��̎Z�o */
	ShadowMatrix(pM, floor_planar, _LIGHT_POSITION_0);

	/* ���̔z�u */
	InitialSphere();

}

//----------------------------------------------------
// �A�C�h�����ɌĂяo�����֐�
//----------------------------------------------------
void Idle() {
	glutPostRedisplay(); //glutDisplayFunc()���P����s����

	if (space_key_pressing) {		//�p������addforce��������
		if (add_force <= 3500) {
			add_force += 15;
		}
	}

	if (look_x == 0){
		look_y = look_sign * _LOOK_RADIUS;
	}else {
		look_y = look_sign * sqrt((_LOOK_RADIUS*_LOOK_RADIUS) - (look_x*look_x));
	}
	if (abs(look_x) >= _LOOK_RADIUS) {
		_LOOK_PLUS = -_LOOK_PLUS;
		look_sign = look_sign * -1.0;
	}
	if (left_key_pressing) {			//�p�����Ď��_�ړ�
		look_x -= _LOOK_PLUS;
	} else if (right_key_pressing) {
		look_x += _LOOK_PLUS;
	}
	look_angle = atan2(p[0].y - look_y, p[0].x - look_x);

	for (GLint i = 0; i < _SPHERE_NUMBER; i++) {
		if (p[i].vx == 0.0 && p[i].vy == 0.0) {
			enable_play = true;
		} else {
			enable_play = false;
			break;
		}
	}
}

//----------------------------------------------------
// ���T�C�Y�֐�
//----------------------------------------------------
void Resize(int w, int h) {
	glViewport(0, 0, w, h);	//�r���[�|�[�g�̐ݒ�

	/* �����ϊ��s��̐ݒ� */
	glMatrixMode(GL_PROJECTION);	//�s�񃂁[�h�̐ݒ�GL_PROJECTION : �����ϊ��s��̐ݒ�AGL_MODELVIEW�F���f���r���[�ϊ��s��j
	glLoadIdentity();																	//�s��̏�����(�ϊ��s��ɒP�ʍs���ݒ�)
	gluPerspective(30.0, (double)_WINDOW_WIDTH / (double)_WINDOW_HEIGHT, 0.1, 1000.0);	//�������e�@�̎��̐�
	//gluPerspactive(��p, �A�X�y�N�g��, ���s���O, ���s�����);

	/* ���f���r���[�ϊ��s��̐ݒ� */
	glMatrixMode(GL_MODELVIEW);						//�s�񃂁[�h�̐ݒ�iGL_PROJECTION : �����ϊ��s��̐ݒ�AGL_MODELVIEW�F���f���r���[�ϊ��s��j
	glLoadIdentity();								//�s��̏�����(�ϊ��s��ɒP�ʍs���ݒ�)
}

//----------------------------------------------------
// �`��֐�
//----------------------------------------------------
void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //�o�b�t�@�̏���

	glPushMatrix();

	/* ���_�̐ݒ� */
	gluLookAt(
		look_x, look_y, 200.0,	//���_�̈ʒux,y,z;
		p[0].x, p[0].y, p[0].z,	//���E�̒��S�ʒu�̎Q�Ɠ_���Wx,y,z
		0.0, 0.0, 1.0);		//���E�̏�����̃x�N�g��x,y,z

	/* ��] */
	glMultMatrixd(rt);		//�C�ӂ̍s���ώZ����֐�

	/* �}�` */
	DrawSphereOfPlayer();		//����
	DrawSphereOfTarget();		//����

	DrawCylinder();

	/* �e */
	DrawShadow();

	glPopMatrix();

	glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)�Ń_�u���o�b�t�@�����O�𗘗p��
}

//----------------------------------------------------
// ���̏����ݒ�
//----------------------------------------------------
void InitialSphere(void) {
	/* Z of All Sphere */
	for (GLint i = 0; i < _SPHERE_NUMBER; i++) {
		p[i].z = _SPHERE_RADIUS;
	}

	/* Player Sphere */
	p[0].x = 0.0; p[0].y = -_FLOOR_S;

	/* Target Sphere */
	p[1].x = 0.0;						p[1].y = 10.0;
	p[2].x = -_SPHERE_RADIUS     - 1.0;	p[2].y = 10.0 + 2 * _SPHERE_RADIUS;
	p[3].x = _SPHERE_RADIUS		 + 1.0;	p[3].y = 10.0 + 2 * _SPHERE_RADIUS;
	p[4].x = -_SPHERE_RADIUS * 2 - 1.0;	p[4].y = 10.0 + 4 * _SPHERE_RADIUS;
	p[5].x = 0.0;						p[5].y = 10.0 + 4 * _SPHERE_RADIUS;
	p[6].x = _SPHERE_RADIUS * 2  + 1.0;	p[6].y = 10.0 + 4 * _SPHERE_RADIUS;
	p[7].x = -_SPHERE_RADIUS     - 1.0;	p[7].y = 10.0 + 6 * _SPHERE_RADIUS;
	p[8].x = _SPHERE_RADIUS      + 1.0;	p[8].y = 10.0 + 6 * _SPHERE_RADIUS;
	p[9].x = 0.0;						p[9].y = 10.0 + 8 * _SPHERE_RADIUS;
};

//----------------------------------------------------
// Player���̂̕`��
//----------------------------------------------------
void DrawSphereOfPlayer(void) {
	GLint n = 0; //Player ball is p[0]
	if (p[n].z > -_WALL_S / 2) {				//���̂����̒��ɏ����Ă��Ȃ�
		if (CollisionFloor(n)) {				//������
			CollisionWall(n);					//�ǂ̒��˕Ԃ�

			if (CollisionSphere(n)) {					//�ǂꂩ�̋��ƏՓ˂��Ă���ꍇ
				for (GLint i = 0; i < opponent_num.size(); i++) {
					GLint opponent = opponent_num[i];
					NotCollisionPosition(n, opponent);													//�R���W��������
					GLdouble angle_n_op = atan2((p[opponent].y - p[n].y), (p[opponent].x - p[n].x));	//�p�x(n-op)
					GLdouble angle_op_n = atan2((p[n].y - p[opponent].y), (p[n].x - p[opponent].x));	//�p�x(op-n)
					CollisionSphereProcess(n, opponent, angle_n_op);									//�Փˌ�̃x�N�g��(n-op)
					CollisionSphereProcess(opponent, n, angle_op_n);									//�Փˌ�̃x�N�g��(op-n)
				}
				for (GLint i = 0; i < opponent_num.size(); i++) {
					GLint opponent = opponent_num[i];

					p[n].vx = col_p[n].x + col_p[opponent].op_x;										//����x�x�N�g��
					p[n].vy = col_p[n].y + col_p[opponent].op_y;										//����y�x�N�g��
					p[opponent].vx = col_p[n].op_x + col_p[opponent].x;									//���苅x�x�N�g��
					p[opponent].vy = col_p[n].op_y + col_p[opponent].y;									//���苅y�x�N�g��
				}
			}
		}
		else {									//����������
			CollisionHole(n);					//�������R���W����
		}
		SphereAddForce(n);						//���̈ړ����Z

		/* �`�� */
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT,   _MS_WHITE_PLASTIC.ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE,   _MS_WHITE_PLASTIC.diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR,  _MS_WHITE_PLASTIC.specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, &_MS_WHITE_PLASTIC.shininess);
		glTranslated(p[n].x, p[n].y, p[n].z);	    //���s�ړ��l�̐ݒ�
		glutSolidSphere(_SPHERE_RADIUS, 20, 20);	//�����F(���a, Z���܂��̕�����, Z���ɉ�����������)
		glPopMatrix();
	}
}
//----------------------------------------------------
// Target���̂̕`��
//----------------------------------------------------
void DrawSphereOfTarget(void) {
	for (GLint n = 1; n < _SPHERE_NUMBER; n++) {
		if (p[n].z > -_WALL_S / 2) {				//���̂����̒��ɏ����Ă��Ȃ�
			if (CollisionFloor(n)) {				//������
				CollisionWall(n);					//�ǂ̒��˕Ԃ�

				if (CollisionSphere(n)) {					//�ǂꂩ�̋��ƏՓ˂��Ă���ꍇ
					for (GLint i = 0; i < opponent_num.size(); i++) {
						GLint opponent = opponent_num[i];
						NotCollisionPosition(n, opponent);													//�R���W��������
						GLdouble angle_n_op = atan2((p[opponent].y - p[n].y), (p[opponent].x - p[n].x));	//�p�x(n-op)
						GLdouble angle_op_n = atan2((p[n].y - p[opponent].y), (p[n].x - p[opponent].x));	//�p�x(op-n)
						CollisionSphereProcess(n, opponent, angle_n_op);									//�Փˌ�̃x�N�g��(n-op)
						CollisionSphereProcess(opponent, n, angle_op_n);									//�Փˌ�̃x�N�g��(op-n)
					}
					for (GLint i = 0; i < opponent_num.size(); i++) {
						GLint opponent = opponent_num[i];

						p[n].vx = col_p[n].x + col_p[opponent].op_x;										//����x�x�N�g��
						p[n].vy = col_p[n].y + col_p[opponent].op_y;										//����y�x�N�g��
						p[opponent].vx = col_p[n].op_x + col_p[opponent].x;									//���苅x�x�N�g��
						p[opponent].vy = col_p[n].op_y + col_p[opponent].y;									//���苅y�x�N�g��
					}
				}
			}
			else {									//����������
				CollisionHole(n);					//�������R���W����
			}
			SphereAddForce(n);						//���̈ړ����Z

			glPushMatrix();
			glMaterialfv(GL_FRONT, GL_AMBIENT, _MS_RUBY.ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, _MS_RUBY.diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, _MS_RUBY.specular);
			glMaterialfv(GL_FRONT, GL_SHININESS, &_MS_RUBY.shininess);
			glTranslated(p[n].x, p[n].y, p[n].z);	//���s�ړ��l�̐ݒ�
			glutSolidSphere(_SPHERE_RADIUS, 20, 20);	//�����F(���a, Z���܂��̕�����, Z���ɉ�����������)
			glPopMatrix();
		} else {
			p[n].vx = p[n].vy = 0.0;
		}
	}
}

//----------------------------------------------------
// �e�[�u���̕`��
//----------------------------------------------------
void DrawTable(void) {
	glDisable(GL_LIGHTING);
	DrawWall();
	DrawHole();
	DrawFloor();
	glEnable(GL_LIGHTING);
}
//----------------------------------------------------
// ��(������)�̕`��
//----------------------------------------------------
void DrawWall(void) {
	glPushMatrix();
	glBegin(GL_QUADS);							//GL_QUADS�F4�_�g�̎l�p�`
	glColor4fv(_RED);
	for (GLint j = 0; j < 6; ++j) {
		for (GLint i = 0; i < 4; ++i) {
			glVertex3dv(_WALL_TOP_VERTEX[_QUADS_FACE[j][i]]);
		}
	}
	glEnd();
	glBegin(GL_QUADS);							//GL_QUADS�F4�_�g�̎l�p�`
	for (GLint j = 0; j < 6; ++j) {
		for (GLint i = 0; i < 4; ++i) {
			glVertex3dv(_WALL_BOTTOM_VERTEX[_QUADS_FACE[j][i]]);
		}
	}
	glEnd();
	glBegin(GL_QUADS);							//GL_QUADS�F4�_�g�̎l�p�`
	for (GLint j = 0; j < 6; ++j) {
		for (GLint i = 0; i < 4; ++i) {
			glVertex3dv(_WALL_LEFT_VERTEX[_QUADS_FACE[j][i]]);
		}
	}
	glEnd();
	glBegin(GL_QUADS);							//GL_QUADS�F4�_�g�̎l�p�`
	for (GLint j = 0; j < 6; ++j) {
		for (GLint i = 0; i < 4; ++i) {
			glVertex3dv(_WALL_RIGHT_VERTEX[_QUADS_FACE[j][i]]);
		}
	}
	glEnd();
	glPopMatrix();
}
//----------------------------------------------------
// ��(����)�̕`��
//----------------------------------------------------
void DrawHole(void) {
	glPushMatrix();
	glColor4fv(_BLACK);
	glBegin(GL_QUADS);
	for (GLint j = 0; j < 6; ++j) {
		for (GLint i = 0; i < 4; ++i) {
			glVertex3dv(_HOLE_VERTEX[_QUADS_FACE[j][i]]);
		}
	}
	glEnd();
	glPopMatrix();
}
//----------------------------------------------------
// ���̕`��
//----------------------------------------------------
void DrawFloor(void) {
	glPushMatrix();
	glColor4fv(_GREEN);
	glBegin(GL_QUADS);
	for (GLint j = 0; j < 3; ++j) {
		for (GLint i = 0; i < 4; ++i) {
			glVertex3dv(_FLOOR_VERTEX[_FLOOR_FACE[j][i]]);
		}
	}
	glEnd();
	glPopMatrix();
}

//----------------------------------------------------
// �L���[�̕`��
//----------------------------------------------------
GLdouble cylinder_pull = 0.0;
void DrawCylinder(void)
{
	glPushMatrix();
	glTranslated(p[0].x, p[0].y, 18.0);
	glRotated(look_angle*180/M_PI+90, 0.0, 0.0, 1.0);
	if (space_key_pressing) {
		cylinder_pull += 0.1;
	} else {
		cylinder_pull = 0.0;
	}
	glTranslated(0, 52 + cylinder_pull, 0.0);
	glRotated(15, 1.0, 0.0, .0);
	/*��� */
	if (enable_play) {
		glCullFace(GL_FRONT);
		glNormal3d(0.0, 1.0, 0.0);
		glBegin(GL_POLYGON);
		for (double i = 0; i < _CYLINDER_SIDES; i++) {
			double t = M_PI * 2 / _CYLINDER_SIDES * (double)i;
			glVertex3d(_CYLINDER_RADIUS * cos(t), _CYLINDER_HEIGHT, _CYLINDER_RADIUS * sin(t));
		}
		glEnd();
		/* ���� */
		glCullFace(GL_BACK);
		glBegin(GL_QUAD_STRIP);
		for (double i = 0; i <= _CYLINDER_SIDES; i = i + 1) {
			double t = i * 2 * M_PI / _CYLINDER_SIDES;
			glNormal3f((GLfloat)cos(t), 0.0, (GLfloat)sin(t));
			glVertex3f((GLfloat)(_CYLINDER_RADIUS*cos(t)), -_CYLINDER_HEIGHT, (GLfloat)(_CYLINDER_RADIUS*sin(t)));
			glVertex3f((GLfloat)(_CYLINDER_RADIUS*cos(t)), _CYLINDER_HEIGHT, (GLfloat)(_CYLINDER_RADIUS*sin(t)));
		}
		glEnd();
		/* ���� */
		glCullFace(GL_FRONT);
		glNormal3d(0.0, -1.0, 0.0);
		glBegin(GL_POLYGON);
		for (double i = _CYLINDER_SIDES; i >= 0; --i) {
			double t = M_PI * 2 / _CYLINDER_SIDES * (double)i;
			glVertex3d(_CYLINDER_RADIUS * cos(t), -_CYLINDER_HEIGHT, _CYLINDER_RADIUS * sin(t));
		}
		glEnd();
		glCullFace(GL_BACK);
	}
	glPopMatrix();
}

//----------------------------------------------------
// ���̂̈ړ����Z
//----------------------------------------------------
void SphereAddForce(GLint n) {
	p[n].vx = p[n].vx * _FRICTION;
	p[n].vy = p[n].vy * _FRICTION;
	p[n].x += p[n].vx * _POS_ADJUSTMENT;
	p[n].y += p[n].vy * _POS_ADJUSTMENT;
	p[n].z += p[n].vz * _POS_ADJUSTMENT;
	if (abs(p[n].vy) < 0.01) {	//�����ɂ���~
		p[n].vy = 0.0;
	}
	if (abs(p[n].vx) < 0.01) {	//�����ɂ���~
		p[n].vx = 0.0;
	}
}

//----------------------------------------------------
// ���̓��m�̓����蔻��
//----------------------------------------------------
bool CollisionSphere(GLint n) {
	bool collision_bool = false;
	std::vector<GLint> opponent_num_reset;
	opponent_num = opponent_num_reset;
	col_p[n].x = 0;
	col_p[n].y = 0;
	col_p[n].op_x = 0;
	col_p[n].op_y = 0;
	for (GLint i = n+1; i < _SPHERE_NUMBER; i++) {
		if ((p[n].x-p[i].x)*(p[n].x - p[i].x) + 
			(p[n].y-p[i].y)*(p[n].y - p[i].y) + 
				(p[n].z-p[i].z)*(p[n].z - p[i].z)
					<= (2*_SPHERE_RADIUS) * (2*_SPHERE_RADIUS)) {	//��̋��̋��������a��菬������
			col_p[i].x = 0;
			col_p[i].y = 0;
			col_p[i].op_x = 0;
			col_p[i].op_y = 0;
			opponent_num.push_back(i);
			collision_bool = true;
		}
	}

	return collision_bool;
}

//----------------------------------------------------
// ���̓��m�̓����莞�̉��Z
//----------------------------------------------------
void CollisionSphereProcess(GLint n, GLint op_n, GLdouble angle) {			//n:����, op_n:���苅, angle:���Ԃ̊p�x
	GLdouble diagonal = sqrt(p[n].vx*p[n].vx + p[n].vy*p[n].vy);	//�Ίp���̒���
	GLdouble direction_travel = atan2(p[n].vy, p[n].vx);			//�i�s����
	GLdouble difference_angle = direction_travel - angle;			//�p�x�̍�
	GLdouble after_opv = abs(diagonal * cos(difference_angle));		//�Փˌ㑊��x�N�g��
	GLdouble after_nv  = abs(diagonal * sin(difference_angle));		//�Փˌ㎩���x�N�g��
	GLdouble after_opvx = after_opv * cos(angle);					//�Փˌ㑊��x�N�g��x
	GLdouble after_opvy = after_opv * sin(angle);					//�Փˌ㑊��x�N�g��y

	GLdouble after_nvx, after_nvy;									//�Փˌ㎩�x�N�g��x,�Փˌ㎩�x�N�g��y

	if (sin(difference_angle) < 0) {
		after_nvx = after_nv * cos(angle - M_PI / 2);
		after_nvy = after_nv * sin(angle - M_PI / 2);
	}else{
		after_nvx = after_nv * cos(angle + M_PI / 2);
		after_nvy = after_nv * sin(angle + M_PI / 2);
	}	

	col_p[n].x    = after_nvx;
	col_p[n].y    = after_nvy;
	col_p[n].op_x = after_opvx;
	col_p[n].op_y = after_opvy;
}

//----------------------------------------------------
// ���̓��m�̓����莞�̃R���W��������
//----------------------------------------------------
void NotCollisionPosition(GLint n, GLint op_n) {
	GLdouble new_pos = atan2((p[n].y - p[op_n].y), (p[n].x - p[op_n].x));
	p[n].x = _COL_PUSH*cos(new_pos) + p[n].x;
	p[n].y = _COL_PUSH*sin(new_pos) + p[n].y;
}

//----------------------------------------------------
// ���̂ƕǂ̓����蔻�肨��щ��Z
//----------------------------------------------------
void CollisionWall(GLint n) {
	if (abs(p[n].y)+1.0 > _FLOOR_S * 2 - _SPHERE_RADIUS) {	//�㉺��
		p[n].vy = -p[n].vy;
	}
	if (abs(p[n].x)+1.0 > _FLOOR_S - _SPHERE_RADIUS) {		//���E��
		p[n].vx = -p[n].vx;
	}
}

//----------------------------------------------------
// ���̂ƌ������̓����蔻�肨��ьv�Z
//----------------------------------------------------
void CollisionHole(GLint n) {
	//h_flag�F�������ɓ���A��x�ǂɔ��˂��Ă��猊�����ǂ̔��˂�L���ɂ���
	if (abs(p[n].y) > _FLOOR_S * 2 - _SPHERE_RADIUS) {										//�e�[�u���㉺��
		double a = -(1.0) * p[n].vy;
		p[n].vy = 2 * a*(1.0) + p[n].vy;
		p[n].h_flag = true;
	}
	if ((abs(p[n].y) < _FLOOR_S * 2 + _SPHERE_RADIUS - _FLOOR_OFFSET) && p[n].h_flag) {		//�������㉺��
		double a = -(1.0) * p[n].vy;
		p[n].vy = 2 * a*(1.0) + p[n].vy;
	}
	if (abs(p[n].x) > _FLOOR_S - _SPHERE_RADIUS) {											//�e�[�u�����E��
		double a = -(1.0) * p[n].vx;
		p[n].vx = 2 * a*(1.0) + p[n].vx;
		p[n].h_flag = true;
	}
	if ((abs(p[n].x) < _FLOOR_S + _SPHERE_RADIUS - _FLOOR_OFFSET) && p[n].h_flag) {			//���������E��
		double a = -(1.0) * p[n].vx;
		p[n].vx = 2 * a*(1.0) + p[n].vx;
	}
}

//----------------------------------------------------
// ���̂Ə��̓����蔻��
//----------------------------------------------------
bool CollisionFloor(GLint n) {
	p[n].vz = _GRAVITY;
	if (p[n].z < _SPHERE_RADIUS+1 && p[n].z > 0) {				//���̍�������(�㉺�]�T����)
		if (2 * _FLOOR_S - _SPHERE_RADIUS  > p[n].y &&
			-2 * _FLOOR_S + _SPHERE_RADIUS < p[n].y &&
			_FLOOR_S + _SPHERE_RADIUS/2 - _FLOOR_OFFSET    > p[n].x &&
			-_FLOOR_S - _SPHERE_RADIUS/2 + _FLOOR_OFFSET	  < p[n].x) {			//����1(�����̒����`�Ɍ����Ă�)
			p[n].h_flag = false;												//�������ł͂Ȃ�
			p[n].z = _SPHERE_RADIUS;											//��������K�؂Ȉʒu�ɒ���
			p[n].vz = _GRAVITY - _GRAVITY;										//���܂Ȃ��悤�ɏd�͂̑ł�����
			return true;
		}
		else if (2 * _FLOOR_S + _SPHERE_RADIUS/2 - _FLOOR_OFFSET  > p[n].y &&
			-2 * _FLOOR_S - _SPHERE_RADIUS/2 + _FLOOR_OFFSET < p[n].y &&
			_FLOOR_S - _SPHERE_RADIUS  > p[n].x &&
			-_FLOOR_S + _SPHERE_RADIUS < p[n].x) {								//����2(�����̒����`�Ɍ����Ă�)
			p[n].h_flag = false;
			p[n].z = _SPHERE_RADIUS;
			p[n].vz = _GRAVITY - _GRAVITY;
			return true;
		}
	}
	return false;	//����łȂ� = ��
}


//----------------------------------------------------
// �����ʂ̕�����
//----------------------------------------------------
void FindPlane(
	GLfloat plane[4],	// �쐬���镽�ʕ������̌W��
	GLfloat v0[3],		// ���_�P
	GLfloat v1[3],		// ���_�Q
	GLfloat v2[3])		// ���_�R
{
	GLfloat vec0[3], vec1[3];

	// Need 2 vectors to find cross product.
	vec0[0] = v1[0] - v0[0];			//x�Fvector0 = ���_2 - ���_1
	vec0[1] = v1[1] - v0[1];			//y�Fvector0 = ���_2 - ���_1
	vec0[2] = v1[2] - v0[2];			//z�Fvector0 = ���_2 - ���_1

	vec1[0] = v2[0] - v0[0];			//x�Fvector1 = ���_3 - ���_1
	vec1[1] = v2[1] - v0[1];			//y�Fvector1 = ���_3 - ���_1
	vec1[2] = v2[2] - v0[2];			//z�Fvector1 = ���_3 - ���_1

	// find cross product to get A, B, and C of plane equation
	plane[0] =	 vec0[1] * vec1[2] - vec0[2] * vec1[1];		//  vector0.y * vector1.z - vector0.z * vector1.y
	plane[1] = -(vec0[0] * vec1[2] - vec0[2] * vec1[0]);	//-(vector0.x * vector1.z - vector0.z * vector1.x)
	plane[2] =   vec0[0] * vec1[1] - vec0[1] * vec1[0];		//  vector0.x * vector1.y - vector0.y * vector1.x

	plane[3] = -(plane[0] * v0[0] + plane[1] * v0[1] + plane[2] * v0[2]);	//�x�N�g�����Z
}

//----------------------------------------------------
// �s��̌v�Z
//----------------------------------------------------
void ShadowMatrix(
	GLfloat *m,			// �쐬����s��̃|�C���^
	GLfloat plane[4],	// �ˉe����\�ʂ̕��ʕ������̌W��
	GLfloat light[4])	// �����̓������W�l
{
	GLfloat dot;

	// Find dot product between light position vector and ground plane normal.
	dot = plane[0] * light[0] +
		plane[1] * light[1] +
		plane[2] * light[2] +
		plane[3] * light[3];

	m[0] = dot - light[0] * plane[0];
	m[4] = 0.f - light[0] * plane[1];
	m[8] = 0.f - light[0] * plane[2];
	m[12] = 0.f - light[0] * plane[3];

	m[1] = 0.f - light[1] * plane[0];
	m[5] = dot - light[1] * plane[1];
	m[9] = 0.f - light[1] * plane[2];
	m[13] = 0.f - light[1] * plane[3];

	m[2] = 0.f - light[2] * plane[0];
	m[6] = 0.f - light[2] * plane[1];
	m[10] = dot - light[2] * plane[2];
	m[14] = 0.f - light[2] * plane[3];

	m[3] = 0.f - light[3] * plane[0];
	m[7] = 0.f - light[3] * plane[1];
	m[11] = 0.f - light[3] * plane[2];
	m[15] = dot - light[3] * plane[3];
}

//----------------------------------------------------
// �e�̕`��
//----------------------------------------------------
void DrawShadow(void) {
	/* ���̃X�e���V����t���� */
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, ~0);

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);	//���ꂩ��`�悷����̂̃X�e���V���l�ɂ��ׂĂP�^�O������
	//(GL_REPLACE�FflStencilFunc()�������ɒu������)
	DrawTable();							//���̕`��

	/* �J���[�E�f�v�X�o�b�t�@�}�X�N���Z�b�g����
	 * ����ňȉ��̓��e�̃s�N�Z���̐F�̒l�́A�������܂�Ȃ��B*/
	glColorMask(0, 0, 0, 0);
	glDepthMask(0);

	/* ���ɃI�u�W�F�N�g�̉e�̃X�e���V����t���� */
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, 1, ~0);

	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);		//���ꂩ��`�悷����̂̃X�e���V���l�ɂ��ׂĂP�^�O������
	//(GL_INCR�F�X�e���V���l��+1 < max)
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	glMultMatrixf(pM);							//���݂̍s���pM(shadowMatrix)���|����
	DrawSphereOfPlayer();
	DrawSphereOfTarget();
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);

	/* �r�b�g�}�X�N������ */
	glColorMask(1, 1, 1, 1);
	glDepthMask(1);

	/* �e������ */
	glStencilFunc(GL_EQUAL, 2, ~0);
	glEnable(GL_BLEND);									//��������
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//�A���t�@�u�����h
	glColor4f(0.1f, 0.1f, 0.1f, 0.5f);
	glDisable(GL_DEPTH_TEST);
	DrawTable();									//���̕`��
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_STENCIL_TEST);
}

//----------------------------------------------------
// �L�[�{�[�h���͎��ɌĂяo�����֐�
//----------------------------------------------------
void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '\040':	//SP
		space_key_pressing = true;
		break;
	case '\033':	//ESC��ASCII�R�[�h
		exit(0);
		break;
	default:
		break;
	}
}

//----------------------------------------------------
// �L�[�{�[�h�������ꂽ���ɌĂяo�����֐�
//----------------------------------------------------
void KeyboardUp(unsigned char key, int x, int y) {
	switch (key) {
	case '\040':	//SP
		space_key_pressing = false;
		GLdouble force_x = (p[0].x - look_x) * _ADD_FORCE_DT;
		GLdouble force_y = (p[0].y - look_y) * _ADD_FORCE_DT;
		p[0].vx += force_x * add_force * _ADD_FORCE_DT;
		p[0].vy += force_y * add_force * _ADD_FORCE_DT;
		add_force = 0.0;
		break;
	}
}

//----------------------------------------------------
// Special�L�[�{�[�h���͎��ɌĂяo�����֐�
//----------------------------------------------------
void SpecialFunc(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:	//��
		left_key_pressing = true;
		break;
	case GLUT_KEY_RIGHT: //��
		right_key_pressing = true;
		break;	
	}
}

//----------------------------------------------------
// Special�L�[�{�[�h�������ꂽ���ɌĂяo�����֐�
//----------------------------------------------------
void SpecialUpFunc(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:	//��
		left_key_pressing = false;
		break;
	case GLUT_KEY_RIGHT: //��
		right_key_pressing = false;
		break;
	}
}

//----------------------------------------------------
// �}�E�X�h���b�O��
//----------------------------------------------------
void MouseMotion(int x, int y) {
	double dx, dy, a;

	/* �}�E�X�|�C���^�̈ʒu�̃h���b�O�J�n�ʒu����̕ψ� */
	dx = (x - cx) * sx;
	dy = (y - cy) * sy;

	/* �}�E�X�|�C���^�̈ʒu�̃h���b�O�J�n�ʒu����̋��� */
	a = sqrt(dx * dx + dy * dy);

	if (a != 0.0)
	{
		/* �}�E�X�̃h���b�O�ɔ�����]�̃N�H�[�^�j�I�� dq �����߂� */
		double ar = a * SCALE * 0.5;
		double as = sin(ar) / a;
		double dq[4] = { cos(ar), dy * as, dx * as, 0.0 };

		/* ��]�̏����l cq �� dq ���|���ĉ�]������ */
		Qmul(tq, dq, cq);

		/* �N�H�[�^�j�I�������]�̕ϊ��s������߂� */
		Qrot(rt, tq);
	}
}
//----------------------------------------------------
// �}�E�X�N���b�N��
//----------------------------------------------------
void MouseOn(int button, int state, int x, int y)
{
	switch (button) {
	case 0:
		switch (state) {
		case 0:
			// �h���b�O�J�n�_���L�^
			cx = x;
			cy = y;
			break;
		case 1:
			// ��]�̕ۑ�
			cq[0] = tq[0];
			cq[1] = tq[1];
			cq[2] = tq[2];
			cq[3] = tq[3];
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

//----------------------------------------------------
// �}�E�X�h���b�O�ɂ���]
//----------------------------------------------------
// �N�H�[�^�j�I���̐� r <- p x q
static void Qmul(double r[], const double p[], const double q[])
{
	r[0] = p[0] * q[0] - p[1] * q[1] - p[2] * q[2] - p[3] * q[3];
	r[1] = p[0] * q[1] + p[1] * q[0] + p[2] * q[3] - p[3] * q[2];
	r[2] = p[0] * q[2] - p[1] * q[3] + p[2] * q[0] + p[3] * q[1];
	r[3] = p[0] * q[3] + p[1] * q[2] - p[2] * q[1] + p[3] * q[0];
}

// ��]�̕ϊ��s�� r <- �N�H�[�^�j�I�� q
static void Qrot(double r[], double q[]) {
	double x2 = q[1] * q[1] * 2.0;
	double y2 = q[2] * q[2] * 2.0;
	double z2 = q[3] * q[3] * 2.0;
	double xy = q[1] * q[2] * 2.0;
	double yz = q[2] * q[3] * 2.0;
	double zx = q[3] * q[1] * 2.0;
	double xw = q[1] * q[0] * 2.0;
	double yw = q[2] * q[0] * 2.0;
	double zw = q[3] * q[0] * 2.0;

	r[0] = 1.0 - y2 - z2;
	r[1] = xy + zw;
	r[2] = zx - yw;
	r[4] = xy - zw;
	r[5] = 1.0 - z2 - x2;
	r[6] = yz + xw;
	r[8] = zx + yw;
	r[9] = yz - xw;
	r[10] = 1.0 - x2 - y2;
	r[3] = r[7] = r[11] = r[12] = r[13] = r[14] = 0.0;
	r[15] = 1.0;
}

void MouseWheel(float z) {
	camera_z_pos += z;

	if (camera_z_pos < 0.0f)
	{
		camera_z_pos = 0.0f;
	}
}