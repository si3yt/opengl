#include <math.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <direct.h>
#include <time.h>
#include <GL/glut.h>
using namespace std;

//----------------------------------------------------
// �ϐ��錾
//----------------------------------------------------
/* �E�B���h�E�ϐ� */
int  WINDOW_POSITION_X = 100; 
int  WINDOW_POSITION_Y = 100;
int  WINDOW_WIDTH = 512;
int  WINDOW_HEIGHT = 512; 
char WINDOW_TITLE[] = "billiards";

/* �}�E�X��] */
#define SCALE (2.0 * 3.14159265358979323846)	// �}�E�X�̑��Έʒu����]�p�̊��Z�W��
int cx, cy;										// �h���b�O�J�n�ʒu
double sx, sy;									// �}�E�X�̐�Έʒu���E�B���h�E���ł̑��Έʒu�̊��Z�W��
double cq[4] = { 1.0, 0.0, 0.0, 0.0 };			// ��]�̏����l (�N�H�[�^�j�I��)
double tq[4];									// �h���b�O���̉�] (�N�H�[�^�j�I��)
double rt[16];									// ��]�̕ϊ��s��
float camera_z_pos = 50.0;

/* �f�B�X�v���C���X�g */
unsigned int listNumber;

/* ��������(�R�{���u���̍\���̂𗘗p) */
struct MaterialStruct {							// material�p�\����
	GLfloat ambient[4];		// ������
	GLfloat diffuse[4];		// �g�U����
	GLfloat specular[4];	// ���ʔ���
	GLfloat shininess;		// ����
};
MaterialStruct ms_jade = {						// jade(�Ő�)
	{ 0.135,     0.2225,   0.1575,   1.0 },
	{ 0.54,      0.89,     0.63,     1.0 },
	{ 0.316228,  0.316228, 0.316228, 1.0 },
	12.8 };
MaterialStruct ms_ruby = {						// ruby(���r�[)
	{ 0.1745,   0.01175,  0.01175,   1.0 },
	{ 0.61424,  0.04136,  0.04136,   1.0 },
	{ 0.727811, 0.626959, 0.626959,  1.0 },
	76.8 };

/* �F */
GLfloat red[]     = { 0.8, 0.2, 0.2, 1.0 };	//�ԐF
GLfloat green[]   = { 0.2, 0.8, 0.2, 1.0 };	//�ΐF
GLfloat blue[]    = { 0.2, 0.2, 0.8, 1.0 };	//�F
GLfloat yellow[]  = { 0.8, 0.8, 0.2, 1.0 };	//���F
GLfloat white[]   = { 1.0, 1.0, 1.0, 1.0 };	//���F
GLfloat black[]   = { 0.0, 0.0, 0.0, 1.0 };	//���F
GLfloat shininess = 30.0;					//����̋���

/* �� */
GLfloat LIGHT_POSITION_0[] = { 0.0, 0.0, 150.0, 1.0 }; //����0�̍��W

static GLfloat floor_planar[4];			//��
static GLfloat floor_s = 50.0f;			//���W��
static GLfloat wall_s = 10.0f;			//�ǂ̍���
static GLfloat pM[16];					//shadowMatrix�Fm[]

static GLfloat floor_offset = 10;

/* �� */
typedef struct _QUADS_VERTEX {
	GLfloat v0[3];
	GLfloat v1[3];
	GLfloat v2[3];
	GLfloat v3[3];
}QUADS_VERTEX;
static QUADS_VERTEX floor_v = {
	{ floor_s,  floor_s, 0.0f },
	{ -floor_s,  floor_s, 0.0f },
	{ -floor_s, -floor_s, 0.0f },
	{ floor_s, -floor_s, 0.0f },
};

