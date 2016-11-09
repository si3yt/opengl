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
using namespace std;

//----------------------------------------------------
// 変数宣言
//----------------------------------------------------
/* ウィンドウ変数 */
static int  _WINDOW_POSITION_X = 100; 
static int  _WINDOW_POSITION_Y = 100;
static int  _WINDOW_WIDTH      = 512;
static int  _WINDOW_HEIGHT     = 512; 
static char _WINDOW_TITLE[]    = "billiards";

/* マウス回転 */
#define SCALE (2.0 * 3.14159265358979323846)	// マウスの相対位置→回転角の換算係数
int    cx, cy;									// ドラッグ開始位置
double sx, sy;									// マウスの絶対位置→ウィンドウ内での相対位置の換算係数
double cq[4] = { 1.0, 0.0, 0.0, 0.0 };			// 回転の初期値 (クォータニオン)
double tq[4];									// ドラッグ中の回転 (クォータニオン)
double rt[16];									// 回転の変換行列
float  camera_z_pos = 50.0;

/* ディスプレイリスト */
GLuint list_number;

/* 物質質感(山本昌志氏の構造体を利用) */
struct _MATERIAL_STRUCT {							// material用構造体
	GLfloat ambient[4];		// 環境反射
	GLfloat diffuse[4];		// 拡散反射
	GLfloat specular[4];	// 鏡面反射
	GLfloat shininess;		// 光沢
};
_MATERIAL_STRUCT _MS_WHITE_PLASTIC = {				//プラスチック(白)
	{ 0.0f,  0.0f,  0.0f,  1.0f },
	{ 0.55f, 0.55f, 0.55f, 1.0f },
	{ 0.70f, 0.70f, 0.70f, 1.0f },
	32.0f };
_MATERIAL_STRUCT _MS_RUBY = {						// ruby(ルビー)
	{ 0.1745f,   0.01175f,  0.01175f,   1.0f },
	{ 0.61424f,  0.04136f,  0.04136f,   1.0f },
	{ 0.727811f, 0.626959f, 0.626959f,  1.0f },
	76.8f };

/* 色 */
GLfloat _RED[]     = { 0.8f, 0.2f, 0.2f, 1.0f };	//赤色
GLfloat _GREEN[]   = { 0.2f, 0.8f, 0.2f, 1.0f };	//緑色
GLfloat _BLUE[]    = { 0.2f, 0.2f, 0.8f, 1.0f };	//青色
GLfloat _YELLOW[]  = { 0.8f, 0.8f, 0.2f, 1.0f };	//黄色
GLfloat _WHITE[]   = { 1.0f, 1.0f, 1.0f, 1.0f };	//白色
GLfloat _BLACK[]   = { 0.0f, 0.0f, 0.0f, 1.0f };	//黒色
GLfloat _SHININESS = 30.0f;						//光沢の強さ

/* 光 */
static GLfloat _LIGHT_POSITION_0[] = { 0.0, 0.0, 150.0, 1.0 }; //光源0の座標

/* 床 */
static GLfloat floor_planar[4];			//床
static GLfloat _FLOOR_S = 50.0f;		//床サイズ用係数
static GLfloat _WALL_S = 10.0f;			//壁サイズ用係数
static GLfloat pM[16];					//shadowMatrix：m[]
static GLfloat _FLOOR_OFFSET = 12.0f;	//穴用床オフセット

typedef struct _QUADS_VERTEX {
	GLfloat v0[3];
	GLfloat v1[3];
	GLfloat v2[3];
	GLfloat v3[3];
}QUADS_VERTEX;
static QUADS_VERTEX _FLOOR_VER = {		//影描画用床設定
	{ _FLOOR_S,  _FLOOR_S,  0.0f },
	{ -_FLOOR_S, _FLOOR_S,  0.0f },
	{ -_FLOOR_S, -_FLOOR_S, 0.0f },
	{ _FLOOR_S,  -_FLOOR_S, 0.0f },
};

static GLdouble _FLOOR_VERTEX[][3] = {					//床頂点
	{ _FLOOR_S  -_FLOOR_OFFSET, _FLOOR_S*2,					0.0f },
	{ -_FLOOR_S +_FLOOR_OFFSET, _FLOOR_S*2,					0.0f },
	{ -_FLOOR_S +_FLOOR_OFFSET, _FLOOR_S*2  -_FLOOR_OFFSET, 0.0f },
	{ -_FLOOR_S,				_FLOOR_S*2  -_FLOOR_OFFSET, 0.0f },
	{ -_FLOOR_S,				-_FLOOR_S*2 +_FLOOR_OFFSET, 0.0f },
	{ -_FLOOR_S +_FLOOR_OFFSET, -_FLOOR_S*2 +_FLOOR_OFFSET, 0.0f },
	{ -_FLOOR_S +_FLOOR_OFFSET, -_FLOOR_S*2,				0.0f },
	{ _FLOOR_S  -_FLOOR_OFFSET, -_FLOOR_S*2,				0.0f },
	{ _FLOOR_S  -_FLOOR_OFFSET, -_FLOOR_S*2 +_FLOOR_OFFSET, 0.0f },
	{ _FLOOR_S,				    -_FLOOR_S*2 +_FLOOR_OFFSET, 0.0f },
	{ _FLOOR_S,				    _FLOOR_S*2  -_FLOOR_OFFSET, 0.0f },
	{ _FLOOR_S  -_FLOOR_OFFSET, _FLOOR_S*2  -_FLOOR_OFFSET, 0.0f },
};
static GLint _FLOOR_FACE[][4] = {						//床面
	{0,  1,  6,  7},
	{2,  3,  4,  5},
	{10, 11, 8,  9}
};

