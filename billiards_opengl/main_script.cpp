#include <gl/glut.h>

void display(void) {
	/* ウィンドウの塗りつぶし
	 * 対象:カラーバッファ*/
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POLYGON);
	glColor3d(1.0, 0.0, 0.0); /* 赤 */
	glVertex2d(-0.9, -0.9);
	glColor3d(0.0, 1.0, 0.0); /* 緑 */
	glVertex2d(0.9, -0.9);
	glColor3d(0.0, 0.0, 1.0); /* 青 */
	glVertex2d(0.9, 0.9);
	glColor3d(1.0, 1.0, 0.0); /* 黄 */
	glVertex2d(-0.9, 0.9);
	glEnd();
	// 未実行命令すべての実行
	glFlush();
}

void resize(int w, int h)
{
	// ウィンドウ全体をビューポートにする
	glViewport(0, 0, w, h);
	// 変換行列の初期化
	glLoadIdentity();
	// スクリーン上の表示領域をビューポートの大きさに比例させる
	glOrtho(-w / 200.0, w / 200.0, -h / 200.0, h / 200.0, -1.0, 1.0);
}

// 初期設定用関数
void init(void) {
	/* 画面を塗りつぶす色
	 * GLclampf型:0~1の値 */
	glClearColor(0.0, 0.0, 0.0, 1.0);
}

int main(int argc, char *argv[]) {
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
	init();
	// イベント待ち受け状態(無限ループ)
	glutMainLoop();

	return 0;
}