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

GLint win_pos_x = _WINDOW_POSITION_X;
GLint win_pos_y = _WINDOW_POSITION_Y;
GLint win_width = _WINDOW_WIDTH;
GLint win_height = _WINDOW_HEIGHT;
char* win_title = _WINDOW_TITLE;

GLfloat light_pos_0[] = _LIGHT_POSITION_0;

/* �f�B�X�v���C���X�g */
GLuint list_number;

//----------------------------------------------------
// �֐��v���g�^�C�v�i��ɌĂяo���֐����ƈ����̐錾�j
//----------------------------------------------------
void initialize(void);
void idle(void);
void display(void);
void resize(int w, int h);

GLint main(int argc, char *argv[]) {
	srand((unsigned)time(NULL));								//�����̔���
	glutInit(&argc, argv);										//���̏�����
	glutInitWindowPosition(win_pos_x, win_pos_y);				//�E�B���h�E�̈ʒu�̎w��
	glutInitWindowSize(win_width, win_height);					//�E�B���h�E�T�C�Y�̎w��
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);	//�f�B�X�v���C���[�h�̎w��(RGBA,�f�v�X�o�b�t�@,�_�u���o�b�t�@)
	glutCreateWindow(win_title);								//�E�B���h�E�̍쐬
	glutDisplayFunc(display);									//�`�掞�ɌĂяo�����֐�
	glutReshapeFunc(resize);									//���T�C�Y���ɌĂяo�����֐�
	//glutKeyboardFunc(Keyboard);										//�L�[�{�[�h���͎��ɌĂяo�����֐�
	//glutKeyboardUpFunc(KeyboardUp);									//�L�[�{�[�h�������ꂽ���ɌĂяo�����֐�
	//glutSpecialFunc(SpecialFunc);									//Special�L�[�{�[�h���͎��ɌĂяo�����֐�
	//glutSpecialUpFunc(SpecialUpFunc);								//Special�L�[�{�[�h�������ꂽ���ɌĂяo�����֐�
	//glutIgnoreKeyRepeat(GL_TRUE);									//�L�[�̌J��Ԃ����͖͂���
	//glutMouseFunc(MouseOn);											//�}�E�X�N���b�N���ɌĂяo�����֐�
	//glutMotionFunc(MouseMotion);									//�}�E�X�h���b�O�������ɌĂяo�����֐�

	glutIdleFunc(idle);											//�v���O�����A�C�h����Ԏ��ɌĂяo�����֐�
	initialize();												//�����ݒ�̊֐����Ăяo��
	glutMainLoop();												//�C�x���g�҂��󂯏��(�������[�v)
	return 0;
}

void idle() {
	glutPostRedisplay(); //glutDisplayFunc()���P����s����
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //�o�b�t�@�̏���

	glPushMatrix();

	/* ���_�̐ݒ� */
	gluLookAt(
		200,200,200,//look_x, look_y, 200.0,	//���_�̈ʒux,y,z;
		0,0,0,//p[0].x, p[0].y, p[0].z,	//���E�̒��S�ʒu�̎Q�Ɠ_���Wx,y,z
		0.0, 0.0, 1.0);		//���E�̏�����̃x�N�g��x,y,z

							/* ��] */
	//glMultMatrixd(rt);		//�C�ӂ̍s���ώZ����֐�

							/* �}�` */
	//DrawShadow();
	//DrawSphereOfPlayer();		//����
	//DrawSphereOfTarget();		//����
	//DrawCylinder();				//�L���[

	//DrawWall();
	

	glPopMatrix();

	glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)�Ń_�u���o�b�t�@�����O�𗘗p��
}

//----------------------------------------------------
// ���T�C�Y�֐�
//----------------------------------------------------
void resize(int w, int h) {
	glViewport(0, 0, w, h);	//�r���[�|�[�g�̐ݒ�

	/* �����ϊ��s��̐ݒ� */
	glMatrixMode(GL_PROJECTION);	//�s�񃂁[�h�̐ݒ�GL_PROJECTION : �����ϊ��s��̐ݒ�AGL_MODELVIEW�F���f���r���[�ϊ��s��j
	glLoadIdentity();															//�s��̏�����(�ϊ��s��ɒP�ʍs���ݒ�)
	gluPerspective(30.0, (double)win_width / (double)win_height, 0.1, 1000.0);	//�������e�@�̎��̐�
	//gluPerspactive(��p, �A�X�y�N�g��, ���s���O, ���s�����);

	/* ���f���r���[�ϊ��s��̐ݒ� */
	glMatrixMode(GL_MODELVIEW);						//�s�񃂁[�h�̐ݒ�iGL_PROJECTION : �����ϊ��s��̐ݒ�AGL_MODELVIEW�F���f���r���[�ϊ��s��j
	glLoadIdentity();								//�s��̏�����(�ϊ��s��ɒP�ʍs���ݒ�)
}

//----------------------------------------------------
// �����ݒ�̊֐�
//----------------------------------------------------
void initialize(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);	//�w�i�F(RGBA 0.0~1.0)
	glEnable(GL_DEPTH_TEST);			//�f�v�X�o�b�t�@���g�p�FglutInitDisplayMode() �� GLUT_DEPTH ���w�肷��
	glDepthFunc(GL_LEQUAL);				//�[�x�o�b�t�@�ƐV�����s�N�Z���n�̐[�x�̔�r�֐�(GL_LEQUAL�F�i�[�[�x�ȉ��ł���Βʉ�)
	glClearDepth(1.0);

	/*
	FindPlane(floor_planar,				//�X�e���V���𒣂邽�߂̏���������
		_FLOOR_VER.v0,
		_FLOOR_VER.v1,
		_FLOOR_VER.v2);
	*/
	/* �����̐ݒ� */
	glEnable(GL_LIGHTING);									//�A�eON
	glEnable(GL_LIGHT0);									//����0�𗘗p
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos_0);	//����0�̈ʒu

															/* �f�B�X�v���C���X�g���쐬 */
	list_number = glGenLists(1);
	glNewList(list_number, GL_COMPILE);	//�R���p�C���̂�
	glEndList();

	/* �}�E�X�|�C���^�ʒu�̃E�B���h�E���̑��ΓI�ʒu�ւ̊��Z�p */
	//sx = 1.0 / (double)512;
	//sy = 1.0 / (double)512;

	/* ��]�s��̏����� */
	//Qrot(rt, cq);

	/* �X�e���V���o�b�t�@�N���A�l�̐ݒ� */
	//glClearStencil(0);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_AUTO_NORMAL);
	//glEnable(GL_NORMALIZE);

	/* ���ʎˉe�s��̎Z�o */
	//ShadowMatrix(pM, floor_planar,light_pos_0);

	/* ���̔z�u */
	//InitialSphere();
	
}