/* 穴 */
static GLdouble _HOLE_VERTEX[][3] = {					//穴頂点
	{ -_FLOOR_S, -_FLOOR_S*2, -_WALL_S   },
	{ _FLOOR_S,  -_FLOOR_S*2, -_WALL_S   },
	{ _FLOOR_S,  _FLOOR_S*2,  -_WALL_S   },
	{ -_FLOOR_S, _FLOOR_S*2,  -_WALL_S   },
	{ -_FLOOR_S, -_FLOOR_S*2, -_WALL_S/2 },
	{ _FLOOR_S,  -_FLOOR_S*2, -_WALL_S/2 },
	{ _FLOOR_S,  _FLOOR_S*2,  -_WALL_S/2 },
	{ -_FLOOR_S, _FLOOR_S*2,  -_WALL_S/2 },
};

/* 壁 */
static GLdouble _WALL_TOP_VERTEX[][3] = {				// 上壁頂点
	{ -_FLOOR_S, _FLOOR_S*2,		  -_WALL_S },
	{ _FLOOR_S,  _FLOOR_S*2,		  -_WALL_S },
	{ _FLOOR_S,  _FLOOR_S*2 +_WALL_S, -_WALL_S },
	{ -_FLOOR_S, _FLOOR_S*2 +_WALL_S, -_WALL_S },
	{ -_FLOOR_S, _FLOOR_S*2,		  _WALL_S  },
	{ _FLOOR_S,  _FLOOR_S*2,		  _WALL_S  },
	{ _FLOOR_S,  _FLOOR_S*2 +_WALL_S, _WALL_S  },
	{ -_FLOOR_S, _FLOOR_S*2 +_WALL_S, _WALL_S  },
};
static GLdouble _WALL_BOTTOM_VERTEX[][3] = {			// 下壁頂点
	{ -_FLOOR_S, -_FLOOR_S*2 -_WALL_S, -_WALL_S },
	{ _FLOOR_S,  -_FLOOR_S*2 -_WALL_S, -_WALL_S },
	{ _FLOOR_S,  -_FLOOR_S*2,		   -_WALL_S },
	{ -_FLOOR_S, -_FLOOR_S*2,		   -_WALL_S },
	{ -_FLOOR_S, -_FLOOR_S*2 -_WALL_S, _WALL_S  },
	{ _FLOOR_S,  -_FLOOR_S*2 -_WALL_S, _WALL_S  },
	{ _FLOOR_S,  -_FLOOR_S*2,		   _WALL_S  },
	{ -_FLOOR_S, -_FLOOR_S*2,		   _WALL_S  },
};
static GLdouble _WALL_LEFT_VERTEX[][3] = {				// 左壁頂点
	{ -_FLOOR_S -_WALL_S, -_FLOOR_S*2 -_WALL_S,	-_WALL_S },
	{ -_FLOOR_S,		  -_FLOOR_S*2 -_WALL_S,	-_WALL_S },
	{ -_FLOOR_S,          _FLOOR_S*2  +_WALL_S, -_WALL_S },
	{ -_FLOOR_S -_WALL_S, _FLOOR_S*2  +_WALL_S, -_WALL_S },
	{ -_FLOOR_S -_WALL_S, -_FLOOR_S*2 -_WALL_S,	_WALL_S  },
	{ -_FLOOR_S,		  -_FLOOR_S*2 -_WALL_S,	_WALL_S  },
	{ -_FLOOR_S,		  _FLOOR_S*2  +_WALL_S, _WALL_S  },
	{ -_FLOOR_S -_WALL_S, _FLOOR_S*2  +_WALL_S, _WALL_S  },
};
static GLdouble _WALL_RIGHT_VERTEX[][3] = {				// 右壁頂点
	{ _FLOOR_S,		     -_FLOOR_S*2 -_WALL_S, -_WALL_S },
	{ _FLOOR_S +_WALL_S, -_FLOOR_S*2 -_WALL_S, -_WALL_S },
	{ _FLOOR_S +_WALL_S, _FLOOR_S*2  +_WALL_S, -_WALL_S },
	{ _FLOOR_S,		     _FLOOR_S*2  +_WALL_S, -_WALL_S },
	{ _FLOOR_S,		     -_FLOOR_S*2 -_WALL_S, _WALL_S  },
	{ _FLOOR_S +_WALL_S, -_FLOOR_S*2 -_WALL_S, _WALL_S  },
	{ _FLOOR_S +_WALL_S, _FLOOR_S*2  +_WALL_S, _WALL_S  },
	{ _FLOOR_S,		     _FLOOR_S*2  +_WALL_S, _WALL_S  },
};

