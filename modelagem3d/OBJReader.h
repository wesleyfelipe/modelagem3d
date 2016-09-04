#ifndef OBJREADER_H
#define OBJREADER_H

#include <string.h>
#include <fstream>
#include <sstream>

using namespace std;

char comentarioChar = '#';
string verticeStringInicial = "v";
string normalStringInicial = "vn";
string texturaStringInicial = "vt";

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

Vertex* buildVertex(vector<string> sLine) {
	return new Vertex(atof(sLine[1].c_str()), atof(sLine[2].c_str()), atof(sLine[3].c_str()));
}

TextureMapping* buildTexture(vector<string> sLine) {
	return new TextureMapping(atof(sLine[1].c_str()), atof(sLine[2].c_str()));
}

Mesh* buildMesh(ifstream *file) {
	string line;
	Mesh *mesh = new Mesh();
	while (getline(*file, line)) {
		vector<string> sLine = split(line, ' ');
		if (sLine.size() > 0 && !line.empty() && !isComentario(sLine)) {
			if (isVertice(sLine)) {
				mesh->allVertex.push_back(buildVertex(sLine));
			} else if (isNormal(sLine)) {
				mesh->allNormals.push_back(buildVertex(sLine));
			}else if(isTextura(sLine)){
				mesh->allMappings.push_back(buildTexture(sLine));
			} else {
				/**
				for (string text : sLine) {
					cout << text;
					cout << "\n";
				}
				*/
			}
		}
	}
	return mesh;
}

Mesh* readObjFile(string filename) {
	ifstream file(filename);
	if (file.fail()) {
		return 0;
	} else {
		return buildMesh(&file);
	}
}

#endif
