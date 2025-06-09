## Multiplicación de Matrices

![image](https://github.com/user-attachments/assets/39931cd0-8393-4ce7-bf8d-894a2bf9519a)


# Multiplicación de Matrices con MPI

# MPI Matrix Multiplication

Este proyecto implementa la multiplicación de matrices usando **MPI (Message Passing Interface)** en C, dividiendo el trabajo entre varios procesos para mejorar el rendimiento en sistemas paralelos o distribuidos.

## 🧠 ¿Qué hace este programa?

Realiza la multiplicación de dos matrices cuadradas `A` y `B` de tamaño `N x N`, distribuyendo el cálculo entre un **proceso maestro** y varios **procesos esclavos**.

El resultado es una matriz `C = A * B`.

---

## ⚙️ ¿Cómo funciona?

### 💻 Estructura del programa

- `rank 0`: Proceso maestro (orquesta el trabajo)
- `rank 1 ... n`: Procesos esclavos (realizan productos punto)

El maestro:
1. Genera las matrices `A` y `B`.
2. Divide el trabajo en tareas: cada celda de la matriz `C[i][j]` es una **tarea**.
3. Envía a cada esclavo una tarea: la fila `i` de `A` y la columna `j` de `B`.
4. Recibe los resultados y los coloca en la posición correcta de `C`.
5. Cuando todas las tareas están hechas, envía una señal de parada a los esclavos.

Los esclavos:
1. Reciben una fila y una columna.
2. Calculan el producto punto: `C[i][j] = ∑ A[i][k] * B[k][j]`.
3. Envían el resultado al maestro.
4. Esperan más tareas, o terminan si reciben la señal de parada.

---

### 📦 Archivos

- `mat_mul.c`: Código fuente principal.

## 🧪 Pruebas realizadas

Se probaron múltiples casos para asegurar la correcta implementación. Uno de los más importantes:

### ✅ Multiplicación por la matriz identidad

Se generó una matriz `A` aleatoria y se multiplicó por la **matriz identidad `I`**, esperando que: A * I = A


El resultado fue correcto, lo cual valida que la implementación es funcional y conserva las propiedades de la multiplicación de matrices.

## Resultados

### Tiempo de Ejecución por Número de procesos
![image](https://github.com/user-attachments/assets/57221f27-806c-4e11-a89f-397c9ad301d7)

## Autor: Edgar Leonardo García Zavala