/* 四角形 */
static GLint _QUADS_FACE[][4] = {			// 面(頂点の順番)
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
static FACE_NORMAL _QUADS_FACE_NOMAL = {	//面の法線ベクトル
	{ 0.0,  0.0,  -1.0 },
	{ 1.0,  0.0,  0.0  },
	{ 0.0,  0.0,  1.0  },
	{ -1.0, 0.0,  0.0  },
	{ 0.0,  -1.0, 0.0  },
	{ 0.0,  1.0,  0.0  }
};

/* 球 */
static GLfloat  _SPHERE_RADIUS  = 4.0;		//球の半径
static GLint    _SPHERE_NUMBER  = 10;		//球の個数
static GLdouble _FRICTION       = 0.995;	//摩擦による速度減少
static GLdouble _POS_ADJUSTMENT = 0.01;		//ベクトル位置反映用係数
static GLdouble _ADD_FORCE_DT   = 0.02;		//add force用係数
GLdouble		add_force       = 0.0;		//player ball 押し出し変数
static GLdouble _GRAVITY        = -10.0;	//重力
static GLdouble _COL_PUSH       = 0.5;		//コリジョン解除時の押し出す距離

struct {									//構造体：p 球体構造体
	GLdouble x, y, z;
	GLdouble vx, vy, vz;
	GLdouble fx, fy;
	bool     h_flag, d_flag;
}p[10];
struct {									//構造体：col_p 球体コリジョン演算用構造体
	GLdouble x, y, op_x, op_y;
}col_p[10];

std::vector<GLint> opponent_num;			//コリジョン

bool enable_play = true;					//プレイ可能

/* キュー */
static GLfloat _CYLINDER_RADIUS = 3.0f;
static GLfloat _CYLINDER_HEIGHT = 50.0f;
static GLint   _CYLINDER_SIDES  = 10;

/* 入力 */
bool space_key_pressing = false;			//スペースキー長押し検知
bool left_key_pressing  = false;			//LEFTキー長押し検知
bool right_key_pressing = false;			//RIGHTキー長押し検知

/* 視点 */
static GLdouble _LOOK_PLUS   = 0.03;
static GLdouble _LOOK_RADIUS = 300.0;
GLdouble look_x		= 0.0;
GLdouble look_y		= 0.0;
GLdouble look_theta = 0.0;
GLdouble look_angle = 0.0;

//----------------------------------------------------
// 関数プロトタイプ（後に呼び出す関数名と引数の宣言）
//----------------------------------------------------
/* 必要関数 */
void Initialize(void);
void Idle(void);
void Display(void);
void Resize(int w, int h);

/* 物体描画 */
void InitialSphere(void);
void DrawSphereOfPlayer(void);
void DrawSphereOfTarget(void);
void DrawTable(void);
void DrawWall(void);
void DrawHole(void);
void DrawFloor(void);
void DrawCylinder(void);

/* 演算 */
void SphereAddForce(GLint n);
bool CollisionSphere(GLint n);
void CollisionSphereProcess(GLint n, GLint op_n, GLdouble angle);
void NotCollisionPosition(GLint n, GLint op_n);
void CollisionWall(GLint n);
void CollisionHole(GLint n);
bool CollisionFloor(GLint n);

/* 影(ステンシル) */
void FindPlane(GLfloat plane[4], GLfloat v0[3], GLfloat v1[3], GLfloat v2[3]);
void ShadowMatrix(GLfloat *m, GLfloat plane[4], GLfloat light[4]);
void DrawShadow(void);

/* 視点回転 */
void Qmul(double r[], const double p[], const double q[]);
void Qrot(double r[], double q[]);

/* 入力 */
void Keyboard(unsigned char key, int x, int y);
void KeyboardUp(unsigned char key, int x, int y);
void SpecialFunc(int key, int x, int y);
void SpecialUpFunc(int key, int x, int y);
void MouseMotion(int x, int y);
void MouseOn(int button, int state, int x, int y);
void MouseWheel(float z);

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
	glutDisplayFunc(Display);										//描画時に呼び出される関数
	glutReshapeFunc(Resize);										//リサイズ時に呼び出される関数
	glutKeyboardFunc(Keyboard);										//キーボード入力時に呼び出される関数
	glutKeyboardUpFunc(KeyboardUp);									//キーボードが離された時に呼び出される関数
	glutSpecialFunc(SpecialFunc);									//Specialキーボード入力時に呼び出される関数
	glutSpecialUpFunc(SpecialUpFunc);								//Specialキーボードが離された時に呼び出される関数
	glutIgnoreKeyRepeat(GL_TRUE);									//キーの繰り返し入力は無視
	glutMouseFunc(MouseOn);											//マウスクリック時に呼び出される関数
	glutMotionFunc(MouseMotion);									//マウスドラッグ解除時に呼び出される関数

	glutIdleFunc(Idle);												//プログラムアイドル状態時に呼び出される関数
	Initialize();													//初期設定の関数を呼び出す
	glutMainLoop();													//イベント待ち受け状態(無限ループ)
	return 0;
}
//----------------------------------------------------
// 初期設定の関数
//----------------------------------------------------
void Initialize(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);	//背景色(RGBA 0.0~1.0)
	glEnable(GL_DEPTH_TEST);			//デプスバッファを使用：glutInitDisplayMode() で GLUT_DEPTH を指定する
	glDepthFunc(GL_LEQUAL);				//深度バッファと新しいピクセル地の深度の比較関数(GL_LEQUAL：格納深度以下であれば通過)
	glClearDepth(1.0);

	FindPlane(floor_planar,				//ステンシルを張るための床を見つける
		_FLOOR_VER.v0,
		_FLOOR_VER.v1,
		_FLOOR_VER.v2);

	/* 光源の設定 */
	glEnable(GL_LIGHTING);									//陰影ON
	glEnable(GL_LIGHT0);									//光源0を利用
	glLightfv(GL_LIGHT0, GL_POSITION, _LIGHT_POSITION_0);	//光源0の位置

	/* ディスプレイリストを作成 */
	list_number = glGenLists(1);
	glNewList(list_number, GL_COMPILE);	//コンパイルのみ
	glEndList();

	/* マウスポインタ位置のウィンドウ内の相対的位置への換算用 */
	sx = 1.0 / (double)512;
	sy = 1.0 / (double)512;

	/* 回転行列の初期化 */
	Qrot(rt, cq);

	/* ステンシルバッファクリア値の設定 */
	glClearStencil(0);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	/* 平面射影行列の算出 */
	ShadowMatrix(pM, floor_planar, _LIGHT_POSITION_0);

	/* 物体配置 */
	InitialSphere();

}

