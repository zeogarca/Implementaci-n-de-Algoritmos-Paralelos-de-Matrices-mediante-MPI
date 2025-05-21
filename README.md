# ⚙️ Estructura General del Código
1. Inicialización y entrada
Se obtiene N (tamaño de la matriz) desde los argumentos del programa.

Se inicializa MPI, se identifican rank (número de proceso) y size (número total de procesos).

2. División de trabajo
Se calcula cuántas filas de la matriz A le corresponden a cada proceso (sendcounts, displs).

Se reparte A entre procesos con MPI_Scatterv.

B se transmite completa a todos los procesos con MPI_Bcast.

3. Multiplicación local
Cada proceso realiza la multiplicación de su parte de A con toda B, almacenando el resultado en local_C.

Se utiliza OpenMP para paralelizar el ciclo anidado de multiplicación.

4. Recolección de resultados
Se utiliza MPI_Gatherv para juntar todas las partes de C en el proceso raíz (rank 0).

El tiempo de ejecución se mide desde el proceso 0 con MPI_Wtime.

5. Finalización
Se imprimen las matrices si son pequeñas (N <= 16) y el tiempo total de ejecución.

Se libera memoria y se finaliza MPI.

#📊 Ventajas de la Implementación Actual
✅ Aprovecha paralelismo a dos niveles: entre procesos (MPI) y entre hilos (OpenMP).

✅ Usa Scatterv y Gatherv, permitiendo una distribución equilibrada aunque N no sea divisible entre procesos.

✅ Puede correr en sistemas distribuidos y aprovechar múltiples núcleos.

#❌ Limitaciones y Oportunidades de Mejora
Área	Descripción	Mejora Propuesta
Memoria	Cada proceso guarda una copia completa de B.	Distribuir B por bloques columnares.
Caché	Multiplicación no optimizada para caché.	Usar tiling (blocking) para mejorar la localidad de memoria.
OpenMP	Usa solo collapse(2) sin política de balanceo.	Agregar schedule(dynamic) para mejor balanceo.
Modularidad	Todo el código está en main.	Separar en funciones (distribuir_matrices(), multiplicar_local(), etc).
Validación	No se compara el resultado con un método secuencial.	Agregar comparación contra resultado secuencial en rank 0.
Reutilización	Repetición de mallocs y liberación de memoria.	Crear funciones auxiliares para asignación y liberación.
Escalabilidad	Podría saturar nodos con demasiados procesos MPI.	Usar un modelo híbrido bien balanceado (1 MPI por nodo + OpenMP por core).
