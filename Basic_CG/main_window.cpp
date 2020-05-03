#define _USE_MATH_DEFINES
#define FREEGLUT_STATIC
#include <GL/glut.h> // ���C�u�����p�w�b�_�t�@�C���̓ǂݍ���
#include <math.h>
#include <cstdio>

int WSize = 400, HSize = 400;  // �`��T�C�Y

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
		draw_line(p, a);	// P����A
		draw_line(a, c);	// A����C
		draw_line(c, b);	// C����B
		draw_line(b, q);	// B����Q
		return;
	}

	koch(n - 1, p, a);
	koch(n - 1, a, c);
	koch(n - 1, c, b);
	koch(n - 1, b, q);
}

// �\�����������̊֐��ŋL��
void display(void) {        
	glClearColor (1.0, 1.0, 1.0, 1.0);  // �����F�w��
	glClear (GL_COLOR_BUFFER_BIT );     // ��ʏ���

	glColor3d(0, 0, 1.0);   // �F�w��(R,G,B)��0�`1�܂�
	int n = 10;  // iteration
	Point mag = Point(0.7, 0.7);  // �}�̔{��
	// ���O�p�`�ɂȂ�悤�Ɋe�Ȑ��𐶐��E�z�u
	// double a = 1.0; // 1�ӂ̒���
	// Point P(a / sqrt(3), 0), Q(- a / (2.0 * sqrt(3)), - a / 2.0), R(-a / (2.0 * sqrt(3)), a / 2.0);  // 90����]�E�ӂ̒������w��
	Point P(0.0, 0.8), Q(0.9, -0.8), R(-0.9, -0.8);
	P = P * mag; Q = Q * mag; R = R * mag;

	koch(n, P, Q);
	koch(n, Q, R);
	koch(n, R, P);
	
	glFlush(); // ��ʏo��
}

// ���C���v���O����
int main (int argc, char *argv[]) { 
	glutInit(&argc, argv);          // ���C�u�����̏�����
	glutInitWindowSize(WSize , HSize);  // �E�B���h�E�T�C�Y���w��
	glutCreateWindow(argv[0]);      // �E�B���h�E���쐬
	glutDisplayFunc(display);       // �\���֐����w��
	glutMainLoop();                 // �C�x���g�҂�
	return 0;
}