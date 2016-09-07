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

typedef struct Material {
	string id;
	float ka[4];
	float kd[4];
	float ks[4];
	float ns;
	string mapKd;
	Material(){}
} Material;

typedef struct Vertex {
	float coord[3];
	Vertex(float x, float y, float z) {
		coord[0] = x;
		coord[1] = y;
		coord[2] = z;
	};
	void update(float x, float y, float z) {
		coord[0] = x;
		coord[1] = y;
		coord[2] = z;
	}
	float getX() {
		return coord[0];
	}
	float getY() {
		return coord[1];
	}
	float getZ() {
		return coord[2];
	}
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
	string material;
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

class ObjSpec {
private:
	Mesh *mesh;
	string mtllibFileName;
public:
	ObjSpec(Mesh *m, string mtl) {
		mesh = m;
		mtllibFileName = mtl;
	}
	Mesh* getMesh() {
		return mesh;
	}
	string getMtllibFilename() {
		return mtllibFileName;
	}
	void setMesh(Mesh *m) {
		mesh = m;
	}
	void setMtllibFilename(string filename) {
		mtllibFileName = filename;
	}
};

#endif
