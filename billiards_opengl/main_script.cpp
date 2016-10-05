#include <gl/glut.h>

void display(void) {
	/* �E�B���h�E�̓h��Ԃ�
	 * �Ώ�:�J���[�o�b�t�@*/
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POLYGON);
	glColor3d(1.0, 0.0, 0.0); /* �� */
	glVertex2d(-0.9, -0.9);
	glColor3d(0.0, 1.0, 0.0); /* �� */
	glVertex2d(0.9, -0.9);
	glColor3d(0.0, 0.0, 1.0); /* �� */
	glVertex2d(0.9, 0.9);
	glColor3d(1.0, 1.0, 0.0); /* �� */
	glVertex2d(-0.9, 0.9);
	glEnd();
	// �����s���߂��ׂĂ̎��s
	glFlush();
}

void resize(int w, int h)
{
	// �E�B���h�E�S�̂��r���[�|�[�g�ɂ���
	glViewport(0, 0, w, h);
	// �ϊ��s��̏�����
	glLoadIdentity();
	// �X�N���[����̕\���̈���r���[�|�[�g�̑傫���ɔ�Ⴓ����
	glOrtho(-w / 200.0, w / 200.0, -h / 200.0, h / 200.0, -1.0, 1.0);
}

// �����ݒ�p�֐�
void init(void) {
	/* ��ʂ�h��Ԃ��F
	 * GLclampf�^:0~1�̒l */
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

int main(int argc, char *argv[]) {
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
	init();
	// �C�x���g�҂��󂯏��(�������[�v)
	glutMainLoop();

	return 0;
}