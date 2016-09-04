#ifndef OBJREADER_H
#define OBJREADER_H

#include <string.h>
#include <fstream>
#include <sstream>

using namespace std;

char comentarioChar = '#';

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

Mesh* buildMesh(ifstream *file) {
	string line;
	while (getline(*file, line)) {
		vector<string> sLine = split(line, ' ');
		if (sLine.size() > 0) {
			if (!line.empty() && !isComentario(sLine)) {
				for (string text : sLine) {
					cout << text;
					cout << "\n";
				}
			}
		}
	}
	return 0;
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
