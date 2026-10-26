<p align="center">
  <img src="images/RUBY%20SOLITAIRE.png" alt="Logo de Ruby Solitaire" width="480">
</p>

<h1 align="center">Ruby Solitaire</h1>

<p align="center">
  <b>Versión 1.1.0</b>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/versi%C3%B3n-1.1.0-c41e3a" alt="Versión 1.1.0">
  <img src="https://img.shields.io/badge/plataforma-Windows%2010%2F11-0078D6?logo=windows&logoColor=white" alt="Windows">
  <img src="https://img.shields.io/badge/interfaz-consola-2E3440" alt="Consola">
</p>

---

## Descripción

**Ruby Solitaire** es un juego de solitario para Windows que se juega en una ventana de consola.
Incluye dos juegos completos con las reglas clásicas: **Golf Solitaire** y **Spider Solitaire**.
En ambos puedes mover cartas, repartir del mazo, y **deshacer** o **rehacer** tus jugadas.

No necesitas instalar nada: se descarga, se descomprime y se juega.

## Juegos disponibles

| Juego | Descripción |
| --- | --- |
| **Golf Solitaire** | Siete columnas y un mazo. Coloca cartas sobre el descarte sin importar el palo, siempre que el valor sea una arriba o una abajo del valor del descarte (el As también es contiguo al Rey). Ganas cuando vacías las siete columnas. |
| **Spider Solitaire** | Diez columnas y 104 cartas. Mueve secuencias de cartas para formar escaleras del mismo palo. Gana cuando completas las ocho escaleras. |

## Descarga

Descarga el archivo **`Ruby-Solitaire-v1.1.0-win64.zip`** desde la sección
[**Releases**](https://github.com/Moth-Logic/Ruby-Solitaire/releases) del repositorio.

Dentro del archivo comprimido encontrarás:

```
Ruby-Solitaire-v1.1.0-win64/
├── Ruby Solitaire.exe
├── msvcp140.dll
├── vcruntime140.dll
└── vcruntime140_1.dll
```

> Los tres archivos `.dll` son necesarios. Deben permanecer **en la misma carpeta** que el `.exe`.

## Cómo ejecutar

1. Descarga `Ruby-Solitaire-v1.1.0-win64.zip`.
2. Haz clic derecho sobre el archivo y elige **Extraer todo…**.
3. Abre la carpeta `Ruby-Solitaire-v1.1.0-win64`.
4. Haz doble clic en **`Ruby Solitaire.exe`**.
5. Elige el juego que quieres jugar: `1` para Golf Solitaire o `2` para Spider Solitaire.

Si Windows muestra un aviso de seguridad (SmartScreen), pulsa **Más información** y luego
**Ejecutar de todas formas**. El aviso aparece porque el programa no está firmado digitalmente.

## Cómo jugar

Dentro de una partida, el menú siempre muestra las opciones disponibles:

| Opción | Acción |
| --- | --- |
| `1` | **Mover** una carta o una secuencia de cartas |
| `2` | **Repartir** del mazo |
| `3` | **Deshacer** la última jugada |
| `4` | **Rehacer** la jugada deshecha |
| `5` | **Abandonar** la partida y volver al menú principal |

Escribe el número de la opción y presiona **Enter**. Cuando el juego pida una columna,
escribe su número. Presiona **Q** para cancelar y volver atrás.

## Requisitos

- **Windows 10 o Windows 11 de 64 bits.**
- No requiere instalación ni permisos de administrador.
- No requiere tener Visual Studio instalado.

## Créditos

Proyecto desarrollado por Julián Solórzano, Andrey Morales, Alexei Quesada y Maricruz Vásquez.