//----------------------------------------------------
// アイドル時に呼び出される関数
//----------------------------------------------------
void Idle() {
	glutPostRedisplay(); //glutDisplayFunc()を１回実行する

	if (space_key_pressing) {		//継続してaddforceを加える
		if (add_force <= 3500) {
			add_force += 15;
		}
	}

	if (left_key_pressing) {
		look_theta = look_theta - _LOOK_PLUS;
	}
	else if (right_key_pressing) {
		look_theta = look_theta + _LOOK_PLUS;
	}
	look_x = _LOOK_RADIUS * sin(look_theta);
	look_y = _LOOK_RADIUS * -cos(look_theta);
	
	look_angle = atan2(p[0].y - look_y, p[0].x - look_x);
	
	for (GLint i = 0; i < _SPHERE_NUMBER; i++) {
		if (p[i].vx == 0.0 && p[i].vy == 0.0) {
			enable_play = true;
		} else {
			enable_play = false;
			break;
		}
	}
}

//----------------------------------------------------
// リサイズ関数
//----------------------------------------------------
void Resize(int w, int h) {
	glViewport(0, 0, w, h);	//ビューポートの設定

	/* 透視変換行列の設定 */
	glMatrixMode(GL_PROJECTION);	//行列モードの設定GL_PROJECTION : 透視変換行列の設定、GL_MODELVIEW：モデルビュー変換行列）
	glLoadIdentity();																	//行列の初期化(変換行列に単位行列を設定)
	gluPerspective(30.0, (double)_WINDOW_WIDTH / (double)_WINDOW_HEIGHT, 0.1, 1000.0);	//透視投影法の視体積
	//gluPerspactive(画角, アスペクト比, 奥行き前, 奥行き後ろ);

	/* モデルビュー変換行列の設定 */
	glMatrixMode(GL_MODELVIEW);						//行列モードの設定（GL_PROJECTION : 透視変換行列の設定、GL_MODELVIEW：モデルビュー変換行列）
	glLoadIdentity();								//行列の初期化(変換行列に単位行列を設定)
}

//----------------------------------------------------
// 描画関数
//----------------------------------------------------
void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //バッファの消去

	glPushMatrix();

	/* 視点の設定 */
	gluLookAt(
		look_x, look_y, 200.0,	//視点の位置x,y,z;
		p[0].x, p[0].y, p[0].z,	//視界の中心位置の参照点座標x,y,z
		0.0, 0.0, 1.0);		//視界の上方向のベクトルx,y,z

	/* 回転 */
	glMultMatrixd(rt);		//任意の行列を積算する関数

	/* 図形 */
	DrawShadow();
	DrawSphereOfPlayer();		//自球
	DrawSphereOfTarget();		//他球
	DrawCylinder();				//キュー

	DrawWall();


	glPopMatrix();

	glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)でダブルバッファリングを利用可
}

//----------------------------------------------------
// 球体初期設定
//----------------------------------------------------
void InitialSphere(void) {
	/* Z of All Sphere */
	for (GLint i = 0; i < _SPHERE_NUMBER; i++) {
		p[i].z = _SPHERE_RADIUS;
	}

	/* Player Sphere */
	p[0].x = 0.0; p[0].y = -_FLOOR_S;

	/* Target Sphere */
	p[1].x = 0.0;						p[1].y = 10.0;
	p[2].x = -_SPHERE_RADIUS     - 1.0;	p[2].y = 10.0 + 2 * _SPHERE_RADIUS;
	p[3].x = _SPHERE_RADIUS		 + 1.0;	p[3].y = 10.0 + 2 * _SPHERE_RADIUS;
	p[4].x = -_SPHERE_RADIUS * 2 - 1.0;	p[4].y = 10.0 + 4 * _SPHERE_RADIUS;
	p[5].x = 0.0;						p[5].y = 10.0 + 4 * _SPHERE_RADIUS;
	p[6].x = _SPHERE_RADIUS  * 2 + 1.0;	p[6].y = 10.0 + 4 * _SPHERE_RADIUS;
	p[7].x = -_SPHERE_RADIUS     - 1.0;	p[7].y = 10.0 + 6 * _SPHERE_RADIUS;
	p[8].x = _SPHERE_RADIUS      + 1.0;	p[8].y = 10.0 + 6 * _SPHERE_RADIUS;
	p[9].x = 0.0;						p[9].y = 10.0 + 8 * _SPHERE_RADIUS;
};

