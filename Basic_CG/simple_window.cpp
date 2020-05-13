#define FREEGLUT_STATIC
#include <math.h>
#include <tuple>
#include <string>
#include <GL/glut.h> // ���C�u�����p�w�b�_�t�@�C���̓ǂݍ���

// �萔�΂̒�`
#ifndef M_PI
#define M_PI 3.14159265358979
#endif

// �f�B�X�v���C���X�g�̊w�K
// ����`�悷��`�施�߈ꎮ���A�f�B�X�v���C���X�g�Ƃ��č쐬���Ă���
// �K�v�Ȏ��ɁA���̖��߂��Ăяo��

#define ID_DRAW_STAR 1 //  glNewList �֐��Ŏg�p���鎯��ID�B�l�͉��ł��\��Ȃ�
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
	b = 0.5; // B�Œ�
	double diff = pos / 10.0;
	int k = int((r + diff) * 100.0) % 100; r = k / 100.0;
	g = (g - diff) < 0 ? (g - diff + 100.0) : (g - diff);
	return std::make_tuple(r, g, b);
}

// �\�����������̊֐��ŋL��
void display(void) {        
	glClearColor (1.0, 1.0, 1.0, 1.0);  // �����F�w��
	glClear(GL_COLOR_BUFFER_BIT);       // ��ʂƉ��s������������

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

	glutSwapBuffers(); // �o�b�t�@�̓���ւ�
}

// ��莞�Ԃ��ƂɌĂяo�����֐�
void timer(int value) {
	position += 0.005;
	rotate++;
	glutPostRedisplay(); // �ĕ`�施��
	glutTimerFunc(100 , timer , 0); // 100�~���b��Ɏ��g�����s����
}

// �f�B�X�v���C���X�g���쐬����
void buildDisplayList() {
	glNewList(ID_DRAW_STAR,GL_COMPILE);

	int vertexes = 5 * 2;	// ���̒��_��
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


// ���C���v���O����
int main (int argc, char *argv[]) { 
	glutInit(&argc, argv);          // ���C�u�����̏�����
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);

	glutInitWindowSize(400 , 400);  // �E�B���h�E�T�C�Y���w��
	glutCreateWindow(argv[0]);      // �E�B���h�E���쐬
	glutDisplayFunc(display);       // �\���֐����w��

	glutTimerFunc(100 , timer , 0); // 100�~���b��Ɏ��s����֐��̎w��

	buildDisplayList();

	position = 0.0;

	glutMainLoop();                 // �C�x���g�҂�
	return 0;
}
