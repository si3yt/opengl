#include <gl/glut.h>
#include<iostream>
#include <stdio.h>
#include <stdlib.h>

#define MAXPOINTS 100      /* 記憶する点の数　　 */
GLint point[MAXPOINTS][2]; /* 座標を記憶する配列 */
int pointnum = 0;          /* 記憶した座標の数　 */
int rubberband = 0;        /* ラバーバンドの消去 */


void display(void) {
	int i;

	glClear(GL_COLOR_BUFFER_BIT);

	/* 記録したデータで線を描く */
	if (pointnum > 1) {
		glColor3d(1.0, 1.0, 1.0);
		glBegin(GL_LINES);
		for (i = 0; i < pointnum; ++i) {
			glVertex2iv(point[i]);
		}
		glEnd();
	}
	// 未実行命令すべての実行
	glFlush();
}

void resize(int w, int h)
{
	// ウィンドウ全体をビューポートにする
	glViewport(0, 0, w, h);
	// 変換行列の初期化
	glLoadIdentity();
	/* スクリーン上の座標系をマウスの座標系に一致させる */
	glOrtho(-0.5, (GLdouble)w - 0.5, (GLdouble)h - 0.5, -0.5, -1.0, 1.0);
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		/* ボタンを操作した位置を記録する */
		point[pointnum][0] = x;
		point[pointnum][1] = y;
		if (state == GLUT_UP) {
			/* ボタンを押した位置から離した位置まで線を引く */
			glColor3d(1.0, 1.0, 1.0);
			glBegin(GL_LINES);
			glVertex2iv(point[pointnum - 1]); /* 一つ前は押した位置　 */
			glVertex2iv(point[pointnum]);     /* 今の位置は離した位置 */
			glEnd();
			glFlush();

			/* 始点ではラバーバンドを描いていないので消さない */
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
	static GLint savepoint[2]; /* 以前のラバーバンドの端点 */

	/* 論理演算機能 ON */
	glEnable(GL_COLOR_LOGIC_OP);
	glLogicOp(GL_INVERT);

	glBegin(GL_LINES);
	if (rubberband) {
		/* 以前のラバーバンドを消す */
		glVertex2iv(point[pointnum - 1]);
		glVertex2iv(savepoint);
	}
	/* 新しいラバーバンドを描く */
	glVertex2iv(point[pointnum - 1]);
	glVertex2i(x, y);
	glEnd();

	glFlush();

	/* 論理演算機能 OFF */
	glLogicOp(GL_COPY);
	glDisable(GL_COLOR_LOGIC_OP);

	/* 今描いたラバーバンドの端点を保存 */
	savepoint[0] = x;
	savepoint[1] = y;

	/* 今描いたラバーバンドは次のタイミングで消す */
	rubberband = 1;
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

// 初期設定用関数
void init(void) {
	/* 画面を塗りつぶす色
	 * GLclampf型:0~1の値 */
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

int main(int argc, char *argv[]) {
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(320, 240);
	// GLUTおよびOpenGL環境の初期化
	glutInit(&argc, argv);
	// ディスプレイの表示モード
	glutInitDisplayMode(GLUT_RGBA);
	// ウィンドウ作成
	glutCreateWindow(argv[0]);
	// 図形描画用関数
	glutDisplayFunc(display);
	// リサイズの実行
	glutReshapeFunc(resize);
	// マウスUpDownイベント
	glutMouseFunc(mouse);
	// マウスドラッグイベント
	glutMotionFunc(motion);
	// キーボード入力イベント
	glutKeyboardFunc(keyboard);
	init();
	// イベント待ち受け状態(無限ループ)
	glutMainLoop();

	return 0;
}