GLdouble floor_vertex[][3] = {
	{ floor_s  -floor_offset, floor_s*2,				0.0f },
	{ -floor_s +floor_offset, floor_s*2,				0.0f },
	{ -floor_s +floor_offset, floor_s*2  -floor_offset, 0.0f },
	{ -floor_s,				  floor_s*2  -floor_offset, 0.0f },
	{ -floor_s,				  -floor_s*2 +floor_offset, 0.0f },
	{ -floor_s +floor_offset, -floor_s*2 +floor_offset, 0.0f },
	{ -floor_s +floor_offset, -floor_s*2,				0.0f },
	{ floor_s  -floor_offset, -floor_s*2,				0.0f },
	{ floor_s  -floor_offset, -floor_s*2 +floor_offset, 0.0f },
	{ floor_s,				  -floor_s*2 +floor_offset, 0.0f },
	{ floor_s,				  floor_s*2  -floor_offset, 0.0f },
	{ floor_s  -floor_offset, floor_s*2  -floor_offset, 0.0f },
};

int floor_face[][4] = {
	{0,  1,  6,  7},
	{2,  3,  4,  5},
	{10, 11, 8,  9}
};

/* �� */
GLdouble hole_vertex[][3] = {
	{ -floor_s, -floor_s*2, -wall_s   },
	{ floor_s,  -floor_s*2, -wall_s   },
	{ floor_s,  floor_s*2,  -wall_s   },
	{ -floor_s, floor_s*2,  -wall_s   },
	{ -floor_s, -floor_s*2, -wall_s/2 },
	{ floor_s,  -floor_s*2, -wall_s/2 },
	{ floor_s,  floor_s*2,  -wall_s/2 },
	{ -floor_s, floor_s*2,  -wall_s/2 },
};

/* �� */
GLdouble vertex_top[][3] = {	// ���_
	{ -floor_s,  floor_s*2,		    -wall_s },
	{ floor_s,   floor_s*2,		    -wall_s },
	{ floor_s,   floor_s*2 +wall_s, -wall_s },
	{ -floor_s,  floor_s*2 +wall_s, -wall_s },
	{ -floor_s,  floor_s*2,		    wall_s  },
	{ floor_s,   floor_s*2,		    wall_s  },
	{ floor_s,   floor_s*2 +wall_s, wall_s  },
	{ -floor_s,  floor_s*2 +wall_s, wall_s  },
};
GLdouble vertex_bottom[][3] = {	// ���_
	{ -floor_s,  -floor_s*2 -wall_s, -wall_s },
	{ floor_s,   -floor_s*2 -wall_s, -wall_s },
	{ floor_s,   -floor_s*2,		 -wall_s },
	{ -floor_s,  -floor_s*2,		 -wall_s },
	{ -floor_s,  -floor_s*2 -wall_s, wall_s  },
	{ floor_s,   -floor_s*2 -wall_s, wall_s  },
	{ floor_s,   -floor_s*2,		 wall_s  },
	{ -floor_s,  -floor_s*2,		 wall_s  },
};
GLdouble vertex_left[][3] = {	// ���_
	{ -floor_s -wall_s,	-floor_s*2 -wall_s,	-wall_s },
	{ -floor_s,			-floor_s*2 -wall_s,	-wall_s },
	{ -floor_s,         floor_s*2  +wall_s, -wall_s },
	{ -floor_s -wall_s, floor_s*2  +wall_s, -wall_s },
	{ -floor_s -wall_s,	-floor_s*2 -wall_s,	wall_s  },
	{ -floor_s,			-floor_s*2 -wall_s,	wall_s  },
	{ -floor_s,		    floor_s*2  +wall_s, wall_s  },
	{ -floor_s -wall_s, floor_s*2  +wall_s, wall_s  },
};
GLdouble vertex_right[][3] = {	// ���_
	{ floor_s,		   -floor_s*2 -wall_s, -wall_s },
	{ floor_s +wall_s, -floor_s*2 -wall_s, -wall_s },
	{ floor_s +wall_s, floor_s*2  +wall_s, -wall_s },
	{ floor_s,		   floor_s*2  +wall_s, -wall_s },
	{ floor_s,		   -floor_s*2 -wall_s, wall_s  },
	{ floor_s +wall_s, -floor_s*2  -wall_s, wall_s  },
	{ floor_s +wall_s, floor_s*2  +wall_s, wall_s  },
	{ floor_s,		   floor_s*2  +wall_s, wall_s  },
};
int face[][4] = {			// ��(���_�̏���)
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
}FACE_NORMAL;
static FACE_NORMAL face_normal = {		//�ʂ̖@���x�N�g��
	{ 0.0,  0.0,  -1.0 },
	{ 1.0,  0.0,  0.0  },
	{ 0.0,  0.0,  1.0  },
	{ -1.0, 0.0,  0.0  },
	{ 0.0,  -1.0, 0.0  },
	{ 0.0,  1.0,  0.0  }
};



