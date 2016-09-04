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
float px = 0;
float py = 1;
float pz = 0;
//angulo
float a = 270;
//tamanho do mundo 400x400 (maxX e maxZ sao as dimensoes maximas do mundo em cada quadrante)
int maxX = 200;
int maxZ = 200;

vector<Mesh*> objetos;

Mesh* getCuboMesh() {
	vector<Vertex*> allVertex;
	allVertex.push_back(new Vertex(1.0f, 1.0f, 1.0f));
	allVertex.push_back(new Vertex(-1.0f, 1.0f, 1.0f));
	allVertex.push_back(new Vertex(-1.0f, -1.0f, 1.0f));
	allVertex.push_back(new Vertex(1.0f, -1.0f, 1.0f));
	allVertex.push_back(new Vertex(1.0f, 1.0f, -1.0f));
	allVertex.push_back(new Vertex(-1.0f, 1.0f, -1.0f));
	allVertex.push_back(new Vertex(-1.0f, -1.0f, -1.0f));
	allVertex.push_back(new Vertex(1.0f, -1.0f, -1.0f));

	vector<Vertex*> allNormals;
	allNormals.push_back(new Vertex(1.0f, 0.0f, 0.0f));
	allNormals.push_back(new Vertex(-1.0f, 0.0f, 0.0f));
	allNormals.push_back(new Vertex(0.0f, 1.0f, 0.0f));
	allNormals.push_back(new Vertex(0.0f, -1.0f, 0.0f));
	allNormals.push_back(new Vertex(0.0f, 0.0f, 1.0f));
	allNormals.push_back(new Vertex(0.0f, 0.0f, -1.0f));

	vector<GLint> f0v;
	f0v.push_back(0);
	f0v.push_back(1);
	f0v.push_back(2);
	f0v.push_back(3);

	vector<GLint> f0n;
	f0n.push_back(4);
	f0n.push_back(4);
	f0n.push_back(4);
	f0n.push_back(4);

	vector<GLint> f1v;
	f1v.push_back(0);
	f1v.push_back(3);
	f1v.push_back(7);
	f1v.push_back(4);

	vector<GLint> f1n;
	f1n.push_back(0);
	f1n.push_back(0);
	f1n.push_back(0);
	f1n.push_back(0);

	vector<GLint> f2v;
	f2v.push_back(0);
	f2v.push_back(4);
	f2v.push_back(5);
	f2v.push_back(1);

	vector<GLint> f2n;
	f2n.push_back(2);
	f2n.push_back(2);
	f2n.push_back(2);
	f2n.push_back(2);

	vector<GLint> f3v;
	f3v.push_back(2);
	f3v.push_back(1);
	f3v.push_back(5);
	f3v.push_back(6);

	vector<GLint> f3n;
	f3n.push_back(1);
	f3n.push_back(1);
	f3n.push_back(1);
	f3n.push_back(1);

	vector<GLint> f4v;
	f4v.push_back(3);
	f4v.push_back(2);
	f4v.push_back(6);
	f4v.push_back(7);

	vector<GLint> f4n;
	f4n.push_back(3);
	f4n.push_back(3);
	f4n.push_back(3);
	f4n.push_back(3);

	vector<GLint> f5v;
	f5v.push_back(7);
	f5v.push_back(6);
	f5v.push_back(5);
	f5v.push_back(4);

	vector<GLint> f5n;
	f5n.push_back(5);
	f5n.push_back(5);
	f5n.push_back(5);
	f5n.push_back(5);

	vector<GLint> mappings;

	vector<Face*> faces;
	faces.push_back(new Face(f0v, f0n, mappings));
	faces.push_back(new Face(f1v, f1n, mappings));
	faces.push_back(new Face(f2v, f2n, mappings));
	faces.push_back(new Face(f3v, f3n, mappings));
	faces.push_back(new Face(f4v, f4n, mappings));
	faces.push_back(new Face(f5v, f5n, mappings));

	vector<Group*> groups;
	groups.push_back(new Group(faces));

	vector<TextureMapping*> allMappings;
	return new Mesh(groups, allVertex, allNormals, allMappings);
}

void desenhaObjeto(Mesh *mesh) {
	glEnable(GL_DEPTH_TEST);

	glColor3d(0.7, 0.6, 0.5);

	for (int i = 0; i < mesh->groups.size(); i++) {
		for (int j = 0; j < mesh->groups[i]->groupFaces.size(); j++) {
			glBegin(GL_POLYGON);
			for (int k = 0; k < mesh->groups[i]->groupFaces[j]->vertex.size(); k++) {
				//glColor3d(0.7 / (i + j + k + 1), 0.6 / (i + j + k + 1), 0.5 / (i + j + k + 1));
				int vi = mesh->groups[i]->groupFaces[j]->vertex[k];
				glVertex3fv(mesh->allVertex[vi]->coord);
			}
			glEnd();
		}
	}
}

void desenhaChao(void) {
	//quadrante 4 - red
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3d(0, -1, 0);
	glVertex3d(0, -1, -maxZ);
	glVertex3d(maxX, -1, -maxZ);
	glVertex3d(maxX, -1, 0);
	glEnd();
	//quadrante 3 - green
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3d(0, -1, 0);
	glVertex3d(0, -1, -maxZ);
	glVertex3d(-maxX, -1, -maxZ);
	glVertex3d(-maxX, -1, 0);
	glEnd();
	//quadrante 2 - blue
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_QUADS);
	glVertex3d(0, -1, 0);
	glVertex3d(0, -1, maxZ);
	glVertex3d(-maxX, -1, maxZ);
	glVertex3d(-maxX, -1, 0);
	glEnd();
	//quadrante 1 - yellow
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3d(0, -1, 0);
	glVertex3d(maxX, -1, 0);
	glVertex3d(maxX, -1, maxZ);
	glVertex3d(0, -1, maxZ);
	glEnd();
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	desenhaChao();
	//desenhaObjeto(getCuboMesh());
	desenhaObjeto(objetos[0]);
	glutSwapBuffers();
}

void configView(void) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (double)width / (double)height, 1, 50);
	gluLookAt(eyex, eyey, eyez, dx, dy, dz, px, py, pz);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	srand((unsigned)time(NULL));
	eyex = 0;
	eyey = 0;
	eyez = 0;
	dx = 0;
	dy = 0;
	dz = -1;
	a = 270;
	configView();
}

void update(int value) {
	glLoadIdentity();
	gluLookAt(eyex, eyey, eyez, dx, dy, dz, px, py, pz);
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
	cout << objetos[0]->allVertex.size();
	cout << "\n";
	cout << objetos[0]->allNormals.size();
	cout << "\n";
	cout << objetos[0]->allMappings.size();
	cout << "\n";
	cout << objetos[0]->groups[0]->groupFaces.size();

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
