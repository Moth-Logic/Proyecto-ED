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

## Características

-  **Mazo completo de Spider**: 104 cartas (8 copias de cada valor) barajadas con Fisher-Yates.
-  **10 columnas y 5 repartos**, con volteo automático de la carta superior.
-  **Movimientos en escalera**: se mueven grupos de cartas consecutivas y boca arriba.
-  **Detección automática** de escaleras completas K → A, que se retiran de la mesa.
-  **Deshacer y rehacer** sin límite mientras la partida está activa, mediante instantáneas del estado.
-  **Validación de entradas** con mensajes claros para columnas, cantidades y jugadas inválidas.
-  **Interfaz de consola en UTF-8** con cartas dibujadas, palos y columnas alineadas.

## Cómo jugar

El objetivo es **completar las 8 escaleras** del mismo palo, desde el Rey hasta el As, para eliminarlas del tablero.

Al inicio de la partida se reparten 54 cartas: las primeras 4 columnas reciben 6 cartas y las 6 restantes reciben 5. La carta superior de cada columna queda boca arriba.

### Controles

| Contexto | Tecla | Acción |
| :--- | :---: | :--- |
| Menú principal | `1` / `2` | Elegir juego (la opción 1 está en desarrollo) |
| Fin de partida | `S` / `N` | Jugar otra partida o salir |
| Partida | `1` | Mover cartas |
| Partida | `2` | Repartir una carta en cada columna |
| Partida | `3` | Deshacer la última jugada |
| Partida | `4` | Rehacer la jugada deshecha |
| Partida | `5` | Abandonar la partida |
| Entrada de mover | `Q` | Cancelar y volver al menú de la partida |

Para mover cartas se pide la **columna de origen**, la **columna de destino** y la **cantidad de cartas**. Las columnas se numeran del **1 al 10**.

### Reglas implementadas

- Solo se pueden mover grupos que formen una escalera descendente y estén boca arriba.
- La carta base del grupo debe encajar sobre una carta de valor inmediatamente mayor en la columna destino, o sobre una columna vacía.
- No se puede repartir si alguna columna está vacía.
- Al quedar una columna sin cartas, la nueva carta superior se voltea automáticamente.

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

## Estructura del proyecto

```text
Ruby-Solitaire/
├── images/
│   └── RUBY SOLITAIRE.png       # Logo del proyecto
├── Ruby Solitaire/
│   ├── Headers/
│   │   ├── Carta.h               # Valor, palo y visibilidad de una carta
│   │   ├── Mazo.h                # Barajado, reparto y cartas restantes
│   │   ├── PilaCartaSpider.h     # Columna de la mesa y dibujo de cartas
│   │   ├── JuegoSpider.h         # Reglas, turnos e historial de la partida
│   │   ├── EstadoSpider.h        # Instantánea para deshacer y rehacer
│   │   ├── Lista.h               # Lista enlazada genérica
│   │   ├── Pila.h                # Pila genérica
│   │   └── Node.h                # Nodo de la lista enlazada
│   ├── Source/
│   │   └── Ruby Solitaire.cpp    # Menú, lectura de entradas y ciclo de juego
│   └── Ruby Solitaire.vcxproj    # Proyecto de Visual Studio
└── README.md
```

## Diseño de la implementación

| Componente | Responsabilidad |
| :--- | :--- |
| `Carta` | Guarda valor, palo y si está boca arriba; genera su representación textual. |
| `Mazo` | Llena y baraja 104 cartas, controla los 5 repartos y entrega cartas al juego. |
| `PilaCartaSpider` | Representa una columna, valida escaleras y dibuja las cartas en consola. |
| `JuegoSpider` | Aplica las reglas, administra los repartos y coordina deshacer/rehacer. |
| `Lista` y `Pila` | Sustituyen a los contenedores de la STL en la lógica del juego. |
| `EstadoSpider` | Almacena una copia del tablero para el historial de jugadas. |

El controlador `JuegoSpider` mantiene **dos pilas de instantáneas**: una para deshacer y otra para rehacer. Cada movimiento o reparto guarda el estado anterior; al deshacer se restaura y se conserva una copia para poder rehacerlo. Cualquier jugada nueva descarta el historial de rehacer.

## Estado del proyecto

- [x] Solitario Spider completo: reparto, movimientos, repartos, deshacer y rehacer.
- [x] Interfaz de consola con validación de entradas.
- [ ] Segundo juego del menú (opción 1), actualmente en desarrollo.

## Integrantes

| Integrante |
| :--- |
| Julián Solórzano |
| Andrey Morales |
| Alexei Quesada |
| Maricruz Vásquez |

---

<p align="center">
  Proyecto académico de estructuras de datos · Hecho con C++ y ♠️
</p>
