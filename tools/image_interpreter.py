#!/usr/bin/env python3
import cv2
import re
import os

script_dir = os.path.dirname(os.path.abspath(__file__))

def cargar_imagen(nombre_archivo):
    ruta_completa = os.path.join(script_dir, nombre_archivo)
    imagen = cv2.imread(ruta_completa)
    if imagen is None:
        print(f"[Error] No se pudo cargar la imagen: {nombre_archivo}")
    return imagen

# Configuración global
current_image = None
font_color = (0, 0, 0)
font_size = 1.0
output_path = "output.jpg"

# Colores predefinidos (nombre → BGR)
named_colors = {
    "NEGRO": (0, 0, 0),
    "BLANCO": (255, 255, 255),
    "ROJO": (0, 0, 255),
    "AZUL": (255, 0, 0),
    "VERDE": (0, 255, 0)
}

positions = {
    "ARRIBA": lambda w, h: (int(w * 0.05), int(h * 0.1)),
    "ABAJO": lambda w, h: (int(w * 0.05), int(h * 0.9)),
    "CENTRO": lambda w, h: (int(w * 0.3), int(h * 0.5)),
    "IZQUIERDA_SUPERIOR": lambda w, h: (int(w * 0.01), int(h * 0.05)),
    "DERECHA_INFERIOR": lambda w, h: (int(w * 0.7), int(h * 0.95))
}


def hex_to_bgr(hex_color):
    hex_color = hex_color.lstrip('#')
    return tuple(int(hex_color[i:i+2], 16) for i in (4, 2, 0))

def interpretar_archivo(path):
    global current_image, font_color, font_size, output_path

    with open(path, 'r') as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("//"):
                continue

            if match := re.match(r'PON_IMAGEN\("([^"]+)"\)', line):
                ruta = match.group(1)
                current_image = cargar_imagen(ruta)
                if current_image is None:
                    print(ruta)
                    print(f"[Error] No se pudo cargar la imagen: {ruta}")
                else:
                    print(ruta)
                    print(f"[OK] Imagen cargada: {ruta}")

            elif match := re.match(r'CAMBIA_COLOR_TEXTO\(?["\']?([#A-Z0-9]+)["\']?\)?', line, re.IGNORECASE):
                color_str = match.group(1).upper()
                if color_str in named_colors:
                    font_color = named_colors[color_str]
                elif re.match(r'^#[0-9A-F]{6}$', color_str, re.IGNORECASE):
                    font_color = hex_to_bgr(color_str)
                else:
                    print(f"[Error] Color no reconocido: {color_str}")

            elif match := re.match(r'CAMBIA_TAMAÑO_TEXTO\((\d+)\)', line):
                size = int(match.group(1))
                if 10 <= size <= 30:
                    font_size = size / 25.0
                else:
                    print(f"[Error] Tamaño de texto inválido: {size}")

            elif match := re.match(r'ESCRIBE\("([^"]+)",\s*(\w+)\)', line):
                texto, pos = match.group(1), match.group(2).upper()
                if current_image is not None and pos in positions:
                    h, w = current_image.shape[:2]
                    coordenadas = positions[pos](w, h)
                    cv2.putText(current_image, texto, coordenadas, cv2.FONT_HERSHEY_SIMPLEX, font_size, font_color, 2)
                else:
                    print(f"[Error] Posición inválida o imagen no cargada: {pos}")

            elif match := re.match(r'ESCRIBE_MULTILINEA\("([^"]+)",\s*"([^"]+)",\s*(\w+)\)', line):
                texto1, texto2, pos = match.group(1), match.group(2), match.group(3).upper()
                if current_image is not None and pos in positions:
                    h, w = current_image.shape[:2]
                    x, y = positions[pos](w, h)
                    spacing = int(30 * font_size)
                    cv2.putText(current_image, texto1, (x, y), cv2.FONT_HERSHEY_SIMPLEX, font_size, font_color, 2)
                    cv2.putText(current_image, texto2, (x, y + spacing), cv2.FONT_HERSHEY_SIMPLEX, font_size, font_color, 2)
                else:
                    print(f"[Error] Posición inválida o imagen no cargada: {pos}")

            elif match := re.match(r'GUARDA_COMO\("([^"]+)"\)', line):
                output_path = match.group(1)
                if current_image is not None:
                    cv2.imwrite(output_path, current_image)
                    print(f"[OK] Imagen guardada como {output_path}")
                else:
                    print("[Error] No hay imagen cargada para guardar.")

            else:
                print(f"[Advertencia] Instrucción no reconocida: {line}")

if __name__ == "__main__":
    interpretar_archivo("../samples/programa_optimizado.out")
