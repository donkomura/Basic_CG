#define _USE_MATH_DEFINES
#define FREEGLUT_STATIC
#include <GL/glut.h> // ライブラリ用ヘッダファイルの読み込み
#include <math.h>
#include <cstdio>

int WSize = 400, HSize = 400;  // 描画サイズ

class Point {
public:
	GLdouble x, y;

	Point() { x = y = 0;  }
	Point(double xx, double yy) : x(xx), y(yy) {}

	Point operator* (Point p) {
		return Point(this->x * p.x, this->y * p.y);
	}
};

void draw_line(Point p, Point q) {
    glBegin(GL_LINES);
        glVertex2d(p.x, p.y);
        glVertex2d(q.x, q.y);
    glEnd();
	return;
}

void koch(int n, Point p, Point q) {
	Point a((2 * p.x + q.x) / 3, (2 * p.y + q.y) / 3);
	Point b((p.x + 2 * q.x) / 3, (p.y + 2 * q.y) / 3);
	Point c;
	double X = q.x - p.x;
	double Y = q.y - p.y;
	double dist = sqrt(X * X + Y * Y) / sqrt(3);
	double phi = 0.0;

	if (X >= 0) {
		phi = atan(Y / X) + M_PI / 6;
		c.x = p.x + dist * cos(phi);
		c.y = p.y + dist * sin(phi);
	} else {
		phi = atan(Y / X) - M_PI / 6;
		c.x = q.x + dist * cos(phi);
		c.y = q.y + dist * sin(phi);
	}

	if (n <= 0) {
		draw_line(p, a);	// PからA
		draw_line(a, c);	// AからC
		draw_line(c, b);	// CからB
		draw_line(b, q);	// BからQ
		return;
	}

	koch(n - 1, p, a);
	koch(n - 1, a, c);
	koch(n - 1, c, b);
	koch(n - 1, b, q);
}

// 表示部分をこの関数で記入
void display(void) {        
	glClearColor (1.0, 1.0, 1.0, 1.0);  // 消去色指定
	glClear (GL_COLOR_BUFFER_BIT );     // 画面消去

	glColor3d(0, 0, 1.0);   // 色指定(R,G,B)で0～1まで
	int n = 10;  // iteration
	Point mag = Point(0.7, 0.7);  // 図の倍率
	// 正三角形になるように各曲線を生成・配置
	// double a = 1.0; // 1辺の長さ
	// Point P(a / sqrt(3), 0), Q(- a / (2.0 * sqrt(3)), - a / 2.0), R(-a / (2.0 * sqrt(3)), a / 2.0);  // 90°回転・辺の長さを指定
	Point P(0.0, 0.8), Q(0.9, -0.8), R(-0.9, -0.8);
	P = P * mag; Q = Q * mag; R = R * mag;

	koch(n, P, Q);
	koch(n, Q, R);
	koch(n, R, P);
	
	glFlush(); // 画面出力
}

// メインプログラム
int main (int argc, char *argv[]) { 
	glutInit(&argc, argv);          // ライブラリの初期化
	glutInitWindowSize(WSize , HSize);  // ウィンドウサイズを指定
	glutCreateWindow(argv[0]);      // ウィンドウを作成
	glutDisplayFunc(display);       // 表示関数を指定
	glutMainLoop();                 // イベント待ち
	return 0;
}