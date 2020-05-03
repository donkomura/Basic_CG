#define _USE_MATH_DEFINES
#define FREEGLUT_STATIC
#include <GL/glut.h> // ���C�u�����p�w�b�_�t�@�C���̓ǂݍ���
#include <math.h>
#include <cstdio>

int WSize = 400, HSize = 400;  // �`��T�C�Y

// �\�����������̊֐��ŋL��
void display(void) {        
	glClearColor (1.0, 1.0, 1.0, 1.0);  // �����F�w��
	glClear (GL_COLOR_BUFFER_BIT );     // ��ʏ���

	int vertexes = 5 * 2;	// ���̒��_��
	// glColor3d(1.0, 1.0, 0.6);   // �F�w��(R,G,B)��0�`1�܂�
	glColor3d(0, 0, 1.0);   // �F�w��(R,G,B)��0�`1�܂�
	glBegin(GL_LINE_LOOP); 
	for (int i = 0; i < vertexes; i++) {
		double r = i % 2 ? 0.5 : 1.0;
		double phi = 2.0 * M_PI * (i + 0.5) / vertexes;
		double x = r * cos(phi);
		double y = r * sin(phi);
		glVertex2d(x, y);
	}
	glEnd(); 
	
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