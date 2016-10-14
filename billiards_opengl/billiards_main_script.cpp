#include <math.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <direct.h>
#include <time.h>
#include <GL/glut.h>
using namespace std;

//----------------------------------------------------
// 変数宣言
//----------------------------------------------------
/* ウィンドウ変数 */
int  WINDOW_POSITION_X = 100; 
int  WINDOW_POSITION_Y = 100;
int  WINDOW_WIDTH = 512;
int  WINDOW_HEIGHT = 512; 
char WINDOW_TITLE[] = "billiards";

/* マウス回転 */
#define SCALE (2.0 * 3.14159265358979323846)	// マウスの相対位置→回転角の換算係数
int cx, cy;										// ドラッグ開始位置
double sx, sy;									// マウスの絶対位置→ウィンドウ内での相対位置の換算係数
double cq[4] = { 1.0, 0.0, 0.0, 0.0 };			// 回転の初期値 (クォータニオン)
double tq[4];									// ドラッグ中の回転 (クォータニオン)
double rt[16];									// 回転の変換行列
float camera_z_pos = 50.0;

/* ディスプレイリスト */
unsigned int listNumber;


/* 直方体 */
GLdouble vertex[][3] = {	// 頂点
	{ 0.0, 0.0, 0.0 },
	{ 2.0, 0.0, 0.0 },
	{ 2.0, 2.0, 0.0 },
	{ 0.0, 2.0, 0.0 },
	{ 0.0, 0.0, 30.0 },
	{ 2.0, 0.0, 30.0 },
	{ 2.0, 2.0, 30.0 },
	{ 0.0, 2.0, 30.0 }
};
int face[][4] = {			// 面(頂点の順番)
	{ 0, 1, 2, 3 },
	{ 1, 5, 6, 2 },
	{ 5, 4, 7, 6 },
	{ 4, 0, 3, 7 },
	{ 4, 5, 1, 0 },
	{ 3, 2, 6, 7 }
};
GLdouble normal[][3] = {	// 面の法線ベクトル
	{ 0.0, 0.0,-1.0 },
	{ 1.0, 0.0, 0.0 },
	{ 0.0, 0.0, 1.0 },
	{ -1.0, 0.0, 0.0 },
	{ 0.0,-1.0, 0.0 },
	{ 0.0, 1.0, 0.0 }
};

/* 物質質感(山本昌志氏の構造体を利用) */
struct MaterialStruct {							// material用構造体
	GLfloat ambient[4];		// 環境反射
	GLfloat diffuse[4];		// 拡散反射
	GLfloat specular[4];	// 鏡面反射
	GLfloat shininess;		// 光沢
};
MaterialStruct ms_jade = {						// jade(翡翠)
	{ 0.135,     0.2225,   0.1575,   1.0 },
	{ 0.54,      0.89,     0.63,     1.0 },
	{ 0.316228,  0.316228, 0.316228, 1.0 },
	12.8 };
MaterialStruct ms_ruby = {						// ruby(ルビー)
	{ 0.1745,   0.01175,  0.01175,   1.0 },
	{ 0.61424,  0.04136,  0.04136,   1.0 },
	{ 0.727811, 0.626959, 0.626959,  1.0 },
	76.8 };

/* 色 */
GLfloat red[]     = { 0.8, 0.2, 0.2, 1.0 };	//赤色
GLfloat green[]   = { 0.2, 0.8, 0.2, 1.0 };	//緑色
GLfloat blue[]    = { 0.2, 0.2, 0.8, 1.0 };	//青色
GLfloat yellow[]  = { 0.8, 0.8, 0.2, 1.0 };	//黄色
GLfloat white[]   = { 1.0, 1.0, 1.0, 1.0 };	//白色
GLfloat shininess = 30.0;					//光沢の強さ

/* 光 */
GLfloat LIGHT_POSITION_0[] = { 0.0, 50.0, 50.0, 1.0 }; //光源0の座標

static GLfloat floor_planar[4];
static GLfloat floor_s = 50.0f;
static GLfloat pM[16];
static GLfloat lightpos[4] = { -30, -100, 50, 1 };

