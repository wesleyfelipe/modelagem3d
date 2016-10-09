#include <windows.h>
#include <gl/glu.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <map>
#include <iostream>
#include "estruturas.h"
#include "OBJReader.h"
#include "MtlReader.h"
#include "Image.h"
#include "ImageReader.h"

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
//vetor de materiais
map<string,Material*> materials;
// fonte de luz 0
GLfloat light0_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light0_diffuse[] = { 0.3, 0.5, 1.0, 1.0 };
GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light0_position[] = { 1.0, 1.0, 3.0, 1.0 };
// flag iluminacao
bool light = true;

GLuint texName;

void moveObjects(float x, float y, float z) {
	for (int i = 0; i < objetos.size(); i++) {
		for (int j = 0; j < objetos.at(i)->getAllVertex()->size(); j++) {
			objetos.at(i)->getAllVertex()->at(j)->addXYZ(x, y, z);
		}
	}
}

void desenhaMalha(Mesh *mesh) {
	glBindTexture(GL_TEXTURE_2D, texName);
	
	for (int i = 0; i < mesh->getGroups()->size(); i++) {
		Group *group = mesh->getGroups()->at(i);

		string matId = group->getMaterial();
		if (!matId.empty()) {
			Material *ma = materials.at(matId);
			//GLuint tid = ma->getTextureId();
			//glBindTexture(GL_TEXTURE_2D, tid);
			//glMaterialfv(GL_FRONT, GL_AMBIENT, ma->getKa());
			//glMaterialfv(GL_FRONT, GL_DIFFUSE, ma->getKd());
			//glMaterialfv(GL_FRONT, GL_SPECULAR, ma->getKs());
			//glMaterialf(GL_FRONT, GL_SHININESS, ma->getNs());
		} 

		for (int j = 0; j < group->getGroupFaces()->size(); j++) {
			Face *face = group->getGroupFaces()->at(j);
			int faceSize = face->getVertex()->size();
			if (faceSize == 3) {
				glBegin(GL_TRIANGLES);
			} else if (faceSize == 4) {
				glBegin(GL_QUADS);
			} else {
				glBegin(GL_POLYGON);
			}

			for (int k = 0; k < face->getVertex()->size(); k++) {
				GLint vi = face->getVertex()->at(k);
				GLint ni = face->getNormals()->at(k);
				GLint vt = face->getMappings()->at(k);
				glTexCoord2fv(mesh->getAllMappings()->at(vt)->getCoord());
				glNormal3fv(mesh->getAllNormals()->at(ni)->getCoord());
				glVertex3fv(mesh->getAllVertex()->at(vi)->getCoord());
			}
			glEnd();
		}
	}
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	for (Mesh *m : objetos) {
		desenhaMalha(m);
	}

	glutSwapBuffers();
	glDisable(GL_TEXTURE_2D);
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

void enableLight() {
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}

void disableLight() {
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
}

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	srand((unsigned)time(NULL));
	eye->update(0,13,0);
	at->update(0,1,-1);
	up->update(0, -0.6, 0);
	a = 270;

	glShadeModel(GL_SMOOTH);

	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	enableLight();

	glEnable(GL_DEPTH_TEST);

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
			break;
		case 'd':
		case 'D':
			moveObjects(1.0f, 0.0f, 0.0f);
			break;
		case 'a':
		case 'A':
			moveObjects(-1.0f, 0.0f, 0.0f);
			break;
		case 'w':
		case 'W':
			moveObjects(+cos((a*M_PI) / 180), 0.0f, +sin((a*M_PI) / 180));
			break;
		case 's':
		case 'S':
			moveObjects(-cos((a*M_PI) / 180), 0.0f, -sin((a*M_PI) / 180));
			break;
		case 'z':
		case 'Z':
			moveObjects(0.0f, 1.0f, 0.0f);
			break;
		case 'x':
		case 'X':
			moveObjects(0.0f, -1.0f, 0.0f);
			break;
		case 'l':
		case 'L':
			if (!light) {
				enableLight();
				light = true;
			} else {
				disableLight();
				light = false;
			}
			break;
	}
}

GLint contarTotalTexturasMateriais(map<string, Material*> mat) {
	GLint k = 0;
	for (auto it: mat ) {
		if (it.second->hasTexture()) {
			k++;
		}
	}
	return k;
}

void setup() {
	printf("%s", "Inicializando... \n");

	ObjReader *objReader = new ObjReader();
	MltReader *mtlReader = new MltReader();
	ImageReader *imageReader = new ImageReader();

	ObjSpec *spec1 = objReader->readObjFile(".\\objs\\mesa\\mesa01.obj");
	mtlReader->readMtlFile(".\\objs\\mesa\\" + spec1->getMtllibFilename(), &materials);

	ObjSpec *spec2 = objReader->readObjFile(".\\objs\\paintball\\cenaPaintball.obj");
	mtlReader->readMtlFile(".\\objs\\paintball\\" + spec2->getMtllibFilename(), &materials);

	objetos.push_back(spec1->getMesh());
	objetos.push_back(spec2->getMesh());

	//Texturas
	Image *image = imageReader->lerArquivo("muro02.ppm");

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->getWidth(),
		image->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
		image->getPixels());

	free(image->getPixels());

	/*GLint textureCount = contarTotalTexturasMateriais(materials);
	GLuint *ids = new GLuint[textureCount];
	glGenTextures(textureCount, ids);

	int k = 0;
	for (auto it : materials) {
		if (it.second->hasTexture()) {
			it.second->setTextureId(ids[k++]);
			glBindTexture(GL_TEXTURE_2D, it.second->getTextureId());
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->getWidth(), img->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, img->getPixels());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
	} */

	printf("%s %d %s", "Total de malhas lidas:", objetos.size(), "\n");
	printf("%s %d %s", "Total de materiais lidos:", materials.size(), "\n");
	//printf("%s %d %s", "Total de materiais com textura: ", textureCount, "\n");
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height); //tamanho da janela
	glutInitWindowPosition(0, 0); //posição da janela
	glutCreateWindow("Modelagem 3D");
	// glutFullScreen();

	init();
	setup();

	glutDisplayFunc(display); //desenho
	glutReshapeFunc(reshape); //tratamento do redimensionamento da tela
	glutSpecialFunc(specialKeyboard); // tratamento das teclas especiais
	glutKeyboardFunc(keyboard); //tratamento do teclado

	glutTimerFunc(33, update, 1); //controla tempo do update

	glutMainLoop();

	return 0;
}
