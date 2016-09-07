#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <vector>

using namespace std;

class TextureMapping {
private:
	float coord[2];
public:
	TextureMapping(float x, float y) {
		coord[0] = x;
		coord[1] = y;
	}
	float* getCoord() {
		return coord;
	}
};

class Material {
private:
	string id;
	float *ka;
	float *kd;
	float *ks;
	float ns;
	string mapKd;
public:
	Material() {}
	void setKa(float *a) {
		ka = a;
	}
	void setKd(float *d) {
		kd = d;
	}
	void setKs(float *s) {
		ks = s;
	}
	void setNs(float n) {
		ns = n;
	}
	void setMapKd(string m) {
		mapKd = m;
	}
	void setId(string i) {
		id = i;
	}
	string getId() {
		return id;
	}
	float* getKa() {
		return ka;
	}
	float* getKs() {
		return ks;
	}
	float getNs() {
		return ns;
	}
	float* getKd() {
		return kd;
	}
	string getMapKd() {
		return mapKd;
	}
};

class Vertex {
private:
	float coord[3];
public:
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
	float* getCoord() {
		return coord;
	}
};

class Face {
private:
	vector<GLint> *vertex;
	vector<GLint> *normals;
	vector<GLint> *mappings;
public:
	Face(vector<GLint> *v, vector<GLint> *n, vector<GLint> *m) {
		vertex = v;
		normals = n;
		mappings = m;
	}
	Face() {
		vertex = new vector<GLint>();
		normals = new vector<GLint>();
		mappings = new vector<GLint>();
	}
	vector<GLint>* getVertex() {
		return vertex;
	}
	vector<GLint>* getNormals() {
		return normals;
	}
	vector<GLint>* getMappings() {
		return mappings;
	}
};

class Group {
private:
	string name;
	string material;
	vector<Face*> *groupFaces;
public:
	Group(vector<Face*> *f) {
		groupFaces = f;
	}
	Group() {
		groupFaces = new vector<Face*>();
	}
	vector<Face*>* getGroupFaces() {
		return groupFaces;
	}
	string getName() {
		return name;
	}
	void setName(string n) {
		name = n;
	}
	void setMaterial(string m) {
		material = m;
	}
};

class Mesh {
private:
	vector<Group*> *groups;
	vector<Vertex*> *allVertex;
	vector<Vertex*> *allNormals;
	vector<TextureMapping*> *allMappings;
public:
	Mesh(vector<Group*> *g, vector<Vertex*> *v, vector<Vertex*> *n, vector<TextureMapping*> *m) {
		groups = g;
		allVertex = v;
		allNormals = n;
		allMappings = m;
	}
	Mesh() {
		groups = new vector<Group*>();
		allVertex = new vector<Vertex*>();
		allNormals = new vector<Vertex*>();
		allMappings = new vector<TextureMapping*>();
	}
	vector<Group*>* getGroups() {
		return groups;
	}
	vector<Vertex*>* getAllVertex() {
		return allVertex;
	}
	vector<Vertex*>* getAllNormals() {
		return allNormals;
	}
	vector<TextureMapping*>* getAllMappings() {
		return allMappings;
	}
};

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
