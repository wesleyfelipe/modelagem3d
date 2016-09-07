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
#include "MtlReader.h"

#define M_PI (3.141592653589793)

using namespace std;

//dimensoes da tela
int width = 800;
int height = 600;
//angulo
float a = 270;
//tamanho do mundo 400x400 (maxX e maxZ sao as dimensoes maximas do mundo em cada quadrante)
int maxX = 200;
int maxZ = 200;
//posicao da camera no mundo
Vertex *eye = new Vertex(0, 0, 0);
//direcao em que aponta a camera
Vertex *at = new Vertex(0,0,0);
//altura
Vertex *up = new Vertex(0, 0, 0);
//vetor dos objetos renderizados
vector<Mesh*> objetos;
vector<Material*> materials;

void desenhaMalha(Mesh *mesh) {
	glEnable(GL_DEPTH_TEST);
	
	for (int i = 0; i < mesh->getGroups()->size(); i++) {
		for (int j = 0; j < mesh->getGroups()->at(i)->getGroupFaces()->size(); j++) {
			int faceSize = mesh->getGroups()->at(i)->getGroupFaces()->at(j)->getVertex()->size();
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

			for (int k = 0; k < mesh->getGroups()->at(i)->getGroupFaces()->at(j)->getVertex()->size(); k++) {
				GLint vi = mesh->getGroups()->at(i)->getGroupFaces()->at(j)->getVertex()->at(k);
				GLint ni = mesh->getGroups()->at(i)->getGroupFaces()->at(j)->getNormals()->at(k);
				glNormal3fv(mesh->getAllNormals()->at(ni)->getCoord());
				glVertex3fv(mesh->getAllVertex()->at(vi)->getCoord());
			}
			glEnd();
		}
	}
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (Mesh *m : objetos) {
		desenhaMalha(m);
	}
	glutSwapBuffers();
}

void configView(void) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (double)width / (double)height, 1, 100);
	gluLookAt(eye->getX(), eye->getY(), eye->getZ(), at->getX(), at->getY(), at->getZ(), up->getX(), up->getY(), up->getZ());
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	srand((unsigned)time(NULL));
	eye->update(0,13,0);
	at->update(0,1,-1);
	up->update(0, -0.6, 0);
	a = 270;
	configView();
}

void update(int value) {
	glLoadIdentity();
	gluLookAt(eye->getX(), eye->getY(), eye->getZ(), at->getX(), at->getY(), at->getZ(), up->getX(), up->getY(), up->getZ());
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
		case GLUT_KEY_LEFT:{
			if (a == 0) {
				a = 360;
			}
			a -= 3;
			at->update(eye->getX() + cos((a*M_PI) / 180), at->getY(), eye->getZ() + sin((a*M_PI) / 180));
			break;
		}
		case GLUT_KEY_RIGHT:{
			if (a == 360) {
				a = 0;
			}
			a += 3;
			at->update(eye->getX() + cos((a*M_PI) / 180), at->getY(), eye->getZ() + sin((a*M_PI) / 180));
			break;
		}
		case GLUT_KEY_UP:{
			double novoZ1 = eye->getZ() + sin((a*M_PI) / 180);
			double novoX1 = eye->getX() + cos((a*M_PI) / 180);
			if (!(novoZ1 >= maxZ || novoZ1 <= -maxZ || novoX1 >= maxX || novoX1 <= -maxX)) {
				eye->update(novoX1, eye->getY(), novoZ1);
				at->update(eye->getX() + cos((a*M_PI) / 180), at->getY(), eye->getZ() + sin((a*M_PI) / 180));
			}
			break;
		}
		case GLUT_KEY_DOWN:{
			double novoZ2 = eye->getZ() - sin((a*M_PI) / 180);
			double novoX2 = eye->getX() - cos((a*M_PI) / 180);
			if (!(novoZ2 >= maxZ || novoZ2 <= -maxZ || novoX2 >= maxX || novoX2 <= -maxX)) {
				eye->update(novoX2, eye->getY(), novoZ2);
				at->update(eye->getX() + cos((a*M_PI) / 180), at->getY(), eye->getZ() + sin((a*M_PI) / 180));
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
	printf("%s", "Inicializando... \n");

	ObjReader *objReader = new ObjReader();
	MltReader *mtlReader = new MltReader();

	ObjSpec *spec1 = objReader->readObjFile(".\\objs\\mesa\\mesa01.obj");
	mtlReader->readMtlFile(".\\objs\\mesa\\" + spec1->getMtllibFilename(), &materials);

	ObjSpec *spec2 = objReader->readObjFile(".\\objs\\paintball\\cenaPaintball.obj");
	mtlReader->readMtlFile(".\\objs\\paintball\\" + spec2->getMtllibFilename(), &materials);

	objetos.push_back(spec1->getMesh());
	objetos.push_back(spec2->getMesh());

	printf("%s %d %s", "Total de malhas lidas:", objetos.size(), "\n");
	printf("%s %d %s", "Total de materiais lidos:", materials.size(), "\n");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height); //tamanho da janela
	glutInitWindowPosition(0, 0); //posi��o da janela
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
