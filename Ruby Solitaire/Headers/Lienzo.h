#ifndef LIENZO_H
#define LIENZO_H

#include <string>
#include <vector>

// Colores del juego. Cada uno se convierte en un código ANSI al dibujar.
enum class Color : unsigned char { Normal, Rojo, Verde, Amarillo, Magenta, Cian, Gris, Blanco };

// Una celda del lienzo: el símbolo que se ve (puede ocupar varios bytes UTF-8) y su color.
struct Celda {
	std::string simbolo = " ";
	Color color = Color::Normal;
};

// Superficie de dibujo para una pantalla completa. Se pinta por celdas y al final
// se convierte en un solo texto con los códigos de color, así no hay parpadeo.
class Lienzo {
private:
	int ancho;
	int alto;
	std::vector<Celda> celdas;

	// Devuelve el código ANSI que activa un color.
	static const char* codigo(Color c) {
		switch (c) {
		case Color::Rojo:     return "\x1b[91m";
		case Color::Verde:    return "\x1b[92m";
		case Color::Amarillo: return "\x1b[93m";
		case Color::Magenta:  return "\x1b[95m";
		case Color::Cian:     return "\x1b[96m";
		case Color::Gris:     return "\x1b[90m";
		case Color::Blanco:   return "\x1b[97m";
		default:              return "\x1b[0m";
		}
	}

	// Cuántos bytes ocupa el carácter UTF-8 que empieza con este byte.
	static int bytesDelCaracter(unsigned char c) {
		if (c >= 0xF0) return 4;
		if (c >= 0xE0) return 3;
		if (c >= 0xC0) return 2;
		return 1;
	}

public:
	Lienzo(int ancho, int alto) : ancho(ancho), alto(alto), celdas(ancho * alto) {}

	int getAncho() const { return ancho; }
	int getAlto() const { return alto; }

	// Divide un texto UTF-8 en símbolos completos, para contar columnas bien con acentos y cajas.
	static std::vector<std::string> simbolos(const std::string& texto) {
		std::vector<std::string> salida;
		size_t i = 0;
		while (i < texto.size()) {
			int largo = bytesDelCaracter(static_cast<unsigned char>(texto[i]));
			salida.push_back(texto.substr(i, largo));
			i += largo;
		}
		return salida;
	}

	// Pone un símbolo en una celda. Las posiciones fuera del lienzo se ignoran.
	void poner(int x, int y, const std::string& simbolo, Color color = Color::Normal) {
		if (x < 0 || y < 0 || x >= ancho || y >= alto) return;
		celdas[y * ancho + x] = Celda{ simbolo, color };
	}

	// Escribe una línea de texto. Entiende los códigos de color de las cartas (\033[45m y \033[0m).
	void escribir(int x, int y, const std::string& texto, Color colorBase = Color::Normal) {
		Color actual = colorBase;
		size_t i = 0;
		int columna = x;
		while (i < texto.size()) {
			if (texto[i] == '\x1b') {
				size_t fin = texto.find('m', i);
				if (fin == std::string::npos) break;
				std::string cod = texto.substr(i + 2, fin - (i + 2));
				actual = (cod == "45") ? Color::Magenta : colorBase;
				i = fin + 1;
				continue;
			}
			int largo = bytesDelCaracter(static_cast<unsigned char>(texto[i]));
			poner(columna, y, texto.substr(i, largo), actual);
			columna++;
			i += largo;
		}
	}

	// Convierte el lienzo en texto con códigos ANSI. Las filas van separadas por saltos de línea.
	std::string render() const {
		std::string salida;
		for (int y = 0; y < alto; y++) {
			Color actual = Color::Normal;
			for (int x = 0; x < ancho; x++) {
				const Celda& c = celdas[y * ancho + x];
				if (c.color != actual) {
					salida += codigo(c.color);
					actual = c.color;
				}
				salida += c.simbolo;
			}
			salida += "\x1b[0m";
			if (y < alto - 1) salida += "\n";
		}
		return salida;
	}
};

#endif
