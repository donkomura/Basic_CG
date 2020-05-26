#define FREEGLUT_STATIC

#include <random>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <GL/glut.h>

using namespace std;

// 3次元ベクトルを扱うためのクラス
class Vector3d {
public:
	double x, y, z;
	Vector3d() { x = y = z = 0; }
	Vector3d(double _x, double _y, double _z) { x = _x; y = _y; z = _z; }
	void set(double _x, double _y, double _z) { x = _x; y = _y; z = _z; }

	// 長さを1に正規化する
	void normalize() {
		double len = length();
		x /= len; y /= len; z /= len;
	}

	// 長さを返す
	double length() { return sqrt(x * x + y * y + z * z); }

	// s倍する
	void scale(const double s) { x *= s; y *= s; z *= s; }

	// 加算の定義
	Vector3d operator+(Vector3d v) { return Vector3d(x + v.x, y + v.y, z + v.z); }

	// 減算の定義
	Vector3d operator-(Vector3d v) { return Vector3d(x - v.x, y - v.y, z - v.z); }

	// 内積の定義
	double operator*(Vector3d v) { return x * v.x + y* v.y + z * v.z; }

	// 外積の定義
	Vector3d operator%(Vector3d v) { return Vector3d(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }
	
	// 代入演算の定義
	Vector3d& operator=(const Vector3d& v){ x = v.x; y = v.y; z = v.z; return (*this); }

	// 加算代入の定義
	Vector3d& operator+=(const Vector3d& v) { x += v.x; y += v.y; z += v.z; return (*this); }

	// 減算代入の定義
	Vector3d& operator-=(const Vector3d& v) { x -= v.x; y -= v.y; z -= v.z; return (*this); }

	// 値を出力する
	void print() { printf("Vector3d(%f %f %f)\n", x, y, z); }
};

// マイナスの符号の付いたベクトルを扱えるようにするための定義 例：b=(-a); のように記述できる
Vector3d operator-( const Vector3d& v ) { return( Vector3d( -v.x, -v.y, -v.z ) ); }

// ベクトルと実数の積を扱えるようにするための定義 例： c=5*a+2*b; c=b*3; のように記述できる
Vector3d operator*( const double& k, const Vector3d& v ) { return( Vector3d( k*v.x, k*v.y, k*v.z ) );}
Vector3d operator*( const Vector3d& v, const double& k ) { return( Vector3d( v.x*k, v.y*k, v.z*k ) );}

// ベクトルを実数で割る操作を扱えるようにするための定義 例： c=a/2.3; のように記述できる
Vector3d operator/( const Vector3d& v, const double& k ) { return( Vector3d( v.x/k, v.y/k, v.z/k ) );}


// 球体の情報を格納するクラス
class Sphere {
public:
	Vector3d position; // 中心位置
	Vector3d vec;      // 速度
	float color[3];    // 描画色

	void setColor(float r, float g, float b) {
		color[0] = r; color[1] = g; color[2] = b;
	}

	// この球体を描画するメンバ関数
	void display() { 
		glPushMatrix(); // 現在のモデル変換行列を退避しておく

		// 座標の平行移動とスケール変換を施して球体を描画する
		glTranslated(position.x, position.y, position.z);
		glScaled(1, 1, 1);
		glutSolidSphere(1.0, 32, 32);
		
		glPopMatrix();  // 退避していたモデル変換行列を戻す
	}
};

// ゲームのポイント
int g_GamePoint = 0;

const int g_AnimationIntervalMsec = 600;

// 3つの球体を準備しておく
#define NUM_OBJECTS 9
Sphere g_Sphere[NUM_OBJECTS];

// 視点
double g_EyeX, g_EyeY, g_EyeZ;

// 選択状態にある球体のID番号（0,1,2）を保持する。選択状態の球が無ければ-1とする。
int g_SelectedSphereID = -1;

// クリックされた3次元座標を保持する
Vector3d g_SelectedPos;

// ウィンドウサイズを保持する
int g_WindowWidth = 512;
int g_WindowHeight = 512;

// 選択した球体のID番号（0,1,2）を返す
// 選択した球体が無い場合は -1 を返す
int pickSphere(int x, int y) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// 照明効果を無くして単色で描画する
	glDisable(GL_LIGHTING);

