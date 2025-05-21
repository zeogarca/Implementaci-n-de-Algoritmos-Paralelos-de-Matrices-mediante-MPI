## Multiplicación de Matrices

![image](https://github.com/user-attachments/assets/39931cd0-8393-4ce7-bf8d-894a2bf9519a)


# ⚙️ Estructura General del Código
- Inicialización y entrada:
Se obtiene N (tamaño de la matriz) desde los argumentos del programa.

Se inicializa MPI, se identifican rank (número de proceso) y size (número total de procesos).

- División de trabajo:
Se calcula cuántas filas de la matriz A le corresponden a cada proceso.

Se reparte A entre procesos con MPI_Scatterv.

B se transmite completa a todos los procesos con MPI_Bcast.

- Multiplicación local: 
Cada proceso realiza la multiplicación de su parte de A con toda B, almacenando el resultado en local_C.

Se utiliza OpenMP para paralelizar el ciclo anidado de multiplicación.

- Recolección de resultados:
Se utiliza MPI_Gatherv para juntar todas las partes de C en el proceso raíz (rank 0).

El tiempo de ejecución se mide desde el proceso 0 con MPI_Wtime.


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


