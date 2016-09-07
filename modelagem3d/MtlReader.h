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

public:

};

#endif