typedef struct _QUADS_VERTEX {
	GLfloat v0[3];
	GLfloat v1[3];
	GLfloat v2[3];
	GLfloat v3[3];
}QUADS_VERTEX;
static QUADS_VERTEX floor_v = {
	{ floor_s,  floor_s, -1.0f },
	{ -floor_s,  floor_s, -1.0f },
	{ -floor_s, -floor_s, -1.0f },
	{ floor_s, -floor_s, -1.0f },
};

struct {
	double x, y, z;
	double vx, vy, vz;
}p[100];
int pn = 0;
double ax = 0.0, ay = 0.0, az = -4.0;
double vx = 5.0, vy = 5.0, vz = 20.0;
double hanpatu = 0.9;

int tn = 0;
double dt = 0.05;

//----------------------------------------------------
// 関数プロトタイプ（後に呼び出す関数名と引数の宣言）
//----------------------------------------------------
void Initialize(void);
void Idle(void);
void Display(void);
void Resize(int w, int h);
void Ground(void);

void Sphere(void);
void Cube(void);
void Rectangular(void);

void findPlane(GLfloat plane[4], GLfloat v0[3], GLfloat v1[3], GLfloat v2[3]);
void shadowMatrix(GLfloat *m, GLfloat plane[4], GLfloat light[4]);
void DrawFloor(bool bTexture);
void DrawShadow(void);
void DrawStructure(bool);

void Qmul(double r[], const double p[], const double q[]);
void Qrot(double r[], double q[]);

void Keyboard(unsigned char key, int x, int y);
void MouseMotion(int x, int y);
void MouseOn(int button, int state, int x, int y);
void MouseWheel(float z);

//----------------------------------------------------
// メイン関数
//----------------------------------------------------
int main(int argc, char *argv[]) {
	srand((unsigned)time(NULL));									//乱数の発生
	glutInit(&argc, argv);											//環境の初期化
	glutInitWindowPosition(WINDOW_POSITION_X, WINDOW_POSITION_Y);	//ウィンドウの位置の指定
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);				//ウィンドウサイズの指定
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);		//ディスプレイモードの指定(RGBA,デプスバッファ,ダブルバッファ)
	glutCreateWindow(WINDOW_TITLE);									//ウィンドウの作成
	glutDisplayFunc(Display);										//描画時に呼び出される関数
	glutReshapeFunc(Resize);										//リサイズ時に呼び出される関数
	glutKeyboardFunc(Keyboard);										//キーボード入力時に呼び出される関数
	//glutMouseFunc(MouseOn);											//マウスクリック時に呼び出される関数
	//glutMotionFunc(MouseMotion);									//マウスドラッグ解除時に呼び出される関数

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

	findPlane(floor_planar,
		floor_v.v0,
		floor_v.v1,
		floor_v.v2);

	/* 光源の設定 */
	glEnable(GL_LIGHTING);									//陰影ON
	glEnable(GL_LIGHT0);									//光源0を利用
	glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_POSITION_0);	//光源0の位置

	/* ディスプレイリストを作成 */
	listNumber = glGenLists(1);
	glNewList(listNumber, GL_COMPILE);	//コンパイルのみ
	glEndList();

	/* マウスポインタ位置のウィンドウ内の相対的位置への換算用 */
	sx = 1.0 / (double)512;
	sy = 1.0 / (double)512;

	/* 回転行列の初期化 */
	Qrot(rt, cq);

	//ステンシルバッファクリア値の設定--------------------------
	glClearStencil(0);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	// 平面射影行列の算出--------------------------
	shadowMatrix(pM, floor_planar, lightpos);
	//--------------------------

}

//----------------------------------------------------
// アイドル時に呼び出される関数
//----------------------------------------------------
void Idle() {
	glutPostRedisplay(); //glutDisplayFunc()を１回実行する
}