	// 3つの球体を描画する
	for (int i = 0; i < NUM_OBJECTS; i++) {
		// RGBのR成分に球体のIDを設定する(unsigned byte型)
		glColor3ub(i, 0, 0);
		g_Sphere[i].display();
	}

	// ★授業スライドを参考に次のようなプログラムコードを追加する
	// ★glReadPixels 関数をつかって、クリックした位置が何色であるか取得する
	// ★取得した色を見て、どの球体を選択したか判定し、そのIDを return する。
	GLubyte c[3];
	glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, c);
	return (c[0] == 255 ? -1 : (int)c[0]);
}

// 描画関数
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	// 透視投影変換行列の設定
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, g_WindowWidth / (float)g_WindowHeight, 1.0, 100.0);

	// カメラビュー座標への変換行列の設定
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(g_EyeX, g_EyeY, g_EyeZ, 0, 0, 0, 0, 1, 0);

	// 3つの球体を描画
	for (int i = 0; i < NUM_OBJECTS; i++) {
		// 球体ごとに色を変更する
		glMaterialfv(GL_FRONT, GL_DIFFUSE, g_Sphere[i].color);

		// 球体の描画を行う
		g_Sphere[i].display();
	}

	// 球が選択されている状態であれば、クリック座標に関する情報を表示する
	if(g_SelectedSphereID != -1) {
		// 照明効果なしで単色描画
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);

		// クリック座標に点を描画
		glColor3f(1, 0, 0);
		glPointSize(5.f);
		glBegin(GL_POINTS);
		glVertex3d(g_SelectedPos.x, g_SelectedPos.y, g_SelectedPos.z);
		glEnd();
	}

	// ポイントの表示
	double M[16];  // モデルビュー行列
	double P[16];  // 凍死投影行列
	int V[4];      // ビューポート
	glGetDoublev(GL_MODELVIEW_MATRIX, M);
	glGetDoublev(GL_PROJECTION_MATRIX, P);
	glGetIntegerv(GL_VIEWPORT, V);

	double gx, gy, gz;
	gluUnProject(0, 1.5, 0, M, P, V, &gx, &gy, &gz);
    glRasterPos3d(gx, gy, gz);
	glColor3f(0, 0, 0);
	char point[64];
	sprintf_s(point, "Point: %d", g_GamePoint);
	for (unsigned i = 0; point[i] != '\0'; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, point[i]);
	}

	glutSwapBuffers();
}

// ウィンドウのサイズが変更されたときの処理
void resize(int w, int h) {
	if (h < 1) return;

	glViewport(0, 0, w, h);

	g_WindowWidth = w;
	g_WindowHeight = h;
}

// マウスカーソル位置に基づく選択処理
void MousePick(int x, int _y) {

	printf("MousePick(%d, %d)\n", x, _y);

	// マウスクリックで得られる座標は左下原点なので OpenGLの座標系と合わせるためにy座標を反転する
	const int y = g_WindowHeight - _y;

	g_SelectedSphereID = pickSphere(x, y);

	// 球が選択されていないなら何もしない
	if (g_SelectedSphereID == -1) return;

	// 赤だったらポイント追加
	if (g_SelectedSphereID < 3) {
		g_GamePoint++;
	}
	else {  // それ以外を選んだらマイナスポイント
		g_GamePoint--;
	}

	// クリックした場所の座標値（3次元座標）を取得する

	// ★授業スライドを参考に次のようなプログラムコードを追加する
	// ★現在のモデルビュー行列を取得する
	// ★現在の透視投影行列の取得を取得する
	// ★現在のビューポートの情報を取得
	// ★マウスクリックした位置の奥行き情報（z値）を取得する
	// ★上記の情報に基づいて、クリックした位置のワールド座標を取得する
	// ★取得した値は、g_SelectedPos に格納しておく（←表示の時に使用する）
	double M[16];  // モデルビュー行列
	double P[16];  // 凍死投影行列
	int V[4];      // ビューポート
	glGetDoublev(GL_MODELVIEW_MATRIX, M);
	glGetDoublev(GL_PROJECTION_MATRIX, P);
	glGetIntegerv(GL_VIEWPORT, V);

	float z;
	glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);

	double ox, oy, oz;
	gluUnProject(x, y, z, M, P, V, &ox, &oy, &oz);

	g_SelectedPos = Vector3d(ox, oy, oz);
}

