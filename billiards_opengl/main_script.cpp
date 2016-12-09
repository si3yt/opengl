#include "billiards_header.h"

/* ディスプレイリスト */
GLuint list_number;

ObjectControl object;
Timer space;

//----------------------------------------------------
// 関数プロトタイプ（後に呼び出す関数名と引数の宣言）
//----------------------------------------------------
void initialize(void);
void idle(void);
void display(void);
void resize(int w, int h);
/* 入力 */
void keyboard(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void specialFunc(int key, int x, int y);
void specialUpFunc(int key, int x, int y);

//----------------------------------------------------
// メイン関数
//----------------------------------------------------
GLint main(int argc, char *argv[]) {
	srand((unsigned)time(NULL));									//乱数の発生
	glutInit(&argc, argv);											//環境の初期化
	glutInitWindowPosition(_WINDOW_POSITION_X, _WINDOW_POSITION_Y);	//ウィンドウの位置の指定
	glutInitWindowSize(_WINDOW_WIDTH, _WINDOW_HEIGHT);				//ウィンドウサイズの指定
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);		//ディスプレイモードの指定(RGBA,デプスバッファ,ダブルバッファ)
	glutCreateWindow(_WINDOW_TITLE);								//ウィンドウの作成
	glutDisplayFunc(display);										//描画時に呼び出される関数
	glutReshapeFunc(resize);										//リサイズ時に呼び出される関数
	glutKeyboardFunc(keyboard);										//キーボード入力時に呼び出される関数
	glutKeyboardUpFunc(keyboardUp);									//キーボードが離された時に呼び出される関数
	glutSpecialFunc(specialFunc);									//Specialキーボード入力時に呼び出される関数
	glutSpecialUpFunc(specialUpFunc);								//Specialキーボードが離された時に呼び出される関数
	glutIgnoreKeyRepeat(GL_TRUE);									//キーの繰り返し入力は無視

	glutIdleFunc(idle);												//プログラムアイドル状態時に呼び出される関数
	initialize();													//初期設定の関数を呼び出す
	glutMainLoop();													//イベント待ち受け状態(無限ループ)
	return 0;
}

void idle() {
	glutPostRedisplay(); //glutDisplayFunc()を１回実行する
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //バッファの消去

	glPushMatrix();

	object.draw();

	glPopMatrix();

	glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)でダブルバッファリングを利用可
}

//----------------------------------------------------
// リサイズ関数
//----------------------------------------------------
void resize(int w, int h) {
	glViewport(0, 0, w, h);	//ビューポートの設定

	/* 透視変換行列の設定 */
	glMatrixMode(GL_PROJECTION);	//行列モードの設定GL_PROJECTION : 透視変換行列の設定、GL_MODELVIEW：モデルビュー変換行列）
	glLoadIdentity();															//行列の初期化(変換行列に単位行列を設定)
	gluPerspective(30.0, (double)_WINDOW_WIDTH / (double)_WINDOW_HEIGHT, 0.1, 1000.0);	//透視投影法の視体積
	//gluPerspactive(画角, アスペクト比, 奥行き前, 奥行き後ろ);

	/* モデルビュー変換行列の設定 */
	glMatrixMode(GL_MODELVIEW);						//行列モードの設定（GL_PROJECTION : 透視変換行列の設定、GL_MODELVIEW：モデルビュー変換行列）
	glLoadIdentity();								//行列の初期化(変換行列に単位行列を設定)
}

//----------------------------------------------------
// 初期設定の関数
//----------------------------------------------------
void initialize(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);	//背景色(RGBA 0.0~1.0)
	glEnable(GL_DEPTH_TEST);			//デプスバッファを使用：glutInitDisplayMode() で GLUT_DEPTH を指定する
	glDepthFunc(GL_LEQUAL);				//深度バッファと新しいピクセル地の深度の比較関数(GL_LEQUAL：格納深度以下であれば通過)
	glClearDepth(1.0);
	glDisable(GL_CULL_FACE);

	/* 光源の設定 */
	glEnable(GL_LIGHTING);									//陰影ON
	glEnable(GL_LIGHT0);									//光源0を利用
	glLightfv(GL_LIGHT0, GL_POSITION, _LIGHT_POSITION_0);	//光源0の位置

	/* ディスプレイリストを作成 */
	list_number = glGenLists(1);
	glNewList(list_number, GL_COMPILE);	//コンパイルのみ
	glEndList();

	object.texture_init();

}

//----------------------------------------------------
// キーボード入力時に呼び出される関数
//----------------------------------------------------
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case _KEY_SPACE:
		if (object.ball_all_stop()) {
			object.key_space = true;
			space.restart();
		}
		break;
	case _KEY_ESC:
		exit(0);
		break;
	default:
		break;
	}
	if (key == 'r') {
		ObjectControl new_object;
		object = new_object;
		object.texture_init();
	}
}

//----------------------------------------------------
// キーボードが離された時に呼び出される関数
//----------------------------------------------------
void keyboardUp(unsigned char key, int x, int y) {
	switch (key) {
	case _KEY_SPACE:
		if (object.key_space) {
			object.key_space = false;
			object.push_for_space(space.elapsed());
		}
		break;
	}
}

//----------------------------------------------------
// Specialキーボード入力時に呼び出される関数
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
// Specialキーボードが離された時に呼び出される関数
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


