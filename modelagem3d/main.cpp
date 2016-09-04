#include <windows.h>
#include <gl/glu.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <iostream>
#include "estruturas.h"
#include "OBJReader.h"

#define M_PI (3.141592653589793)

using namespace std;

//dimensoes da tela
int width = 800;
int height = 600;
//posicao da camera no mundo
float eyex = 0;
float eyey = 0;
float eyez = 0;
//direcao em que aponta a camera
float dx = 0;
float dy = 0;
float dz = -1;
//altura
Vertex *up = new Vertex(0, 0, 0);

//angulo
float a = 270;
//tamanho do mundo 400x400 (maxX e maxZ sao as dimensoes maximas do mundo em cada quadrante)
int maxX = 200;
int maxZ = 200;

vector<Mesh*> objetos;

void desenhaObjeto(Mesh *mesh) {
	glEnable(GL_DEPTH_TEST);
	
	for (int i = 0; i < mesh->groups.size(); i++) {
		for (int j = 0; j < mesh->groups[i]->groupFaces.size(); j++) {
			int faceSize = mesh->groups[i]->groupFaces[j]->vertex.size();
			if (faceSize == 3) {
				glBegin(GL_TRIANGLES);
			} else if (faceSize == 4) {
				glBegin(GL_QUADS);
			} else {
				glBegin(GL_POLYGON);
			}

			float r = ((float)rand() / (RAND_MAX));
			float g = ((float)rand() / (RAND_MAX));
			float b = ((float)rand() / (RAND_MAX));
			glColor3d(r, g, b);

			for (int k = 0; k < mesh->groups[i]->groupFaces[j]->vertex.size(); k++) {
				int vi = mesh->groups[i]->groupFaces[j]->vertex[k];
				int ni = mesh->groups[i]->groupFaces[j]->normals[k];
				glNormal3fv(mesh->allNormals[ni]->coord);
				glVertex3fv(mesh->allVertex[vi]->coord);
			}
			glEnd();
		}
	}
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (Mesh *m : objetos) {
		desenhaObjeto(m);
	}
	glutSwapBuffers();
}

void configView(void) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (double)width / (double)height, 1, 100);
	gluLookAt(eyex, eyey, eyez, dx, dy, dz, up->getX(), up->getY(), up->getZ());
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	srand((unsigned)time(NULL));
	eyex = 0;
	eyey = 13;
	eyez = 0;
	dx = 0;
	dy = 1;
	dz = -1;
	up->update(0, -0.6, 0);
	a = 270;
	configView();
}

void update(int value) {
	glLoadIdentity();
	gluLookAt(eyex, eyey, eyez, dx, dy, dz, up->getX(), up->getY(), up->getZ());
	glutPostRedisplay();
	glutTimerFunc(33, update, 1);
}

void reshape(int w, int h) {
	if (h < 1)
		h = 1;
	width = w;
	height = h;
	configView();
}

void specialKeyboard(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
	{
		if (a == 0) {
			a = 360;
		}
		a -= 3;
		dz = eyez + sin((a*M_PI) / 180);
		dx = eyex + cos((a*M_PI) / 180);
		break;
	}
	case GLUT_KEY_RIGHT:
	{
		if (a == 360) {
			a = 0;
		}
		a += 3;
		dz = eyez + sin((a*M_PI) / 180);
		dx = eyex + cos((a*M_PI) / 180);
		break;
	}
	case GLUT_KEY_UP:
	{
		double novoZ1 = eyez + sin((a*M_PI) / 180);
		double novoX1 = eyex + cos((a*M_PI) / 180);
		if (!(novoZ1 >= maxZ || novoZ1 <= -maxZ || novoX1 >= maxX || novoX1 <= -maxX)) {
			eyez = novoZ1;
			eyex = novoX1;
			dz = eyez + sin((a*M_PI) / 180);
			dx = eyex + cos((a*M_PI) / 180);
		}
		break;
	}
	case GLUT_KEY_DOWN:
	{
		double novoZ2 = eyez - sin((a*M_PI) / 180);
		double novoX2 = eyex - cos((a*M_PI) / 180);
		if (!(novoZ2 >= maxZ || novoZ2 <= -maxZ || novoX2 >= maxX || novoX2 <= -maxX)) {
			eyez = novoZ2;
			eyex = novoX2;
			dz = eyez + sin((a*M_PI) / 180);
			dx = eyex + cos((a*M_PI) / 180);
		}
		break;
	}
	}
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':
	case 'Q':
		exit(0);
	}
}

int main(int argc, char** argv) {

	objetos.push_back(readObjFile(".\\objs\\mesa\\mesa01.obj"));
	objetos.push_back(readObjFile(".\\objs\\paintball\\cenaPaintball.obj"));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height); //tamanho da janela
	glutInitWindowPosition(0, 0); //posição da janela
	glutCreateWindow("Modelagem 3D");
	// glutFullScreen();

	init();

	glutDisplayFunc(display); //desenho
	glutReshapeFunc(reshape); //tratamento do redimensionamento da tela
	glutSpecialFunc(specialKeyboard); // tratamento das teclas especiais
	glutKeyboardFunc(keyboard); //tratamento do teclado

	glutTimerFunc(33, update, 1); //controla tempo do update

	glutMainLoop();

	return 0;
}