//----------------------------------------------------
// Player球体の描画
//----------------------------------------------------
void DrawSphereOfPlayer(void) {
	GLint n = 0; //Player ball is p[0]
	if (p[n].z > -_WALL_S / 2) {				//球体が穴の中に消えていない
		if (CollisionFloor(n)) {				//球が床
			CollisionWall(n);					//壁の跳ね返り

			if (CollisionSphere(n)) {					//どれかの球と衝突している場合
				for (GLint i = 0, ilen = opponent_num.size(); i < ilen; i++) {
					GLint opponent = opponent_num[i];
					NotCollisionPosition(n, opponent);													//コリジョン解除
					GLdouble angle_n_op = atan2((p[opponent].y - p[n].y), (p[opponent].x - p[n].x));	//角度(n-op)
					GLdouble angle_op_n = atan2((p[n].y - p[opponent].y), (p[n].x - p[opponent].x));	//角度(op-n)
					CollisionSphereProcess(n, opponent, angle_n_op);									//衝突後のベクトル(n-op)
					CollisionSphereProcess(opponent, n, angle_op_n);									//衝突後のベクトル(op-n)
				}
				for (GLint i = 0, ilen = opponent_num.size(); i < ilen; i++) {
					GLint opponent = opponent_num[i];

					p[n].vx = col_p[n].x + col_p[opponent].op_x;										//自球xベクトル
					p[n].vy = col_p[n].y + col_p[opponent].op_y;										//自球yベクトル
					p[opponent].vx = col_p[n].op_x + col_p[opponent].x;									//相手球xベクトル
					p[opponent].vy = col_p[n].op_y + col_p[opponent].y;									//相手球yベクトル
				}
			}
		}
		else {									//球が穴内部
			CollisionHole(n);					//穴内部コリジョン
		}
		SphereAddForce(n);						//球体移動演算

		/* 描画 */
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT,   _MS_WHITE_PLASTIC.ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE,   _MS_WHITE_PLASTIC.diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR,  _MS_WHITE_PLASTIC.specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, &_MS_WHITE_PLASTIC.shininess);
		glTranslated(p[n].x, p[n].y, p[n].z);	    //平行移動値の設定
		glutSolidSphere(_SPHERE_RADIUS, 20, 20);	//引数：(半径, Z軸まわりの分割数, Z軸に沿った分割数)
		glPopMatrix();
	}
}
//----------------------------------------------------
// Target球体の描画
//----------------------------------------------------
void DrawSphereOfTarget(void) {
	for (GLint n = 1; n < _SPHERE_NUMBER; n++) {
		if (p[n].z > -_WALL_S / 2) {				//球体が穴の中に消えていない
			if (CollisionFloor(n)) {				//球が床
				CollisionWall(n);					//壁の跳ね返り

				if (CollisionSphere(n)) {					//どれかの球と衝突している場合
					for (GLint i = 0, ilen = opponent_num.size(); i < ilen; i++) {
						GLint opponent = opponent_num[i];
						NotCollisionPosition(n, opponent);													//コリジョン解除
						GLdouble angle_n_op = atan2((p[opponent].y - p[n].y), (p[opponent].x - p[n].x));	//角度(n-op)
						GLdouble angle_op_n = atan2((p[n].y - p[opponent].y), (p[n].x - p[opponent].x));	//角度(op-n)
						CollisionSphereProcess(n, opponent, angle_n_op);									//衝突後のベクトル(n-op)
						CollisionSphereProcess(opponent, n, angle_op_n);									//衝突後のベクトル(op-n)
					}
					for (GLint i = 0, ilen = opponent_num.size(); i < ilen; i++) {
						GLint opponent = opponent_num[i];

						p[n].vx = col_p[n].x + col_p[opponent].op_x;										//自球xベクトル
						p[n].vy = col_p[n].y + col_p[opponent].op_y;										//自球yベクトル
						p[opponent].vx = col_p[n].op_x + col_p[opponent].x;									//相手球xベクトル
						p[opponent].vy = col_p[n].op_y + col_p[opponent].y;									//相手球yベクトル
					}
				}
			}
			else {									//球が穴内部
				CollisionHole(n);					//穴内部コリジョン
			}
			SphereAddForce(n);						//球体移動演算

			glPushMatrix();
			glMaterialfv(GL_FRONT, GL_AMBIENT, _MS_RUBY.ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, _MS_RUBY.diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, _MS_RUBY.specular);
			glMaterialfv(GL_FRONT, GL_SHININESS, &_MS_RUBY.shininess);
			glTranslated(p[n].x, p[n].y, p[n].z);	//平行移動値の設定
			glutSolidSphere(_SPHERE_RADIUS, 20, 20);	//引数：(半径, Z軸まわりの分割数, Z軸に沿った分割数)
			glPopMatrix();
		} else {
			p[n].vx = p[n].vy = 0.0;
		}
	}
}

//----------------------------------------------------
// テーブルの描画
//----------------------------------------------------
void DrawTable(void) {
	glDisable(GL_LIGHTING);
	DrawHole();
	DrawFloor();
	glEnable(GL_LIGHTING);
}
//----------------------------------------------------
// 壁(直方体)の描画
//----------------------------------------------------
void DrawWall(void) {
	glDisable(GL_LIGHTING);
	glPushMatrix();
	glBegin(GL_QUADS);							//GL_QUADS：4点組の四角形
	glColor4fv(_RED);
	for (GLint j = 0; j < 6; ++j) {
		for (GLint i = 0; i < 4; ++i) {
			glVertex3dv(_WALL_TOP_VERTEX[_QUADS_FACE[j][i]]);
		}
	}
	glEnd();
	glBegin(GL_QUADS);							//GL_QUADS：4点組の四角形
	for (GLint j = 0; j < 6; ++j) {
		for (GLint i = 0; i < 4; ++i) {
			glVertex3dv(_WALL_BOTTOM_VERTEX[_QUADS_FACE[j][i]]);
		}
	}
	glEnd();
	glBegin(GL_QUADS);							//GL_QUADS：4点組の四角形
	for (GLint j = 0; j < 6; ++j) {
		for (GLint i = 0; i < 4; ++i) {
			glVertex3dv(_WALL_LEFT_VERTEX[_QUADS_FACE[j][i]]);
		}
	}
	glEnd();
	glBegin(GL_QUADS);							//GL_QUADS：4点組の四角形
	for (GLint j = 0; j < 6; ++j) {
		for (GLint i = 0; i < 4; ++i) {
			glVertex3dv(_WALL_RIGHT_VERTEX[_QUADS_FACE[j][i]]);
		}
	}
	glEnd();
	glPopMatrix();
	glEnable(GL_LIGHTING);
}
//----------------------------------------------------
// 穴(下床)の描画
//----------------------------------------------------
void DrawHole(void) {
	glPushMatrix();
	glColor4fv(_BLACK);
	glBegin(GL_QUADS);
	for (GLint j = 0; j < 6; ++j) {
		for (GLint i = 0; i < 4; ++i) {
			glVertex3dv(_HOLE_VERTEX[_QUADS_FACE[j][i]]);
		}
	}
	glEnd();
	glPopMatrix();
}
//----------------------------------------------------
// 床の描画
//----------------------------------------------------
void DrawFloor(void) {
	glPushMatrix();
	glColor4fv(_GREEN);
	glBegin(GL_QUADS);
	for (GLint j = 0; j < 3; ++j) {
		for (GLint i = 0; i < 4; ++i) {
			glVertex3dv(_FLOOR_VERTEX[_FLOOR_FACE[j][i]]);
		}
	}
	glEnd();
	glPopMatrix();
}

