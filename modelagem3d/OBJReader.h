#ifndef OBJREADER_H
#define OBJREADER_H

#include <string.h>
#include <fstream>
#include <sstream>

using namespace std;

class ObjReader {
private:
	char comentarioChar = '#';
	string verticeStringInicial = "v";
	string normalStringInicial = "vn";
	string texturaStringInicial = "vt";
	string faceStringInicial = "f";
	string groupStringInicial = "g";
	string usemtlStringInicial = "usemtl";
	string mtllibStringIncial = "mtllib";

	vector<string> split(string str, char delimiter) {
		vector<string> internal;
		stringstream ss(str);
		string tok;
		while (getline(ss, tok, delimiter)) {
			internal.push_back(tok);
		}
		return internal;
	}

	bool isComentario(vector<string> sLine) {
		return sLine[0][0] == comentarioChar;
	}

	bool isVertice(vector<string> sLine) {
		return sLine[0] == verticeStringInicial;
	}

	bool isNormal(vector<string> sLine) {
		return sLine[0] == normalStringInicial;
	}

	bool isTextura(vector<string> sLine) {
		return sLine[0] == texturaStringInicial;
	}

	bool isMtlLib(vector<string> sLine) {
		return sLine[0] == mtllibStringIncial;
	}

	bool isMaterialRef(vector<string> sLine) {
		return sLine[0] == usemtlStringInicial;
	}

	bool isFace(vector<string> sLine) {
		return sLine[0] == faceStringInicial;
	}

	bool isGroup(vector<string> sLine) {
		return sLine[0] == groupStringInicial;
	}

	Vertex* buildVertex(vector<string> sLine) {
		return new Vertex(atof(sLine[1].c_str()), atof(sLine[2].c_str()), atof(sLine[3].c_str()));
	}

	TextureMapping* buildTexture(vector<string> sLine) {
		return new TextureMapping(atof(sLine[1].c_str()), atof(sLine[2].c_str()));
	}

	Face* buildFace(vector<string> sLine) {
		Face *face = new Face();
		for (string text : sLine) {
			if (text != faceStringInicial) {
				vector<string> vFace = split(text, '/');
				face->vertex.push_back(atoi(vFace[0].c_str()) - 1);
				if (vFace.size() > 1) {
					if (!vFace[1].empty()) {
						face->mappings.push_back(atoi(vFace[1].c_str()) - 1);
					}
					if (vFace.size() > 2) {
						face->normals.push_back(atoi(vFace[2].c_str()) - 1);
					}
				}
			}
		}
		return face;
	}

	ObjSpec* buildMesh(ifstream *file) {
		string line;
		Mesh *mesh = new Mesh();
		Group *group = new Group();
		string mtlib = "";
		while (getline(*file, line)) {
			vector<string> sLine = split(line, ' ');
			if (sLine.size() > 0 && !line.empty() && !isComentario(sLine)) {
				if (isVertice(sLine)) {
					mesh->allVertex.push_back(buildVertex(sLine));
				}
				else if (isNormal(sLine)) {
					mesh->allNormals.push_back(buildVertex(sLine));
				}
				else if (isTextura(sLine)) {
					mesh->allMappings.push_back(buildTexture(sLine));
				}
				else if (isFace(sLine)) {
					group->groupFaces.push_back(buildFace(sLine));
				}
				else if (isGroup(sLine)) {
					if (group->name.empty()) {
						group->name = sLine[1];
					}
					else {
						mesh->groups.push_back(group);
						group = new Group();
						group->name = sLine[1];
					}
				}
				else if (isMaterialRef(sLine)) {
					group->material = sLine[1];
				}
				else if (isMtlLib(sLine)) {
					mtlib = sLine[1];
				}
				else {
					//TODO
				}
			}
		}
		mesh->groups.push_back(group);
		ObjSpec *spec = new ObjSpec(mesh, mtlib);
		return spec;
	}
	
public:
	ObjSpec* readObjFile(string filename) {
		ifstream file(filename);
		if (file.fail()) {
			file.close();
			return 0;
		}
		else {
			ObjSpec *spec = buildMesh(&file);
			if (spec->getMtllibFilename() == "") {
				spec->setMtllibFilename(split(filename, '.')[1] + ".mtl");
			}
			file.close();
			return spec;
		}
	}
};





#endif
