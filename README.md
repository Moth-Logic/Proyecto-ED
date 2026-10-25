<p align="center">
  <img src="images/RUBY%20SOLITAIRE.png" alt="Logo de Ruby Solitaire" width="480">
</p>

<h1 align="center">Ruby Solitaire</h1>

<p align="center">
  Un solitario Spider jugable por consola, escrito en C++ con estructuras de datos implementadas desde cero.
</p>

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-20-00599C?logo=cplusplus&logoColor=white" alt="C++20">
  <img src="https://img.shields.io/badge/Visual%20Studio-2026-5C2D91?logo=visualstudio&logoColor=white" alt="Visual Studio">
  <img src="https://img.shields.io/badge/Plataforma-Windows-0078D6?logo=windows&logoColor=white" alt="Windows">
  <img src="https://img.shields.io/badge/Interfaz-Consola-2E3440" alt="Consola">
</p>

---

## Descripción

**Ruby Solitaire** es un proyecto académico que implementa el clásico **Solitario Spider** para jugar desde la terminal. El juego reparte las 104 cartas en 10 columnas, valida cada movimiento según las reglas y permite deshacer y rehacer jugadas durante la partida.

Todo el manejo de cartas, columnas, mazos e historial se apoya en estructuras de datos lineales propias (listas enlazadas, pilas y nodos), sin usar los contenedores de la biblioteca estándar para la lógica del juego.

## Requisitos

- **Windows 10 o superior** (el programa usa `windows.h` y `system("cls")`).
- **Visual Studio 2026** con la carga de trabajo *Desarrollo para el escritorio con C++*, o una versión compatible con el toolset `v145`.
- **C++20**.

> Si usas una versión anterior de Visual Studio, cambia el conjunto de herramientas del proyecto (`v145`) por el que tengas instalado, por ejemplo `v143`.

## Compilación y ejecución

1. Clona el repositorio:

   ```bash
   git clone https://github.com/Moth-Logic/Ruby-Solitaire.git
   cd Ruby-Solitaire
   ```

2. Abre `Ruby Solitaire/Ruby Solitaire.slnx` en Visual Studio.
3. Selecciona la configuración **Release** y la plataforma **x64**.
4. Presiona **Ctrl + F5** para compilar y ejecutar.

También puedes compilar desde la línea de comandos con MSBuild:

```bash
msbuild "Ruby Solitaire/Ruby Solitaire.slnx" /p:Configuration=Release /p:Platform=x64
```

## Estado del proyecto

- [x] Solitario Spider completo: reparto, movimientos, repartos, deshacer y rehacer.
- [x] Interfaz de consola con validación de entradas.
- [x] Solitario Golf completo: reparto, movimientos, repartos, deshacer y rehacer.
- [x] Segundo juego del menú (opción 1) con tablero de texto básico.

## Integrantes

| Julián Solórzano |
| Andrey Morales |
| Alexei Quesada |
| Maricruz Vásquez |