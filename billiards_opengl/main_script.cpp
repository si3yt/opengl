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

/* ディスプレイリスト */
GLuint list_number;

//----------------------------------------------------
// 関数プロトタイプ（後に呼び出す関数名と引数の宣言）
//----------------------------------------------------
void initialize(void);
void idle(void);
void display(void);
void resize(int w, int h);

GLint main(int argc, char *argv[]) {
	srand((unsigned)time(NULL));								//乱数の発生
	glutInit(&argc, argv);										//環境の初期化
	glutInitWindowPosition(win_pos_x, win_pos_y);				//ウィンドウの位置の指定
	glutInitWindowSize(win_width, win_height);					//ウィンドウサイズの指定
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);	//ディスプレイモードの指定(RGBA,デプスバッファ,ダブルバッファ)
	glutCreateWindow(win_title);								//ウィンドウの作成
	glutDisplayFunc(display);									//描画時に呼び出される関数
	glutReshapeFunc(resize);									//リサイズ時に呼び出される関数
	//glutKeyboardFunc(Keyboard);										//キーボード入力時に呼び出される関数
	//glutKeyboardUpFunc(KeyboardUp);									//キーボードが離された時に呼び出される関数
	//glutSpecialFunc(SpecialFunc);									//Specialキーボード入力時に呼び出される関数
	//glutSpecialUpFunc(SpecialUpFunc);								//Specialキーボードが離された時に呼び出される関数
	//glutIgnoreKeyRepeat(GL_TRUE);									//キーの繰り返し入力は無視
	//glutMouseFunc(MouseOn);											//マウスクリック時に呼び出される関数
	//glutMotionFunc(MouseMotion);									//マウスドラッグ解除時に呼び出される関数

	glutIdleFunc(idle);											//プログラムアイドル状態時に呼び出される関数
	initialize();												//初期設定の関数を呼び出す
	glutMainLoop();												//イベント待ち受け状態(無限ループ)
	return 0;
}

void idle() {
	glutPostRedisplay(); //glutDisplayFunc()を１回実行する
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //バッファの消去

	glPushMatrix();

	/* 視点の設定 */
	gluLookAt(
		200,200,200,//look_x, look_y, 200.0,	//視点の位置x,y,z;
		0,0,0,//p[0].x, p[0].y, p[0].z,	//視界の中心位置の参照点座標x,y,z
		0.0, 0.0, 1.0);		//視界の上方向のベクトルx,y,z

							/* 回転 */
	//glMultMatrixd(rt);		//任意の行列を積算する関数

							/* 図形 */
	//DrawShadow();
	//DrawSphereOfPlayer();		//自球
	//DrawSphereOfTarget();		//他球
	//DrawCylinder();				//キュー

	//DrawWall();
	

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
	gluPerspective(30.0, (double)win_width / (double)win_height, 0.1, 1000.0);	//透視投影法の視体積
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

	/*
	FindPlane(floor_planar,				//ステンシルを張るための床を見つける
		_FLOOR_VER.v0,
		_FLOOR_VER.v1,
		_FLOOR_VER.v2);
	*/
	/* 光源の設定 */
	glEnable(GL_LIGHTING);									//陰影ON
	glEnable(GL_LIGHT0);									//光源0を利用
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos_0);	//光源0の位置

															/* ディスプレイリストを作成 */
	list_number = glGenLists(1);
	glNewList(list_number, GL_COMPILE);	//コンパイルのみ
	glEndList();

	/* マウスポインタ位置のウィンドウ内の相対的位置への換算用 */
	//sx = 1.0 / (double)512;
	//sy = 1.0 / (double)512;

	/* 回転行列の初期化 */
	//Qrot(rt, cq);

	/* ステンシルバッファクリア値の設定 */
	//glClearStencil(0);
	//glCullFace(GL_BACK);
	//glEnable(GL_CULL_FACE);
	//glEnable(GL_AUTO_NORMAL);
	//glEnable(GL_NORMALIZE);

	/* 平面射影行列の算出 */
	//ShadowMatrix(pM, floor_planar,light_pos_0);

	/* 物体配置 */
	//InitialSphere();
	
}
