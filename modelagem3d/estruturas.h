#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <vector>

using namespace std;

typedef struct TextureMapping {
	float coord[2];
	TextureMapping(float x, float y) {
		coord[0] = x;
		coord[1] = y;
	};
} TextureMapping;

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
	vector<GLint> mappings;
	Face(vector<GLint> v, vector<GLint> n, vector<GLint> m) {
		vertex = v;
		normals = n;
		mappings = m;
	}
	Face(){}
} Face;

typedef struct Group {
	string name;
	GLint material;
	vector<Face*> groupFaces;
	Group(vector<Face*> f) {
		groupFaces = f;
	}
	Group(){}
} Group;

typedef struct Mesh {
	vector<Group*> groups;
	vector<Vertex*> allVertex;
	vector<Vertex*> allNormals;
	vector<TextureMapping*> allMappings;
	Mesh(vector<Group*> g, vector<Vertex*> v, vector<Vertex*> n, vector<TextureMapping*> m) {
		groups = g;
		allVertex = v;
		allNormals = n;
		allMappings = m;
	}
	Mesh() {
	}

} Mesh;

#endif
