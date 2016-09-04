#ifndef OBJREADER_H
#define OBJREADER_H

#include <string.h>
#include <fstream>
#include <sstream>

using namespace std;

Mesh* readObjFile(string filename) {
	ifstream file(filename);
	if (file.fail()) {
		return 0;
	}
	else {
		string line;
		while (getline(file, line)) {
			printf("%s", line);
		}
		return 0;
	}
}

#endif
