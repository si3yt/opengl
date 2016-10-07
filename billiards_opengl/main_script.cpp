#include <gl/glut.h>
#include<iostream>
#include <stdio.h>
#include <stdlib.h>

#define MAXPOINTS 100      /* �L������_�̐��@�@ */
GLint point[MAXPOINTS][2]; /* ���W���L������z�� */
int pointnum = 0;          /* �L���������W�̐��@ */
int rubberband = 0;        /* ���o�[�o���h�̏��� */


void display(void) {
	int i;

	glClear(GL_COLOR_BUFFER_BIT);

	/* �L�^�����f�[�^�Ő���`�� */
	if (pointnum > 1) {
		glColor3d(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		for (i = 0; i < pointnum; ++i) {
			glVertex2iv(point[i]);
		}
		glEnd();
	}
	// �����s���߂��ׂĂ̎��s
	glFlush();
}

void resize(int w, int h)
{
	// �E�B���h�E�S�̂��r���[�|�[�g�ɂ���
	glViewport(0, 0, w, h);
	// �ϊ��s��̏�����
	glLoadIdentity();
	/* �X�N���[����̍��W�n���}�E�X�̍��W�n�Ɉ�v������ */
	glOrtho(-0.5, (GLdouble)w - 0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		/* �{�^���𑀍삵���ʒu���L�^���� */
		point[pointnum][0] = x;
		point[pointnum][1] = y;
		if (state == GLUT_UP) {
			/* �{�^�����������ʒu���痣�����ʒu�܂Ő������� */
			glColor3d(1.0, 1.0, 1.0);
			glBegin(GL_LINES);
			glVertex2iv(point[pointnum - 1]); /* ��O�͉������ʒu�@ */
			glVertex2iv(point[pointnum]);     /* ���̈ʒu�͗������ʒu */
			glEnd();
			glFlush();

			/* �n�_�ł̓��o�[�o���h��`���Ă��Ȃ��̂ŏ����Ȃ� */
			rubberband = 0;
		}
		if (pointnum < MAXPOINTS - 1) ++pointnum;
		break;
	case GLUT_MIDDLE_BUTTON:
		break;
	case GLUT_RIGHT_BUTTON:
		break;
	default:
		break;
	}
}

void motion(int x, int y)
{
	static GLint savepoint[2]; /* �ȑO�̃��o�[�o���h�̒[�_ */

	/* �_�����Z�@�\ ON */
	glEnable(GL_COLOR_LOGIC_OP);
	glLogicOp(GL_INVERT);

	glBegin(GL_LINES);
	if (rubberband) {
		/* �ȑO�̃��o�[�o���h������ */
		glVertex2iv(point[pointnum - 1]);
		glVertex2iv(savepoint);
	}
	/* �V�������o�[�o���h��`�� */
	glVertex2iv(point[pointnum - 1]);
	glVertex2i(x, y);
	glEnd();

	glFlush();

	/* �_�����Z�@�\ OFF */
	glLogicOp(GL_COPY);
	glDisable(GL_COLOR_LOGIC_OP);

	/* ���`�������o�[�o���h�̒[�_��ۑ� */
	savepoint[0] = x;
	savepoint[1] = y;

	/* ���`�������o�[�o���h�͎��̃^�C�~���O�ŏ��� */
	rubberband = 1;
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '\033':  /* '\033' �� ESC �� ASCII �R�[�h */
		exit(0);
	default:
		break;
	}
}

// �����ݒ�p�֐�
void init(void) {
	/* ��ʂ�h��Ԃ��F
	 * GLclampf�^:0~1�̒l */
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

int main(int argc, char *argv[]) {
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 240);
	// GLUT�����OpenGL���̏�����
	glutInit(&argc, argv);
	// �f�B�X�v���C�̕\�����[�h
	glutInitDisplayMode(GLUT_RGBA);
	// �E�B���h�E�쐬
	glutCreateWindow(argv[0]);
	// �}�`�`��p�֐�
	glutDisplayFunc(display);
	// ���T�C�Y�̎��s
	glutReshapeFunc(resize);
	// �}�E�XUpDown�C�x���g
	glutMouseFunc(mouse);
	// �}�E�X�h���b�O�C�x���g
	glutMotionFunc(motion);
	// �L�[�{�[�h���̓C�x���g
	glutKeyboardFunc(keyboard);
	init();
	// �C�x���g�҂��󂯏��(�������[�v)
	glutMainLoop();

	return 0;
}