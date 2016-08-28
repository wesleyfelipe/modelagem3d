#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <vector>

using namespace std;

typedef struct Vertex {
	float coord[3];
	Vertex(float x, float y, float z) {
		coord[0] = x;
		coord[1] = y;
		coord[2] = z;
	};
} Vertex;

typedef struct Face {
	vector<GLint> vertex;
	vector<GLint> normals;
	Face(vector<GLint> v, vector<GLint> n) {
		vertex = v;
		normals = n;
	}
} Face;

typedef struct Group {
	vector<Face*> groupFaces;
	Group(vector<Face*> f) {
		groupFaces = f;
	}
} Group;

typedef struct Mesh {
	vector<Group*> groups;
	vector<Vertex*> allVertex;
	vector<Vertex*> allNormals;
	Mesh(vector<Group*> g, vector<Vertex*> v, vector<Vertex*> n) {
		groups = g;
		allVertex = v;
		allNormals = n;
	}
} Mesh;

#endif
