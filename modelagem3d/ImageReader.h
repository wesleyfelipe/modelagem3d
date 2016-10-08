#ifndef IMAGE_READER_H
#define IMAGE_READER_H

#include <fstream>
#include "Image.h"

class ImageReader {
private:
//metodo para ler arquivos de imagem ppm p3 (modo texto)
	Image* lerImagem(ifstream &arq) {

		char tipo[3];
		arq.getline(tipo, 3);

		char temp[256];
		arq.getline(temp, 256);

		int w = 0;
		arq >> w;
		int h = 0;
		arq >> h;

		Image *image = new Image(w, h);
		int maxvalue;
		arq >> maxvalue;

		int r = 0;
		int g = 0;
		int b = 0;

		int rgb = 0;

		for (int y = h - 1; y >= 0; y--) {
			for (int x = w - 1; x >= 0; x--) {
				arq >> b >> g >> r;
				rgb = (r << 16) | (g << 8) | b;
				image->setRGB(x, y, rgb);
			}
		}
		arq.close();

		return image;
	}

public:
	Image* lerArquivo(string path) {
		Image *image = 0;
		ifstream file(path);
		if (!file.fail()) {
			image = lerImagem(file);
		}
		file.close();
		return image;
	}
};

#endif