//----------------------------------------------------
// キューの描画
//----------------------------------------------------
GLdouble cylinder_pull = 0.0;
void DrawCylinder(void)
{
	glPushMatrix();
	glTranslated(p[0].x, p[0].y, 18.0);
	glRotated(look_angle * 180 / M_PI + 90, 0.0, 0.0, 1.0);
	if (space_key_pressing) {
		cylinder_pull += 0.1;
	} else {
		cylinder_pull = 0.0;
	}
	glTranslated(0, 52 + cylinder_pull, 0.0);
	glRotated(15, 1.0, 0.0, .0);
	/*上面 */
	if (enable_play) {
		glNormal3d(0.0, 1.0, 0.0);
		glBegin(GL_POLYGON);
		for (double i = _CYLINDER_SIDES; i >= 0; --i) {
			double t = M_PI * 2 / _CYLINDER_SIDES * (double)i;
			glVertex3d(_CYLINDER_RADIUS * cos(t), _CYLINDER_HEIGHT, _CYLINDER_RADIUS * sin(t));
		}
		glEnd();
		/* 側面 */
		glBegin(GL_QUAD_STRIP);
		for (double i = 0; i <= _CYLINDER_SIDES; i = i + 1) {
			double t = i * 2 * M_PI / _CYLINDER_SIDES;
			glNormal3f((GLfloat)cos(t), 0.0, (GLfloat)sin(t));
			glVertex3f((GLfloat)(_CYLINDER_RADIUS*cos(t)), -_CYLINDER_HEIGHT, (GLfloat)(_CYLINDER_RADIUS*sin(t)));
			glVertex3f((GLfloat)(_CYLINDER_RADIUS*cos(t)), _CYLINDER_HEIGHT, (GLfloat)(_CYLINDER_RADIUS*sin(t)));
		}
		glEnd();
		/* 下面 */
		glNormal3d(0.0, -1.0, 0.0);
		glBegin(GL_POLYGON);
		for (double i = 0; i < _CYLINDER_SIDES; i++) {
			double t = M_PI * 2 / _CYLINDER_SIDES * (double)i;
			glVertex3d(_CYLINDER_RADIUS * cos(t), -_CYLINDER_HEIGHT, _CYLINDER_RADIUS * sin(t));
		}
		glEnd();
		glCullFace(GL_BACK);
	}
	glPopMatrix();
}

//----------------------------------------------------
// 球体の移動演算
//----------------------------------------------------
void SphereAddForce(GLint n) {
	p[n].vx = p[n].vx * _FRICTION;
	p[n].vy = p[n].vy * _FRICTION;
	p[n].x += p[n].vx * _POS_ADJUSTMENT;
	p[n].y += p[n].vy * _POS_ADJUSTMENT;
	p[n].z += p[n].vz * _POS_ADJUSTMENT;
	if (abs(p[n].vy) < 0.01) {	//失速による停止
		p[n].vy = 0.0;
	}
	if (abs(p[n].vx) < 0.01) {	//失速による停止
		p[n].vx = 0.0;
	}
}

//----------------------------------------------------
// 球体同士の当たり判定
//----------------------------------------------------
bool CollisionSphere(GLint n) {
	bool collision_bool = false;
	std::vector<GLint> opponent_num_reset;
	opponent_num = opponent_num_reset;
	col_p[n].x = 0;
	col_p[n].y = 0;
	col_p[n].op_x = 0;
	col_p[n].op_y = 0;
	for (GLint i = n+1; i < _SPHERE_NUMBER; i++) {
		if ((p[n].x-p[i].x)*(p[n].x - p[i].x) + 
			(p[n].y-p[i].y)*(p[n].y - p[i].y) + 
				(p[n].z-p[i].z)*(p[n].z - p[i].z)
					<= (2*_SPHERE_RADIUS) * (2*_SPHERE_RADIUS)) {	//二つの球の距離が直径より小さい時
			col_p[i].x = 0;
			col_p[i].y = 0;
			col_p[i].op_x = 0;
			col_p[i].op_y = 0;
			opponent_num.push_back(i);
			collision_bool = true;
		}
	}

	return collision_bool;
}

//----------------------------------------------------
// 球体同士の当たり時の演算
//----------------------------------------------------
void CollisionSphereProcess(GLint n, GLint op_n, GLdouble angle) {			//n:自球, op_n:相手球, angle:球間の角度
	GLdouble diagonal = sqrt(p[n].vx*p[n].vx + p[n].vy*p[n].vy);	//対角線の長さ
	GLdouble direction_travel = atan2(p[n].vy, p[n].vx);			//進行方向
	GLdouble difference_angle = direction_travel - angle;			//角度の差
	GLdouble after_opv = abs(diagonal * cos(difference_angle));		//衝突後相手ベクトル
	GLdouble after_nv  = abs(diagonal * sin(difference_angle));		//衝突後自分ベクトル
	GLdouble after_opvx = after_opv * cos(angle);					//衝突後相手ベクトルx
	GLdouble after_opvy = after_opv * sin(angle);					//衝突後相手ベクトルy

	GLdouble after_nvx, after_nvy;									//衝突後自ベクトルx,衝突後自ベクトルy

	if (sin(difference_angle) < 0) {
		after_nvx = after_nv * cos(angle - M_PI / 2);
		after_nvy = after_nv * sin(angle - M_PI / 2);
	}else{
		after_nvx = after_nv * cos(angle + M_PI / 2);
		after_nvy = after_nv * sin(angle + M_PI / 2);
	}	

	col_p[n].x    = after_nvx;
	col_p[n].y    = after_nvy;
	col_p[n].op_x = after_opvx;
	col_p[n].op_y = after_opvy;
}

