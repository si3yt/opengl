#include "billiards_header.h"

/* �f�B�X�v���C���X�g */
GLuint list_number;

ObjectControl object;
Timer space;

//----------------------------------------------------
// �֐��v���g�^�C�v�i��ɌĂяo���֐����ƈ����̐錾�j
//----------------------------------------------------
void initialize(void);
void idle(void);
void display(void);
void resize(int w, int h);
/* ���� */
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void specialFunc(int key, int x, int y);
void specialUpFunc(int key, int x, int y);

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
	glutDisplayFunc(display);										//�`�掞�ɌĂяo�����֐�
	glutReshapeFunc(resize);										//���T�C�Y���ɌĂяo�����֐�
	glutKeyboardFunc(keyboard);										//�L�[�{�[�h���͎��ɌĂяo�����֐�
	glutKeyboardUpFunc(keyboardUp);									//�L�[�{�[�h�������ꂽ���ɌĂяo�����֐�
	glutSpecialFunc(specialFunc);									//Special�L�[�{�[�h���͎��ɌĂяo�����֐�
	glutSpecialUpFunc(specialUpFunc);								//Special�L�[�{�[�h�������ꂽ���ɌĂяo�����֐�
	glutIgnoreKeyRepeat(GL_TRUE);									//�L�[�̌J��Ԃ����͖͂���

	glutIdleFunc(idle);												//�v���O�����A�C�h����Ԏ��ɌĂяo�����֐�
	initialize();													//�����ݒ�̊֐����Ăяo��
	glutMainLoop();													//�C�x���g�҂��󂯏��(�������[�v)
	return 0;
}

void idle() {
	glutPostRedisplay(); //glutDisplayFunc()���P����s����
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //�o�b�t�@�̏���

	glPushMatrix();

	object.draw();

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
	gluPerspective(30.0, (double)_WINDOW_WIDTH / (double)_WINDOW_HEIGHT, 0.1, 1000.0);	//�������e�@�̎��̐�
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
	glDisable(GL_CULL_FACE);

	/*
	FindPlane(floor_planar,				//�X�e���V���𒣂邽�߂̏���������
		_FLOOR_VER.v0,
		_FLOOR_VER.v1,
		_FLOOR_VER.v2);
	*/
	/* �����̐ݒ� */
	glEnable(GL_LIGHTING);									//�A�eON
	glEnable(GL_LIGHT0);									//����0�𗘗p
	glLightfv(GL_LIGHT0, GL_POSITION, _LIGHT_POSITION_0);	//����0�̈ʒu

	/* �f�B�X�v���C���X�g���쐬 */
	list_number = glGenLists(1);
	glNewList(list_number, GL_COMPILE);	//�R���p�C���̂�
	glEndList();

	/* �X�e���V���o�b�t�@�N���A�l�̐ݒ� */
	//glClearStencil(0);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_AUTO_NORMAL);
	//glEnable(GL_NORMALIZE);

	/* ���ʎˉe�s��̎Z�o */
	//ShadowMatrix(pM, floor_planar,light_pos_0);

	object.texture_init();

}

//----------------------------------------------------
// �L�[�{�[�h���͎��ɌĂяo�����֐�
//----------------------------------------------------
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case _KEY_SPACE:
		object.key_space = true;
		space.restart();
		break;
	case _KEY_ESC:
		exit(0);
		break;
	default:
		break;
	}
}

//----------------------------------------------------
// �L�[�{�[�h�������ꂽ���ɌĂяo�����֐�
//----------------------------------------------------
void keyboardUp(unsigned char key, int x, int y) {
	switch (key) {
	case _KEY_SPACE:
		object.key_space = false;
		object.push_for_space(space.elapsed());
		break;
	}
}

//----------------------------------------------------
// Special�L�[�{�[�h���͎��ɌĂяo�����֐�
//----------------------------------------------------
void specialFunc(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		object.key_left = true;
		break;
	case GLUT_KEY_RIGHT:
		object.key_right = true;
		break;
	}
}

//----------------------------------------------------
// Special�L�[�{�[�h�������ꂽ���ɌĂяo�����֐�
//----------------------------------------------------
void specialUpFunc(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		object.key_left = false;
		break;
	case GLUT_KEY_RIGHT:
		object.key_right = false;
		break;
	}
}


