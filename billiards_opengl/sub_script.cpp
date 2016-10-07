#include <gl/glut.h>

GLdouble vertex[][3] = {
	{ 0.0, 0.0, 0.0 },
	{ 1.0, 0.0, 0.0 },
	{ 1.0, 1.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	{ 1.0, 0.0, 1.0 },
	{ 1.0, 1.0, 1.0 },
	{ 0.0, 1.0, 1.0 }
};

int face[][4] = {
	{ 0, 1, 2, 3 },
	{ 1, 5, 6, 2 },
	{ 5, 4, 7, 6 },
	{ 4, 0, 3, 7 },
	{ 4, 5, 1, 0 },
	{ 3, 2, 6, 7 }
};

GLdouble normal[][3] = {
	{ 0.0, 0.0,-1.0 },
	{ 1.0, 0.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	{ -1.0, 0.0, 0.0 },
	{ 0.0,-1.0, 0.0 },
	{ 0.0, 1.0, 0.0 }
};

GLfloat light0pos[] = { 0.0, 3.0, 5.0, 1.0 };
GLfloat light1pos[] = { 5.0, 3.0, 0.0, 1.0 };

GLfloat green[] = { 0.0, 1.0, 0.0, 1.0 };
GLfloat red[] = { 0.8, 0.2, 0.2, 1.0 };
GLfloat blue[] = { 0.2, 0.2, 0.8, 1.0 };

void cube(void)
{
	int i;
	int j;

	glBegin(GL_QUADS);
	for (j = 0; j < 6; ++j) {
		glNormal3dv(normal[j]);
		for (i = 0; i < 4; ++i) {
			glVertex3dv(vertex[face[j][i]]);
		}
	}
	glEnd();
}

void idle(void)
{
	glutPostRedisplay();
}

void display(void) {
	static int r = 0; /* 回転角 */

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* 光源の位置設定 */
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
	glLightfv(GL_LIGHT1, GL_POSITION, light1pos);

	/* モデルビュー変換行列の保存 */
	glPushMatrix();
	
	/* 図形の回転 */
	glRotated((double)r, 0.0, 1.0, 0.0);

	/* 図形の色 (赤)  */
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);
	
	/* 図形の描画 */
	cube();

	/* 二つ目の図形の描画 */
	glPushMatrix();
	glTranslated(1.0, 1.0, 1.0);
	glRotated((double)(2 * r), 0.0, 1.0, 0.0);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue);
	cube();
	glPopMatrix();

	/* モデルビュー変換行列の復帰 */
	glPopMatrix();

	glutSwapBuffers();

	/* 一周回ったら回転角を 0 に戻す */
	if (++r >= 360) r = 0;
}

void resize(int w, int h)
{
	glViewport(0, 0, w, h);

	/* 透視変換行列の設定 */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);
	
	/* モデルビュー変換行列の設定 */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(3.0, 4.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			/* アニメーション開始 */
			glutIdleFunc(idle);
		}
		else {
			/* アニメーション停止 */
			glutIdleFunc(0);
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			/* コマ送り (1ステップだけ進める) */
			glutPostRedisplay();
		}
		break;
	default:
		break;
	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '\033':  /* '\033' は ESC の ASCII コード */
		exit(0);
	default:
		break;
	}
}

void init(void) {
	/* 画面を塗りつぶす色
	* GLclampf型:0~1の値 */
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, green);
	glLightfv(GL_LIGHT1, GL_SPECULAR, green);
}

int main(int argc, char *argv[]) {
	// GLUTおよびOpenGL環境の初期化
	glutInit(&argc, argv);
	// ディスプレイの表示モード
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	// ウィンドウ作成
	glutCreateWindow(argv[0]);
	// 図形描画用関数
	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	init();
	// イベント待ち受け状態(無限ループ)
	glutMainLoop();

	return 0;
}