struct {								//�\���́Fp �`�敨�̍\����
	double x, y, z;
	double vx, vy, vz;
}p[100];
int pn = 0;								//pnumber�F�V�����`�悵�����̔ԍ�
double ax = 0.0, ay = 0.0, az = -4.0;	//addx,addy,addz�Fadd force
double vx = 5.0, vy = 5.0, vz = 20.0;	//vectorx,vectory,vectorz�F���̈ړ��̕����x�N�g��
double hanpatu = 0.9;					//�������W��

double dt = 0.05;						//���̉��Z�ϐ�

double sphere_radius = 4.0;				//���̔��a
double sphere_number = 100;				//���̌�

bool key_space = false;
double add_force = 0.0;

//----------------------------------------------------
// �֐��v���g�^�C�v�i��ɌĂяo���֐����ƈ����̐錾�j
//----------------------------------------------------
void Initialize(void);
void Idle(void);
void Display(void);
void Resize(int w, int h);

int Collision(int n);
void WallCollistion(int n);

void Sphere(void);
void Wall(void);
void Hole(void);

void findPlane(GLfloat plane[4], GLfloat v0[3], GLfloat v1[3], GLfloat v2[3]);
void shadowMatrix(GLfloat *m, GLfloat plane[4], GLfloat light[4]);
void DrawTable(bool bTexture);
void DrawShadow(void);
void DrawStructure(bool);

void Qmul(double r[], const double p[], const double q[]);
void Qrot(double r[], double q[]);

void Keyboard(unsigned char key, int x, int y);
void KeyboardUp(unsigned char key, int x, int y);
void MouseMotion(int x, int y);
void MouseOn(int button, int state, int x, int y);
void MouseWheel(float z);

//----------------------------------------------------
// ���C���֐�
//----------------------------------------------------
int main(int argc, char *argv[]) {
	srand((unsigned)time(NULL));									//�����̔���
	glutInit(&argc, argv);											//���̏�����
	glutInitWindowPosition(WINDOW_POSITION_X, WINDOW_POSITION_Y);	//�E�B���h�E�̈ʒu�̎w��
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);				//�E�B���h�E�T�C�Y�̎w��
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);		//�f�B�X�v���C���[�h�̎w��(RGBA,�f�v�X�o�b�t�@,�_�u���o�b�t�@)
	glutCreateWindow(WINDOW_TITLE);									//�E�B���h�E�̍쐬
	glutDisplayFunc(Display);										//�`�掞�ɌĂяo�����֐�
	glutReshapeFunc(Resize);										//���T�C�Y���ɌĂяo�����֐�
	glutKeyboardFunc(Keyboard);										//�L�[�{�[�h���͎��ɌĂяo�����֐�
	glutKeyboardUpFunc(KeyboardUp);
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

	findPlane(floor_planar,
		floor_v.v0,
		floor_v.v1,
		floor_v.v2);

	/* �����̐ݒ� */
	glEnable(GL_LIGHTING);									//�A�eON
	glEnable(GL_LIGHT0);									//����0�𗘗p
	glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_POSITION_0);	//����0�̈ʒu

	/* �f�B�X�v���C���X�g���쐬 */
	listNumber = glGenLists(1);
	glNewList(listNumber, GL_COMPILE);	//�R���p�C���̂�
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
	shadowMatrix(pM, floor_planar, LIGHT_POSITION_0);

}