//----------------------------------------------------
// 球体同士の当たり時のコリジョン解除
//----------------------------------------------------
void NotCollisionPosition(GLint n, GLint op_n) {
	GLdouble new_pos = atan2((p[n].y - p[op_n].y), (p[n].x - p[op_n].x));
	p[n].x = _COL_PUSH*cos(new_pos) + p[n].x;
	p[n].y = _COL_PUSH*sin(new_pos) + p[n].y;
}

//----------------------------------------------------
// 球体と壁の当たり判定および演算
//----------------------------------------------------
void CollisionWall(GLint n) {
	if (abs(p[n].y)+1.0 > _FLOOR_S * 2 - _SPHERE_RADIUS) {	//上下壁
		p[n].vy = -p[n].vy;
	}
	if (abs(p[n].x)+1.0 > _FLOOR_S - _SPHERE_RADIUS) {		//左右壁
		p[n].vx = -p[n].vx;
	}
}

//----------------------------------------------------
// 球体と穴内部の当たり判定および計算
//----------------------------------------------------
void CollisionHole(GLint n) {
	//h_flag：穴内部に入り、一度壁に反射してから穴内部壁の反射を有効にする
	if (abs(p[n].y) > _FLOOR_S * 2 - _SPHERE_RADIUS) {										//テーブル上下壁
		double a = -(1.0) * p[n].vy;
		p[n].vy = 2 * a*(1.0) + p[n].vy;
		p[n].h_flag = true;
	}
	if ((abs(p[n].y) < _FLOOR_S * 2 + _SPHERE_RADIUS - _FLOOR_OFFSET) && p[n].h_flag) {		//穴内部上下壁
		double a = -(1.0) * p[n].vy;
		p[n].vy = 2 * a*(1.0) + p[n].vy;
	}
	if (abs(p[n].x) > _FLOOR_S - _SPHERE_RADIUS) {											//テーブル左右壁
		double a = -(1.0) * p[n].vx;
		p[n].vx = 2 * a*(1.0) + p[n].vx;
		p[n].h_flag = true;
	}
	if ((abs(p[n].x) < _FLOOR_S + _SPHERE_RADIUS - _FLOOR_OFFSET) && p[n].h_flag) {			//穴内部左右壁
		double a = -(1.0) * p[n].vx;
		p[n].vx = 2 * a*(1.0) + p[n].vx;
	}
}

//----------------------------------------------------
// 球体と床の当たり判定
//----------------------------------------------------
bool CollisionFloor(GLint n) {
	p[n].vz = _GRAVITY;
	if (p[n].z < _SPHERE_RADIUS+1 && p[n].z > 0) {				//床の高さより上(上下余裕あり)
		if (2 * _FLOOR_S - _SPHERE_RADIUS  > p[n].y &&
			-2 * _FLOOR_S + _SPHERE_RADIUS < p[n].y &&
			_FLOOR_S + _SPHERE_RADIUS/2 - _FLOOR_OFFSET    > p[n].x &&
			-_FLOOR_S - _SPHERE_RADIUS/2 + _FLOOR_OFFSET	  < p[n].x) {			//床上1(床を二つの長方形に見立てる)
			p[n].h_flag = false;												//穴内部ではない
			p[n].z = _SPHERE_RADIUS;											//球を床上適切な位置に調整
			p[n].vz = _GRAVITY - _GRAVITY;										//沈まないように重力の打ち消し
			return true;
		}
		else if (2 * _FLOOR_S + _SPHERE_RADIUS/2 - _FLOOR_OFFSET  > p[n].y &&
			-2 * _FLOOR_S - _SPHERE_RADIUS/2 + _FLOOR_OFFSET < p[n].y &&
			_FLOOR_S - _SPHERE_RADIUS  > p[n].x &&
			-_FLOOR_S + _SPHERE_RADIUS < p[n].x) {								//床上2(床を二つの長方形に見立てる)
			p[n].h_flag = false;
			p[n].z = _SPHERE_RADIUS;
			p[n].vz = _GRAVITY - _GRAVITY;
			return true;
		}
	}
	return false;	//床上でない = 穴
}


//----------------------------------------------------
// 床平面の方程式
//----------------------------------------------------
void FindPlane(
	GLfloat plane[4],	// 作成する平面方程式の係数
	GLfloat v0[3],		// 頂点１
	GLfloat v1[3],		// 頂点２
	GLfloat v2[3])		// 頂点３
{
	GLfloat vec0[3], vec1[3];

	// Need 2 vectors to find cross product.
	vec0[0] = v1[0] - v0[0];			//x：vector0 = 頂点2 - 頂点1
	vec0[1] = v1[1] - v0[1];			//y：vector0 = 頂点2 - 頂点1
	vec0[2] = v1[2] - v0[2];			//z：vector0 = 頂点2 - 頂点1

	vec1[0] = v2[0] - v0[0];			//x：vector1 = 頂点3 - 頂点1
	vec1[1] = v2[1] - v0[1];			//y：vector1 = 頂点3 - 頂点1
	vec1[2] = v2[2] - v0[2];			//z：vector1 = 頂点3 - 頂点1

	// find cross product to get A, B, and C of plane equation
	plane[0] =	 vec0[1] * vec1[2] - vec0[2] * vec1[1];		//  vector0.y * vector1.z - vector0.z * vector1.y
	plane[1] = -(vec0[0] * vec1[2] - vec0[2] * vec1[0]);	//-(vector0.x * vector1.z - vector0.z * vector1.x)
	plane[2] =   vec0[0] * vec1[1] - vec0[1] * vec1[0];		//  vector0.x * vector1.y - vector0.y * vector1.x

	plane[3] = -(plane[0] * v0[0] + plane[1] * v0[1] + plane[2] * v0[2]);	//ベクトル演算
}

