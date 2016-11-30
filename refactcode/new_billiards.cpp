
//----------------------------------------------------
// 変数宣言
//----------------------------------------------------

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

static GLdouble _ADD_FORCE_DT   = 0.02;		//add force用係数
static GLdouble _GRAVITY        = -10.0;	//重力
static GLdouble _COL_PUSH       = 0.5;		//コリジョン解除時の押し出す距離

struct {									//構造体：col_p 球体コリジョン演算用構造体
	GLdouble x, y, op_x, op_y;
}col_p[10];

std::vector<GLint> opponent_num;			//コリジョン

bool enable_play = true;					//プレイ可能

/* キュー */
static GLfloat _CYLINDER_RADIUS = 3.0f;
static GLfloat _CYLINDER_HEIGHT = 50.0f;
static GLint   _CYLINDER_SIDES  = 10;

/* 視点 */
static GLdouble _LOOK_PLUS   = 0.03;
static GLdouble _LOOK_RADIUS = 300.0;
GLdouble look_x		= 0.0;
GLdouble look_y		= 0.0;
GLdouble look_theta = 0.0;
GLdouble look_angle = 0.0;

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