//----------------------------------------------------
// �A�C�h�����ɌĂяo�����֐�
//----------------------------------------------------
void Idle() {
	glutPostRedisplay(); //glutDisplayFunc()���P����s����

	if (key_space) {
		if (add_force <= 3500) {
			add_force += 15;
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
	gluPerspective(30.0, (double)WINDOW_WIDTH / (double)WINDOW_HEIGHT, 0.1, 1000.0);	//�������e�@�̎��̐�
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
		0.0, -300.0, 200.0,	//���_�̈ʒux,y,z;
		0.0, 0.0, 0.0,	//���E�̒��S�ʒu�̎Q�Ɠ_���Wx,y,z
		0.0, 0.0, 1.0);		//���E�̏�����̃x�N�g��x,y,z

	/* ��] */
	glMultMatrixd(rt);		//�C�ӂ̍s���ώZ����֐�

	/* �}�`�̕`�� */
	Sphere();		//��

	DrawStructure(false);
	DrawShadow();
	glPopMatrix();

	glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)�Ń_�u���o�b�t�@�����O�𗘗p��
}
//----------------------------------------------------
// ���̂̕`��
//----------------------------------------------------
void DrawStructure(bool flag) {
	for (int i = 1; i <= pn; i++) {
		p[i].vx += ax * dt;				//vector = vector + add * dt
		p[i].vy += ay * dt;				//vector = vector + add * dt
		p[i].vz += az * dt;				//vector = vector + add * dt
		p[i].x += p[i].vx * dt;			//position = vector * dt
		p[i].y += p[i].vy * dt;			//position = vector * dt
		p[i].z += p[i].vz * dt;			//position = vector * dt
		if (p[i].z < 4 && abs(p[i].x) < floor_s  && abs(p[i].y) < floor_s) {
		//z < 4(���̔��a?)
		//|x| < floor��x���[
		//|y| < floor��y���[  //����ŏ��𔲂���ꍇ
			p[i].z = 4.0;					//z������ɏ��ʒu�ɍX�V
			p[i].vz = -hanpatu * p[i].vz;	//vector = -�����W�� * vector  //���ɂ�锽���ɂ��x�N�g���X�V
		}
		if (!flag || p[i].z >0) {			//display or z > 0
			glPushMatrix();
			glMaterialfv(GL_FRONT, GL_AMBIENT, ms_ruby.ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, ms_ruby.diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, ms_ruby.specular);
			glMaterialfv(GL_FRONT, GL_SHININESS, &ms_ruby.shininess);
			glTranslated(p[i].x, p[i].y, p[i].z);	//���s�ړ��l�̐ݒ�
			glutSolidSphere(sphere_radius, 20, 20);			//�����F(���a, Z���܂��̕�����, Z���ɉ�����������)
			glPopMatrix();
		}
	}
}

int Collision(int n) {
	for (size_t i = n; i < sphere_number; i++) {
		if (i == n){}
		else {
			if (abs((p[n].x-p[i].x) + (p[n].y-p[i].y) + (p[n].z-p[i].z)) <= sphere_radius*sphere_radius) {
				return i;
			}
		}
	}
	return -1;
}

void WallCollistion(int n) {
	if (abs(p[n].y) > floor_s * 2 - sphere_radius) {
		double a = -(1.0) * p[n].vy;
		p[n].vy = 2 * a*(1.0) + p[n].vy;
	}
	if (abs(p[n].x) > floor_s - sphere_radius) {
		double a = -(1.0) * p[n].vx;
		p[n].vx = 2 * a*(1.0) + p[n].vx;
	}
}
//----------------------------------------------------
// ��
//---------------------------------------------------
double sphere_y = 0.0;
double sphere_vy = 0.0;
double force = 0.0;
void Sphere(void) {
	int n = 1;
	p[n].vy += force * dt;
	p[n].y += p[n].vy * 0.01;
	if (p[n].vy < 0) { force = 2.0; }	//���W�����܂����Ƃ��̕����ϊ�
	else if(p[n].vy > 0){ force = -2.0; }
	if (abs(p[n].vy) < 0.01) {	//�����ɂ���~
		p[n].vy = 0.0;
		force = 0;
	}
	WallCollistion(n);
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, ms_ruby.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ms_ruby.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ms_ruby.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, &ms_ruby.shininess);
	glTranslated(0.0, p[n].y, sphere_radius);	//���s�ړ��l�̐ݒ�
	glutSolidSphere(sphere_radius, 20, 20);	//�����F(���a, Z���܂��̕�����, Z���ɉ�����������)
	glPopMatrix();
}