//----------------------------------------------------
// 行列の計算
//----------------------------------------------------
void ShadowMatrix(
	GLfloat *m,			// 作成する行列のポインタ
	GLfloat plane[4],	// 射影する表面の平面方程式の係数
	GLfloat light[4])	// 光源の同時座標値
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
// 影の描画
//----------------------------------------------------
void DrawShadow(void) {
	/* 床のステンシルを付ける */
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, ~0);

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);	//これから描画するもののステンシル値にすべて１タグをつける
	//(GL_REPLACE：flStencilFunc()第二引数に置き換え)
	DrawTable();							//床の描画

	/* カラー・デプスバッファマスクをセットする
	 * これで以下の内容のピクセルの色の値は、書き込まれない。*/
	glColorMask(0, 0, 0, 0);
	glDepthMask(0);

	/* 床にオブジェクトの影のステンシルを付ける */
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, 1, ~0);

	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);		//これから描画するもののステンシル値にすべて１タグをつける
	//(GL_INCR：ステンシル値を+1 < max)
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	glMultMatrixf(pM);							//現在の行列にpM(shadowMatrix)を掛ける
	DrawSphereOfPlayer();
	DrawSphereOfTarget();
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);

	/* ビットマスクを解除 */
	glColorMask(1, 1, 1, 1);
	glDepthMask(1);

	/* 影をつける */
	glStencilFunc(GL_EQUAL, 2, ~0);
	glEnable(GL_BLEND);									//混合処理
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//アルファブレンド
	glColor4f(0.1f, 0.1f, 0.1f, 0.5f);
	glDisable(GL_DEPTH_TEST);
	DrawTable();									//床の描画
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_STENCIL_TEST);
}

//----------------------------------------------------
// キーボード入力時に呼び出される関数
//----------------------------------------------------
void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '\040':	//SP
		space_key_pressing = true;
		break;
	case '\033':	//ESCのASCIIコード
		exit(0);
		break;
	default:
		break;
	}
}

//----------------------------------------------------
// キーボードが離された時に呼び出される関数
//----------------------------------------------------
void KeyboardUp(unsigned char key, int x, int y) {
	switch (key) {
	case '\040':	//SP
		space_key_pressing = false;
		GLdouble force_x = (p[0].x - look_x) * _ADD_FORCE_DT;
		GLdouble force_y = (p[0].y - look_y) * _ADD_FORCE_DT;
		p[0].vx += force_x * add_force * _ADD_FORCE_DT;
		p[0].vy += force_y * add_force * _ADD_FORCE_DT;
		add_force = 0.0;
		break;
	}
}

//----------------------------------------------------
// Specialキーボード入力時に呼び出される関数
//----------------------------------------------------
void SpecialFunc(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:	//←
		left_key_pressing = true;
		break;
	case GLUT_KEY_RIGHT: //→
		right_key_pressing = true;
		break;	
	}
}

//----------------------------------------------------
// Specialキーボードが離された時に呼び出される関数
//----------------------------------------------------
void SpecialUpFunc(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:	//←
		left_key_pressing = false;
		break;
	case GLUT_KEY_RIGHT: //→
		right_key_pressing = false;
		break;
	}
}

//----------------------------------------------------
// マウスドラッグ時
//----------------------------------------------------
void MouseMotion(int x, int y) {
	double dx, dy, a;

	/* マウスポインタの位置のドラッグ開始位置からの変位 */
	dx = (x - cx) * sx;
	dy = (y - cy) * sy;

	/* マウスポインタの位置のドラッグ開始位置からの距離 */
	a = sqrt(dx * dx + dy * dy);

	if (a != 0.0)
	{
		/* マウスのドラッグに伴う回転のクォータニオン dq を求める */
		double ar = a * SCALE * 0.5;
		double as = sin(ar) / a;
		double dq[4] = { cos(ar), dy * as, dx * as, 0.0 };

		/* 回転の初期値 cq に dq を掛けて回転を合成 */
		Qmul(tq, dq, cq);

		/* クォータニオンから回転の変換行列を求める */
		Qrot(rt, tq);
	}
}
//----------------------------------------------------
// マウスクリック時
//----------------------------------------------------
void MouseOn(int button, int state, int x, int y)
{
	switch (button) {
	case 0:
		switch (state) {
		case 0:
			// ドラッグ開始点を記録
			cx = x;
			cy = y;
			break;
		case 1:
			// 回転の保存
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
}

//----------------------------------------------------
// マウスドラッグによる回転
//----------------------------------------------------
// クォータニオンの積 r <- p x q
static void Qmul(double r[], const double p[], const double q[])
{
	r[0] = p[0] * q[0] - p[1] * q[1] - p[2] * q[2] - p[3] * q[3];
	r[1] = p[0] * q[1] + p[1] * q[0] + p[2] * q[3] - p[3] * q[2];
	r[2] = p[0] * q[2] - p[1] * q[3] + p[2] * q[0] + p[3] * q[1];
	r[3] = p[0] * q[3] + p[1] * q[2] - p[2] * q[1] + p[3] * q[0];
}

// 回転の変換行列 r <- クォータニオン q
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