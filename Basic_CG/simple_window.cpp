#define FREEGLUT_STATIC
#include <math.h>
#include <tuple>
#include <string>
#include <GL/glut.h> // ライブラリ用ヘッダファイルの読み込み

// 定数πの定義
#ifndef M_PI
#define M_PI 3.14159265358979
#endif

// ディスプレイリストの学習
// 星を描画する描画命令一式を、ディスプレイリストとして作成しておき
// 必要な時に、その命令を呼び出す

#define ID_DRAW_STAR 1 //  glNewList 関数で使用する識別ID。値は何でも構わない
#define RED "RED"
#define BLUE "BLUE"
#define GREEN "GREEN"

using rgb = std::tuple<double, double, double>;

double rotate;
double position = -1.0;
double R = 0.0, G = 0.0, B = 0.0;

rgb nextColor(rgb color, double pos) {
	double r, g, b;
	std::tie(r, g, b) = color;
	b = 0.5; // B固定
	double diff = pos / 10.0;
	int k = int((r + diff) * 100.0) % 100; r = k / 100.0;
	g = (g - diff) < 0 ? (g - diff + 100.0) : (g - diff);
	return std::make_tuple(r, g, b);
}

// 表示部分をこの関数で記入
void display(void) {        
	glClearColor (1.0, 1.0, 1.0, 1.0);  // 消去色指定
	glClear(GL_COLOR_BUFFER_BIT);       // 画面と奥行き情報を初期化

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    glTranslated(position, -position, 0);

	int count =12, cnt = 0;
	for (double diff = -10.0; diff <= 10.0; diff += 0.2) {
		std::tie(R, G, G) = nextColor(std::make_tuple(R, G, B), position);
		for (double i = -2.0; i <= 2.0; i += 2.0 / count) {
			glPushMatrix();
			glColor3d(R, G, B);
			glTranslated(position + diff, i, 0);
			glRotated(rotate, 0, 0, 1);
			glCallList(ID_DRAW_STAR);
			glPopMatrix();
			cnt++;
		}
	}

	glutSwapBuffers(); // バッファの入れ替え
}

// 一定時間ごとに呼び出される関数
void timer(int value) {
	position += 0.005;
	rotate++;
	glutPostRedisplay(); // 再描画命令
	glutTimerFunc(100 , timer , 0); // 100ミリ秒後に自身を実行する
}

// ディスプレイリストを作成する
void buildDisplayList() {
	glNewList(ID_DRAW_STAR,GL_COMPILE);

	int vertexes = 5 * 2;	// 星の頂点数
	double size = 0.1;
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < vertexes; i++) {
		double r = size * (i % 2 ? 0.5 : 1.0);
		double phi = 2.0 * M_PI * (i + 0.5) / vertexes;
		double x = r * cos(phi);
		double y = r * sin(phi);
		glVertex2d(x, y);
	}
	glEnd();

	glEndList();
}


// メインプログラム
int main (int argc, char *argv[]) { 
	glutInit(&argc, argv);          // ライブラリの初期化
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);

	glutInitWindowSize(400 , 400);  // ウィンドウサイズを指定
	glutCreateWindow(argv[0]);      // ウィンドウを作成
	glutDisplayFunc(display);       // 表示関数を指定

	glutTimerFunc(100 , timer , 0); // 100ミリ秒後に実行する関数の指定

	buildDisplayList();

	position = 0.0;

	glutMainLoop();                 // イベント待ち
	return 0;
}
