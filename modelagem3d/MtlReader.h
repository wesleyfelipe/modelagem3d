#ifndef MTLREADER_H
#define MTLREADER_H

#include <string.h>
#include <fstream>
#include <sstream>

using namespace std;

class MltReader {
private:
	char comentarioChar = '#';
	string newmtlStringInicial = "newmtl";
	string kaStringInicial = "Ka";
	string kdStringInicial = "Kd";
	string ksStringInicial = "Ks";
	string nsStringInicial = "Ns";
	string mapKdStringInicial = "map_Kd";

	void fillMaterialVector(ifstream *file, vector<Material*> *vector) {

	}

public:
	void readMtlFile(string filename, vector<Material*> *materials) {
		ifstream file(filename);
		if (!file.fail()) {
			fillMaterialVector(&file, materials);
		}
		file.close();
	}
};

#endif
