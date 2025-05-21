# ⚙️ Estructura General del Código
- Inicialización y entrada
Se obtiene N (tamaño de la matriz) desde los argumentos del programa.

Se inicializa MPI, se identifican rank (número de proceso) y size (número total de procesos).

- División de trabajo
Se calcula cuántas filas de la matriz A le corresponden a cada proceso (sendcounts, displs).

Se reparte A entre procesos con MPI_Scatterv.

B se transmite completa a todos los procesos con MPI_Bcast.

- Multiplicación local: 
Cada proceso realiza la multiplicación de su parte de A con toda B, almacenando el resultado en local_C.

Se utiliza OpenMP para paralelizar el ciclo anidado de multiplicación.

- Recolección de resultados
Se utiliza MPI_Gatherv para juntar todas las partes de C en el proceso raíz (rank 0).

El tiempo de ejecución se mide desde el proceso 0 con MPI_Wtime.

# Implementación Actual
- ✅ Aprovecha paralelismo a dos niveles: entre procesos (MPI) y entre hilos (OpenMP).

- ✅ Usa Scatterv y Gatherv, permitiendo una distribución equilibrada aunque N no sea divisible entre procesos.

- ✅ Puede correr en sistemas distribuidos y aprovechar múltiples núcleos.

### ❌ Mejoras en la siguiente entrega

| Área         | Descripción                                              | Mejora Propuesta                                                            |
|--------------|----------------------------------------------------------|-----------------------------------------------------------------------------|
| **Memoria**      | Cada proceso guarda una copia completa de B.             | Distribuir B por bloques columnares.                                       |