// マウスクリックのイベント処理
void mouse(int button, int state, int x, int y) {
	if (state == GLUT_DOWN) MousePick(x, y); 
	glutPostRedisplay();
}

// マウスドラッグのイベント処理
void motion(int x, int y) {
	MousePick(x, y);
	glutPostRedisplay();
}

// キーが押されたときのイベント処理
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':
	case 'Q':
	case '\033':
		exit(0);  /* '\033' は ESC の ASCII コード */
	case 'w':
	case 'W':
		g_EyeY++;
		break;
	case 's':
	case 'S':
		g_EyeY--;
		break;
	case 'a':
	case 'A':
		g_EyeX--;
		break;
	case 'd':
	case 'D':
		g_EyeX++;
		break;
	default:
		break;
	}

	gluLookAt(g_EyeX, g_EyeY, g_EyeZ, 0, 0, 0, 0, 1, 0);
	glutPostRedisplay();
}

void timer(int val) {
	// 球体の移動
	for (int i = 0; i < NUM_OBJECTS; i++) {
		g_Sphere[i].position += g_Sphere[i].vec;
	}

	glutPostRedisplay();
    glutTimerFunc(g_AnimationIntervalMsec, timer, val);
}


void init() {
	// 視点
	g_EyeZ = 30.0;

	// 球体の速度を乱数で決定
	random_device rnd;
	mt19937 mt(rnd());
	uniform_int_distribution<> rand(-1, 1);
	for (int i = 0; i < NUM_OBJECTS; i++) {
		if (i < 3) {      // 赤
			g_Sphere[i].position.set( -5, 0, 0);
			g_Sphere[i].setColor(1, 0, 0);
		}
		else if (i < 6) { // 緑
            g_Sphere[i].position.set( 0, 0, 0);
			g_Sphere[i].setColor(0, 1, 0);
		}
		else {            // 青
            g_Sphere[i].position.set( 5, 0, 0);
            g_Sphere[i].setColor(0, 0, 1);
		}
		// 適当な速度を設定
		Vector3d randSpeed = Vector3d(rand(mt), rand(mt), rand(mt));
		g_Sphere[i].vec = randSpeed;
	}

	glClearDepth(1000.0);
	glClearColor(1, 1, 1, 1); // 背景の色を白に設定

	// 照明の設定
	float lightAmbientColor[] = { 0.2f, 0.2f, 0.2f, 0.0f };
	float lightDiffuseColor[] = { 1.f, 1.f, 1.f, 0.0f };
	float lightSpecularColor[] = { 0.4f, 0.4f, 0.4f, 0.0f };
	float lightPosition[] = { 0.0f, 30.0f, 30.0f, 0.0f };
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbientColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuseColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecularColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	// 材質の設定
	float specularColor[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float diffuseColor[] = { 1.f, 0.f, 0.f, 1.f };
	float shininess = 64.f;
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularColor);
	glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseColor);
}

int main(int argc, char**argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(g_WindowWidth, g_WindowHeight);
	glutCreateWindow("Mouse Picking");

	glutDisplayFunc(display);
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(g_AnimationIntervalMsec, timer, 0);

	init();

	glutMainLoop();

	return 0;
}

