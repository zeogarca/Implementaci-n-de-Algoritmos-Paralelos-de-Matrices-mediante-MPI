## Multiplicación de Matrices

![image](https://github.com/user-attachments/assets/39931cd0-8393-4ce7-bf8d-894a2bf9519a)


# Multiplicación de Matrices con MPI y OpenMP

## ¿Qué hace el programa?

Este proyecto realiza la multiplicación de dos matrices cuadradas usando programación paralela. Utiliza dos tecnologías:

- MPI (Message Passing Interface) para repartir el trabajo entre varios procesos núcleos.

- OpenMP para aprovechar varios hilos dentro de cada proceso.

La combinación de ambas permite que el programa sea rápido y eficiente, aprovechando al máximo los recursos disponibles.

## ¿Cómo está organizado el programa?

El programa está dividido en dos tipos de procesos:

- Maestro: es el que organiza todo. Crea las matrices A y B, reparte partes de ellas entre los demás procesos, y al final recoge los resultados.

- Esclavos: reciben sus partes de las matrices y hacen su parte del cálculo. Luego envían el resultado de vuelta al maestro.

## Pasos del programa

Inicio y lectura del tamaño de la matriz:

1. El usuario le dice al programa qué tamaño deben tener las matrices cuadradas (por ejemplo, 1000x1000).
2. Se inicializa MPI para que todos los procesos se preparen.
3. Creación y reparto de datos (por el maestro)
4. Se crean dos matrices aleatorias: A y B.
5. La matriz A se divide por filas, y la matriz B se divide por columnas.
6. Cada proceso recibe una parte de A y una parte de B, según lo que le toca.

Todos los procesos:

1. Cada proceso multiplica las filas que le tocaron de A por las columnas de B que le llegaron.

    - Para esto se usa OpenMP, que permite que dentro de cada proceso se usen varios hilos para hacer los cálculos más rápido.

Recolección de resultados:

1. Cada proceso genera un pedazo de la matriz resultado C.

    - El maestro junta todos esos pedazos para armar la matriz completa.

## Final

- Se imprime cuánto tiempo tardó la operación.

- Se libera la memoria usada y se termina el programa.

## Resultados Actuales

![image](https://github.com/user-attachments/assets/64b6433a-0322-4c57-9c72-99aaf0a9e99a)
- En este caso particular, parece que aumentar el número de procesos MPI (con pocos o un solo hilo OMP por proceso) tiende a dar mejores resultados globales que tener pocos procesos MPI con muchos hilos OMP cada uno, especialmente cuando se dispone de un gran número de unidades de procesamiento.

## Mapa de calor
![image](https://github.com/user-attachments/assets/35285c8b-0cb4-4bda-92d4-b9360c3469f7)
- La estrategia de paralelización más efectiva cuando se dispone de un número considerable de unidades de procesamiento (representadas por el escalado hasta 24 procesos MPI) es utilizar muchos procesos MPI con un solo hilo OpenMP por proceso.
El uso de un modelo híbrido con múltiples hilos OMP por proceso MPI solo es beneficioso cuando el número de procesos MPI es limitado (ej. 4 procesos MPI con 8 hilos OMP es una buena combinación localmente).
