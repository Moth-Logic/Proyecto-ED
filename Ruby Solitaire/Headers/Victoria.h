#ifndef VICTORIA_H
#define VICTORIA_H

#include <cctype>
#include <cmath>
#include <random>
#include <string>
#include <vector>
#include "Consola.h"
#include "Lienzo.h"

const int VICTORIA_ANCHO = 118;
const int VICTORIA_ALTO = 30;

// Letras grandes de 5 filas para el mensaje final.
inline std::vector<std::string> glifo(char c) {
	switch (c) {
	case 'Y': return { "█   █", " █ █ ", "  █  ", "  █  ", "  █  " };
	case 'O': return { " ███ ", "█   █", "█   █", "█   █", " ███ " };
	case 'U': return { "█   █", "█   █", "█   █", "█   █", " ███ " };
	case 'W': return { "█   █", "█   █", "█ █ █", "██ ██", "█   █" };
	case 'I': return { "█████", "  █  ", "  █  ", "  █  ", "█████" };
	case 'N': return { "█   █", "██  █", "█ █ █", "█  ██", "█   █" };
	case '.': return { "  ", "  ", "  ", "  ", "██" };
	default:  return { "     ", "     ", "     ", "     ", "     " };
	}
}

// Ancho visible del texto en letras grandes (una columna de separación entre letras).
inline int anchoGrande(const std::string& texto) {
	int ancho = 0;
	for (char c : texto)
		ancho += (int)Lienzo::simbolos(glifo(c)[0]).size() + 1;
	return ancho > 0 ? ancho - 1 : 0;
}

// Posición horizontal para centrar un texto normal en el lienzo de victoria.
inline int centrar(const std::string& texto) {
	return (VICTORIA_ANCHO - (int)Lienzo::simbolos(texto).size()) / 2;
}

// Redondea una coordenada decimal a celda.
inline int redondear(float v) { return (int)std::floor(v + 0.5f); }