//----------------------------------------------------
// ��(������)
//----------------------------------------------------
void Wall(void) {
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glBegin(GL_QUADS);							//GL_QUADS�F4�_�g�̎l�p�`
	glColor4fv(red);
	for (int j = 0; j < 6; ++j) {
		for (int i = 0; i < 4; ++i) {
			glVertex3dv(vertex_top[face[j][i]]);
		}
	}
	glEnd();
	glBegin(GL_QUADS);							//GL_QUADS�F4�_�g�̎l�p�`
	for (int j = 0; j < 6; ++j) {
		for (int i = 0; i < 4; ++i) {
			glVertex3dv(vertex_bottom[face[j][i]]);
		}
	}
	glEnd();
	glBegin(GL_QUADS);							//GL_QUADS�F4�_�g�̎l�p�`
	for (int j = 0; j < 6; ++j) {
		for (int i = 0; i < 4; ++i) {
			glVertex3dv(vertex_left[face[j][i]]);
		}
	}
	glEnd();
	glBegin(GL_QUADS);							//GL_QUADS�F4�_�g�̎l�p�`
	for (int j = 0; j < 6; ++j) {
		for (int i = 0; i < 4; ++i) {
			glVertex3dv(vertex_right[face[j][i]]);
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
	glPopMatrix();
}
//----------------------------------------------------
// ���̕`��
//----------------------------------------------------
void Hole(void) {
	glDisable(GL_LIGHTING);
	glColor4fv(black);
	glBegin(GL_QUADS);
	for (int j = 0; j < 6; ++j) {
		for (int i = 0; i < 4; ++i) {
			glVertex3dv(hole_vertex[face[j][i]]);
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
}

//----------------------------------------------------
// �����ʂ̕�����
//----------------------------------------------------
void findPlane(
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
void shadowMatrix(
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
// ���̕`��
//----------------------------------------------------
void DrawTable(bool bTexture) {
	if (!bTexture) {
		glDisable(GL_LIGHTING);
		glBegin(GL_QUADS);
		for (int j = 0; j < 3; ++j) {
			for (int i = 0; i < 4; ++i) {
				glVertex3dv(floor_vertex[floor_face[j][i]]);
			}
		}
		glEnd();
		glEnable(GL_LIGHTING);
	}
	Wall();
	Hole();
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
	glColor4fv(green);
	DrawTable(false);							//���̕`��

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
	DrawStructure(true);
	Sphere();
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
	DrawTable(false);									//���̕`��
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_STENCIL_TEST);
}

//----------------------------------------------------
// �L�[�{�[�h���͎��ɌĂяo�����֐�
//----------------------------------------------------
void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
		pn++;
		p[pn].x = -0.0;
		p[pn].y = -10.0;
		p[pn].z = 5.0;
		p[pn].vx = vx * ((double)rand() / (double)RAND_MAX - (double)rand() / (double)RAND_MAX);
		p[pn].vy = vy * ((double)rand() / (double)RAND_MAX - (double)rand() / (double)RAND_MAX);
		p[pn].vz = vz * ((double)rand() / (double)RAND_MAX);
		break;
	case '\040':	//SP
		key_space = true;
		break;
	case '\033':	//ESC��ASCII�R�[�h
		exit(0);
		break;
	default:
		break;
	}
}

void KeyboardUp(unsigned char key, int x, int y) {
	switch (key) {
	case '\040':	//SP
		key_space = false;
		force = add_force;
		add_force = 0.0;
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
	cout << x << " " << y << endl;
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