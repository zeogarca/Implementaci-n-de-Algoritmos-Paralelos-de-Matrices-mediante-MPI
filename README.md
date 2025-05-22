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

- Maestro (rank 0): es el que organiza todo. Crea las matrices A y B, reparte partes de ellas entre los demás procesos, y al final recoge los resultados.

- Esclavos (los demás procesos): reciben sus partes de las matrices y hacen su parte del cálculo. Luego envían el resultado de vuelta al maestro.

## Pasos del programa

Inicio y lectura del tamaño de la matriz:

1. El usuario le dice al programa qué tamaño deben tener las matrices cuadradas (por ejemplo, 1000x1000).

- 2. Se inicializa MPI para que todos los procesos se preparen.

- Creación y reparto de datos (por el maestro)

Se crean dos matrices aleatorias: A y B.

La matriz A se divide por filas, y la matriz B se divide por columnas.

Cada proceso recibe una parte de A y una parte de B, según lo que le toca.

Cómputo (todos los procesos)

Cada proceso multiplica las filas que le tocaron de A por las columnas de B que le llegaron.

Para esto se usa OpenMP, que permite que dentro de cada proceso se usen varios hilos para hacer los cálculos más rápido.

Recolección de resultados

Cada proceso genera un pedazo de la matriz resultado C.

El maestro junta todos esos pedazos para armar la matriz completa.

## Final

Se imprime cuánto tiempo tardó la operación.

Se libera la memoria usada y se termina el programa.




### ❌ Mejoras en la siguiente entrega

| Área         | Descripción                                              | Mejora Propuesta                                                            |
|--------------|----------------------------------------------------------|-----------------------------------------------------------------------------|
| **Memoria**      | Cada proceso guarda una copia completa de B.             | Distribuir B por bloques columnares.                                       |

## Resultados Actuales

              lgarcia@ce:~/m1$ mpirun -np 4 ./matrix_mpi_openmp 2000
              
              ✅ Execution time: 0.375169 seconds
              lgarcia@ce:~/m1$ mpirun -np 8 ./matrix_mpi_openmp 2000
              
              ✅ Execution time: 0.457550 seconds
              lgarcia@ce:~/m1$ mpirun -np 16 ./matrix_mpi_openmp 2000
              
              ✅ Execution time: 0.547565 seconds
              lgarcia@ce:~/m1$ mpirun -np 24 ./matrix_mpi_openmp 2000
              
              ✅ Execution time: 0.605839 seconds

## Resultado en Numpy

              Resultados:
              Tamaño de las matrices: 2000x2000
              Tiempo de ejecución: 0.4345 segundos

## Proceso maestro:

- Crear las matrices completas (A y B) y llenarlas con valores.

- Dividir el trabajo (distribuir filas de A entre los procesos).

- Recolectar los resultados parciales para armar la matriz resultado C.

- Mostrar el tiempo de ejecución y (opcionalmente) las matrices.

## Procesos esclavos:

- Reciben su parte de filas de la matriz A para multiplicar por la matriz B.

- Ejecutan la multiplicación sólo sobre su pedazo asignado.

- Envían de vuelta los resultados parciales al maestro.

## 🧠 Inicialización

      MPI_Init(&argc, &argv);
      MPI_Comm_rank(MPI_COMM_WORLD, &rank);
      MPI_Comm_size(MPI_COMM_WORLD, &size);
      Se inicializa MPI.

rank: el número de proceso actual.

size: cantidad total de procesos.

## 📦 Distribución de datos
1. El proceso 0 crea las matrices:

        if (rank == 0) {
            A = malloc(...);
            C = malloc(...);
            fill_matrix(A, N);
            fill_matrix(B, N);
        }
   
Se generan dos matrices A y B con valores aleatorios.

A se va a distribuir, y B se transmite completa a todos los procesos.

2. Se calcula cuántas filas le toca a cada proceso:

        rows_per_proc = N / size;
        remaining_rows = N % size;
        local_rows = rows_per_proc + (rank < remaining_rows ? 1 : 0);
   
Esto se hace para manejar matrices que no se dividen exactamente entre los procesos.

El offset calcula desde qué fila empieza cada proceso.

3. Se usa MPI_Scatterv para enviar solo las filas necesarias de A a cada proceso:

        MPI_Scatterv(A, sendcounts, displs, MPI_DOUBLE, A_local, ..., 0, MPI_COMM_WORLD);

sendcounts y displs indican cuántos elementos enviar y desde qué posición.