// Etiqueta corta de una carta (A, 2 ... 10, J, Q, K) entre corchetes.
inline std::string etiquetaCarta(int valor) {
	static const char* nombres[] = { "", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
	std::string n = nombres[valor];
	if (n.size() < 2) n += " ";
	return "[" + n + "]";
}

// Escribe el texto en letras grandes. 'ruido' (0 a 100) introduce desplazamientos y bloques para el efecto glitch.
inline void textoGrande(Lienzo& L, const std::string& texto, int x0, int y0, Color color, int ruido, std::mt19937& gen) {
	std::vector<std::vector<std::string>> filas(5);
	for (char c : texto) {
		std::vector<std::string> g = glifo(c);
		for (int f = 0; f < 5; f++) {
			for (const std::string& s : Lienzo::simbolos(g[f]))
				filas[f].push_back(s);
			filas[f].push_back(" ");
		}
	}

	std::uniform_int_distribution<int> azar(0, 99);
	std::uniform_int_distribution<int> desplazamiento(-2, 2);
	std::uniform_int_distribution<int> indice(0, 3);
	const char* bloques[] = { "░", "▒", "▓", " " };

	for (int f = 0; f < 5; f++) {
		int corrimiento = (ruido > 0 && azar(gen) < ruido) ? desplazamiento(gen) : 0;
		for (size_t c = 0; c < filas[f].size(); c++) {
			std::string s = filas[f][c];
			if (ruido > 0 && azar(gen) < ruido)
				s = bloques[indice(gen)];
			L.poner(x0 + (int)c + corrimiento, y0 + f, s, color);
		}
	}
}

// Una carta del mazo reconstruido. En la explosión tiene posición y velocidad.
struct Pieza {
	float x;
	float y;
	float vx;
	float vy;
	int valor;
};

// Secuencia de victoria en tres etapas: el mazo se reconstruye, explota y aparece "YOU WIN.".
// Devuelve true si el jugador elige jugar otra partida, false si quiere volver al menú.
// Las teclas que se pulsan durante la animación se descartan.
inline bool reproducirVictoria(const std::string& resumen) {
	std::mt19937 gen(std::random_device{}());
	const int PASO = 16;   // milisegundos por cuadro
	limpiarPantalla();

	// Etapa 1: las 104 cartas aparecen una por una formando el mazo (8 copias de cada valor).
	std::vector<Pieza> piezas;
	const int GX = (VICTORIA_ANCHO - 13 * 5) / 2;
	const int GY = 9;
	for (int copia = 0; copia < 8; copia++)
		for (int valor = 1; valor <= 13; valor++)
			piezas.push_back(Pieza{ (float)(GX + (valor - 1) * 5), (float)(GY + copia), 0.0f, 0.0f, valor });

	const std::string titulo1 = "REBUILDING THE DECK";
	for (size_t n = 0; n < piezas.size(); n++) {
		Lienzo L(VICTORIA_ANCHO, VICTORIA_ALTO);
		L.escribir(centrar(titulo1), 4, titulo1, Color::Gris);
		for (size_t k = 0; k <= n; k++)
			L.escribir((int)piezas[k].x, (int)piezas[k].y, etiquetaCarta(piezas[k].valor),
				k == n ? Color::Blanco : Color::Cian);
		dibujar(L.render());
		esperar(14);
	}
	esperar(450);

	// Etapa 2: explosión. Las cartas salen en todas direcciones girando y dejan chispas.
	std::uniform_real_distribution<float> angulo(0.0f, 6.2831853f);
	std::uniform_real_distribution<float> velocidad(18.0f, 60.0f);
	std::uniform_real_distribution<float> velocidadChispa(15.0f, 45.0f);
	for (Pieza& p : piezas) {
		float a = angulo(gen);
		float v = velocidad(gen);
		p.vx = std::cos(a) * v;
		p.vy = std::sin(a) * v * 0.5f;   // las celdas son más altas que anchas, se corrige el eje vertical
	}

	struct Chispa {
		float x;
		float y;
		float vx;
		float vy;
		Color color;
	};
	const Color colores[] = { Color::Rojo, Color::Amarillo, Color::Magenta, Color::Cian };
	const char* simbolosChispa[] = { "✦", "*", "·", "+" };
	const float cx = (float)(VICTORIA_ANCHO / 2);
	const float cy = 14.0f;
	std::vector<Chispa> chispas;
	for (int i = 0; i < 160; i++) {
		float a = angulo(gen);
		float v = velocidadChispa(gen);
		chispas.push_back(Chispa{ cx, cy, std::cos(a) * v, std::sin(a) * v * 0.5f, colores[i % 4] });
	}

	const int DURACION = 1500;
	const float dt = PASO / 1000.0f;
	for (int t = 0; t < DURACION; t += PASO) {
		for (Pieza& p : piezas) {
			p.x += p.vx * dt;
			p.y += p.vy * dt;
			p.vx *= 0.985f;
			p.vy *= 0.985f;
		}
		for (Chispa& c : chispas) {
			c.x += c.vx * dt;
			c.y += c.vy * dt;
			c.vx *= 0.97f;
			c.vy *= 0.97f;
		}

		Lienzo L(VICTORIA_ANCHO, VICTORIA_ALTO);
		for (size_t i = 0; i < chispas.size(); i++)
			L.poner(redondear(chispas[i].x), redondear(chispas[i].y), simbolosChispa[i % 4], chispas[i].color);

		// Cada carta cambia de aspecto cada tanto para simular que gira.
		for (size_t i = 0; i < piezas.size(); i++) {
			const Pieza& p = piezas[i];
			int fase = (t / 110 + (int)i) % 4;
			std::string texto;
			if (fase == 0) texto = etiquetaCarta(p.valor);
			else if (fase == 1) texto = "▮▮";
			else if (fase == 2) texto = "[ ]";
			else texto = "▯▯";
			L.escribir(redondear(p.x), redondear(p.y), texto, colores[p.valor % 4]);
		}

		dibujar(L.render());
		esperar(PASO);
	}

	// Etapa 3: "YOU WIN." aparece con efecto glitch y después se estabiliza.
	const std::string mensaje = "YOU WIN.";
	const int x0 = (VICTORIA_ANCHO - anchoGrande(mensaje)) / 2;
	const int y0 = 9;
	for (int f = 0; f < 24; f++) {
		int ruido = 60 - f * 3;
		if (ruido < 0) ruido = 0;
		Lienzo L(VICTORIA_ANCHO, VICTORIA_ALTO);
		textoGrande(L, mensaje, x0, y0, Color::Verde, ruido, gen);
		dibujar(L.render());
		esperar(30);
	}

	Lienzo L(VICTORIA_ANCHO, VICTORIA_ALTO);
	textoGrande(L, mensaje, x0, y0, Color::Verde, 0, gen);
	L.escribir(centrar(resumen), y0 + 7, resumen, Color::Gris);
	const std::string opciones = "[N] New game      [M] Main menu";
	L.escribir(centrar(opciones), y0 + 9, opciones, Color::Cian);
	dibujar(L.render());

	// Espera la elección del jugador. Las teclas pulsadas durante la animación se descartan antes.
	vaciarTeclado();
	while (true) {
		int c = _getch();
		if (c == 0 || c == 0xE0) {
			_getch();
			continue;
		}
		c = std::toupper(c);
		if (c == 'N') return true;
		if (c == 'M' || c == 27) return false;
	}
}

#endif
