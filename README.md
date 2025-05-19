# 🧠 MemeScript Interpreter

Este proyecto es un **intérprete para un lenguaje de scripting personalizado para generar memes**, desarrollado como parte de una asignatura de compiladores.

## 📚 Descripción

El sistema consta de dos fases principales:

1. **Análisis léxico (`lexer`)**: lee archivos `.meme` y produce una lista de tokens válidos.
2. **Análisis sintáctico (`parser`)**: valida la secuencia de tokens según una gramática BNF, detecta errores y produce una salida optimizada.

## 📁 Estructura del Proyecto

.
├── include/              # Headers (.hpp)
│   ├── lexer.hpp
│   └── parser.hpp
├── src/                  # Implementación principal
│   ├── lexer.cpp
│   └── parser.cpp
├── samples/              # Archivos de entrada de ejemplo (.meme)
│   └── ejemplo.meme
├── test/                 # Archivos de prueba
├── Makefile              # Compilación automatizada
├── bnf.txt               # Gramática del lenguaje en BNF
├── README.md             # Este archivo

## 🛠️ Requisitos

- C++17 o superior
- Make

## 🔧 Compilación

Para compilar el proyecto completo:

```bash
make all

Para compilar sólo el lexer o parser:

make lexer
make parser

🚀 Uso

1. Ejecutar el lexer

./lexer samples/ejemplo.meme

Esto generará dos archivos:
	•	meme.out: salida formal con tokens (⟨tipo, valor⟩)
	•	meme.txt: salida legible para humanos

2. Ejecutar el parser

./parser

Esto:
	•	Leerá meme.out
	•	Verificará la sintaxis
	•	Mostrará errores si los hay
	•	Escribirá un archivo optimized.out si el análisis es exitoso

🧾 Ejemplo de Entrada

Contenido de samples/ejemplo.meme:

top "Texto superior"
bottom "Texto inferior"
imagen 1

Tokens generados (meme.out):

⟨comando, top⟩
⟨const, "Texto superior"⟩
⟨comando, bottom⟩
⟨const, "Texto inferior"⟩
⟨comando, imagen⟩
⟨num, 1⟩

🔍 Gramática (BNF)

Archivo bnf.txt:

<program> ::= <instruction_list>
<instruction_list> ::= <instruction> <instruction_list> | ε
<instruction> ::= <command> <args>
<command> ::= ⟨comando, valor⟩
<args> ::= <const> <args> | <num> <args> | ε
<const> ::= ⟨const, valor⟩
<num> ::= ⟨num, valor⟩

🧪 Pruebas

Puedes colocar archivos .meme de prueba en la carpeta samples/ y ejecutar:

make test

(Requiere que configures pruebas automatizadas si lo deseas)

📌 Autor

Proyecto desarrollado por [Tu Nombre] para la materia Introducción a los Compiladores – 2025.

📄 Licencia

Este proyecto es de uso académico. Puedes adaptarlo o extenderlo libremente para fines educativos.

---

¿Te gustaría que también incluya ejemplos de errores sintácticos comunes o cómo agregar nuevas instrucciones al lenguaje?