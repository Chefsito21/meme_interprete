# MemeScript Interpreter

Este proyecto es un intérprete para un lenguaje de scripting personalizado diseñado para generar memes, desarrollado como parte de una asignatura de introducción a los compiladores.([GitHub][1])

## Descripción

El sistema consta de dos fases principales:

1. **Análisis léxico (`lexer`)**: Lee archivos `.meme` y produce una lista de tokens válidos.
2. **Análisis sintáctico (`parser`)**: Valida la secuencia de tokens según una gramática BNF, detecta errores y produce una salida optimizada.

## Requisitos

* C++17 o superior
* Make([GitHub][2])

## Instalación

1. Clona el repositorio:

   ```bash
   git clone https://github.com/Chefsito21/meme_interprete.git
   ```



2. Navega al directorio del proyecto:

   ```bash
   cd meme_interprete/build
   ```



3. Compila el proyecto:

   ```bash
   make
   ```



## Uso

Una vez compilado, puedes ejecutar el intérprete proporcionando un archivo `.meme` como entrada:

```bash
./bin/main path/to/archivo.meme
```



El intérprete procesará el archivo, realizando el análisis léxico y sintáctico, y generará la salida correspondiente.

## Estructura del Proyecto

* `src/`: Contiene el código fuente principal del intérprete.
* `include/`: Archivos de encabezado (.h) utilizados en el proyecto.
* `lib/`: Bibliotecas necesarias para el funcionamiento del intérprete.
* `samples/`: Ejemplos de archivos `.meme` para probar el intérprete.
* `tools/`: Herramientas auxiliares utilizadas durante el desarrollo.
* `build/`: Archivos generados durante el proceso de compilación.