//----------------------------------------------------
// リサイズ関数
//----------------------------------------------------
void Resize(int w, int h) {
	glViewport(0, 0, w, h);	//ビューポートの設定

	/* 透視変換行列の設定 */
	glMatrixMode(GL_PROJECTION);	//行列モードの設定GL_PROJECTION : 透視変換行列の設定、GL_MODELVIEW：モデルビュー変換行列）
	glLoadIdentity();																	//行列の初期化(変換行列に単位行列を設定)
	gluPerspective(30.0, (double)WINDOW_WIDTH / (double)WINDOW_HEIGHT, 0.1, 1000.0);	//透視投影法の視体積
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
		0.0, -160.0, 40.0,	//視点の位置x,y,z;
		0.0, 0.0, 0.0,	//視界の中心位置の参照点座標x,y,z
		0.0, 0.0, 1.0);		//視界の上方向のベクトルx,y,z

	/* 回転 */
	glMultMatrixd(rt);		//任意の行列を積算する関数

	/* 図形の描画 */
	Sphere();		//球
	Cube();			//立方体
	Rectangular();	//直方体
	Ground();		//大地

	DrawStructure(false);
	DrawShadow();
	glPopMatrix();

	glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)でダブルバッファリングを利用可
}
//----------------------------------------------------
// 物体の描画
//----------------------------------------------------
void DrawStructure(bool flag) {
	for (int i = 1; i <= pn; i++) {
		p[i].vx += ax * dt;
		p[i].vy += ay * dt;
		p[i].vz += az * dt;
		p[i].x += p[i].vx * dt;
		p[i].y += p[i].vy * dt;
		p[i].z += p[i].vz * dt;
		if (p[i].z < 4 && abs(p[i].x) < floor_s  && abs(p[i].y) < floor_s) {
			p[i].z = 4.0;
			p[i].vz = -hanpatu * p[i].vz;
		}
		if (!flag || p[i].z >0) {
			glPushMatrix();
			glMaterialfv(GL_FRONT, GL_AMBIENT, ms_ruby.ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, ms_ruby.diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, ms_ruby.specular);
			glMaterialfv(GL_FRONT, GL_SHININESS, &ms_ruby.shininess);
			glTranslated(p[i].x, p[i].y, p[i].z);//平行移動値の設定
			glutSolidSphere(4.0, 20, 20);//引数：(半径, Z軸まわりの分割数, Z軸に沿った分割数)
			glPopMatrix();
		}
	}
}
//----------------------------------------------------
// 球
//----------------------------------------------------
void Sphere(void) {
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, ms_ruby.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ms_ruby.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ms_ruby.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, &ms_ruby.shininess);
	glTranslated(0.0, 10.0, 20.0);	//平行移動値の設定
	glutSolidSphere(4.0, 20, 20);	//引数：(半径, Z軸まわりの分割数, Z軸に沿った分割数)
	glPopMatrix();
}
//----------------------------------------------------
// 立方体
//----------------------------------------------------
void Cube(void) {
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
	glTranslated(-20.0, 0.0, 20.0);	//平行移動値の設定
	glutSolidCube(10.0);			//引数：(一辺の長さ)
	glPopMatrix();
}

