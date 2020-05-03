#define _USE_MATH_DEFINES
#define FREEGLUT_STATIC
#include <GL/glut.h> // ライブラリ用ヘッダファイルの読み込み
#include <math.h>
#include <cstdio>

int WSize = 400, HSize = 400;  // 描画サイズ

// 表示部分をこの関数で記入
void display(void) {        
	glClearColor (1.0, 1.0, 1.0, 1.0);  // 消去色指定
	glClear (GL_COLOR_BUFFER_BIT );     // 画面消去

	int vertexes = 5 * 2;	// 星の頂点数
	// glColor3d(1.0, 1.0, 0.6);   // 色指定(R,G,B)で0～1まで
	glColor3d(0, 0, 1.0);   // 色指定(R,G,B)で0～1まで
	glBegin(GL_LINE_LOOP); 
	for (int i = 0; i < vertexes; i++) {
		double r = i % 2 ? 0.5 : 1.0;
		double phi = 2.0 * M_PI * (i + 0.5) / vertexes;
		double x = r * cos(phi);
		double y = r * sin(phi);
		glVertex2d(x, y);
	}
	glEnd(); 
	
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