//----------------------------------------------------
// 直方体
//----------------------------------------------------
void Rectangular(void) {
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, ms_jade.ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ms_jade.diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ms_jade.specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, &ms_jade.shininess);
	glTranslated(30.0, 50.0, 0.0);	//平行移動値の設定
	glBegin(GL_QUADS);							//GL_QUADS：4点組の四角形
	for (int j = 0; j < 6; ++j) {
		glNormal3dv(normal[j]);					//法線ベクトルの指定
		for (int i = 0; i < 4; ++i) {
			glVertex3dv(vertex[face[j][i]]);
		}
	}
	glEnd();
	glPopMatrix();
}
//----------------------------------------------------
// 大地の描画
//----------------------------------------------------
void Ground(void) {
	double ground_max_x = 300.0;
	double ground_max_y = 300.0;
	glColor3d(0.8, 0.8, 0.8);  // 大地の色
	glBegin(GL_LINES);
	for (double ly = -ground_max_y; ly <= ground_max_y; ly += 10.0) {
		glVertex3d(-ground_max_x, ly, 0);
		glVertex3d(ground_max_x, ly, 0);
	}
	for (double lx = -ground_max_x; lx <= ground_max_x; lx += 10.0) {
		glVertex3d(lx, ground_max_y, 0);
		glVertex3d(lx, -ground_max_y, 0);
	}
	glEnd();
}
//----------------------------------------------------
// 床平面の方程式と行列の計算
//----------------------------------------------------
void findPlane(
	GLfloat plane[4],  // 作成する平面方程式の係数
	GLfloat v0[3],    // 頂点１
	GLfloat v1[3],    // 頂点２
	GLfloat v2[3])    // 頂点３
{
	GLfloat vec0[3], vec1[3];

	// Need 2 vectors to find cross product.
	vec0[0] = v1[0] - v0[0];
	vec0[1] = v1[1] - v0[1];
	vec0[2] = v1[2] - v0[2];

	vec1[0] = v2[0] - v0[0];
	vec1[1] = v2[1] - v0[1];
	vec1[2] = v2[2] - v0[2];

	// find cross product to get A, B, and C of plane equation
	plane[0] = vec0[1] * vec1[2] - vec0[2] * vec1[1];
	plane[1] = -(vec0[0] * vec1[2] - vec0[2] * vec1[0]);
	plane[2] = vec0[0] * vec1[1] - vec0[1] * vec1[0];

	plane[3] = -(plane[0] * v0[0] + plane[1] * v0[1] + plane[2] * v0[2]);
}
void shadowMatrix(
	GLfloat *m,      // 作成する行列のポインタ
	GLfloat plane[4],  // 射影する表面の平面方程式の係数
	GLfloat light[4])  // 光源の同時座標値
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
// 床の描画と影の描画
//----------------------------------------------------
void DrawFloor(bool bTexture) {
	if (bTexture) {
		// 床にテクスチャを使う時はココで設定する
		//  glBindTexture( GL_TEXTURE_2D, );

		glDisable(GL_LIGHTING);
		glBegin(GL_QUADS);
		//    glTexCoord2f( , );
		glVertex3fv(floor_v.v0);
		//    glTexCoord2f( , );
		glVertex3fv(floor_v.v1);
		//    glTexCoord2f( , );
		glVertex3fv(floor_v.v2);
		//    glTexCoord2f( , );
		glVertex3fv(floor_v.v3);
		glEnd();
		glEnable(GL_LIGHTING);
	}
	else {
		glDisable(GL_LIGHTING);
		glBegin(GL_QUADS);
		glVertex3fv(floor_v.v0);
		glVertex3fv(floor_v.v1);
		glVertex3fv(floor_v.v2);
		glVertex3fv(floor_v.v3);
		glEnd();
		glEnable(GL_LIGHTING);
	}
}
void DrawShadow(void) {
	/////////////////////////////////////////////
	//床のステンシルを付ける
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, ~0);
	//これから描画するもののステンシル値にすべて１タグをつける
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glColor4f(0.7f, 0.4f, 0.0f, 1.0f);
	DrawFloor(true);//床の描画

					/////////////////////////////////////////////
					//カラー・デプスバッファマスクをセットする
					//これで以下の内容のピクセルの色の値は、書き込まれない。
	glColorMask(0, 0, 0, 0);
	glDepthMask(0);
	/////////////////////////////////////////////
	//床にオブジェクトの影のステンシルを付ける
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_EQUAL, 1, ~0);
	//これから描画するもののステンシル値にすべて１タグをつける
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	glDisable(GL_DEPTH_TEST);
	glPushMatrix();
	glMultMatrixf(pM);
	DrawStructure(true);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);

	/////////////////////////////////////////////
	//ビットマスクを解除
	glColorMask(1, 1, 1, 1);
	glDepthMask(1);

	/////////////////////////////////////////////
	//影をつける
	glStencilFunc(GL_EQUAL, 2, ~0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.1f, 0.1f, 0.1f, 0.5f);
	glDisable(GL_DEPTH_TEST);
	DrawFloor(false);//床の描画
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_STENCIL_TEST);
}

//----------------------------------------------------
// キーボード入力時に呼び出される関数
//----------------------------------------------------
void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
		pn++;
		p[pn].x = -0.0;
		p[pn].y = -10.0;
		p[pn].z = 5.0;
		p[pn].vx = vx * ((double)rand() / (double)RAND_MAX - (double)rand() / (double)RAND_MAX);
		p[pn].vy = vy * ((double)rand() / (double)RAND_MAX - (double)rand() / (double)RAND_MAX);
		p[pn].vz = vz * ((double)rand() / (double)RAND_MAX);
		break;
	case '\033':	//ESCのASCIIコード
		exit(0);
		break;
	default:
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
	cout << x << " " << y << endl;
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