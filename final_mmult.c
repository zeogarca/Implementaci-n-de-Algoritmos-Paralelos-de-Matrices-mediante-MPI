#include <mpi.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void fill_matrix(double *matrix, int rows, int cols) {
    for (int i = 0; i < rows * cols; i++)
        matrix[i] = rand() % 10;
}


int main(int argc, char *argv[]) {
    int rank, size, N;
    double *A = NULL, *B = NULL, *C = NULL;
    double *A_local, *B_local, *C_local;
    int *sendcnts_B, *displs_B, *sendcnts_C, *displs_C;
    double start, end;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 2) {
        if (rank == 0) printf("Uso: %s <tamaño N>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }
    N = atoi(argv[1]);

    // ----------------------------------------------------------------
    // 1) Distribución de filas de A (igual que antes)
    int rows_per_proc = N / size;
    int rem_rows      = N % size;
    int local_rows    = rows_per_proc + (rank < rem_rows ? 1 : 0);
    int *sendcnts_A   = malloc(size * sizeof(int));
    int *displs_A     = malloc(size * sizeof(int));
    {
        int offset = 0;
        for (int i = 0; i < size; i++) {
            int r = rows_per_proc + (i < rem_rows ? 1 : 0);
            sendcnts_A[i] = r * N;
            displs_A[i]   = offset;
            offset       += sendcnts_A[i];
        }
    }

    // ----------------------------------------------------------------
    // 2) Distribución de columnas de B
    int cols_per_proc = N / size;
    int rem_cols      = N % size;
    int local_cols    = cols_per_proc + (rank < rem_cols ? 1 : 0);

    sendcnts_B = malloc(size * sizeof(int));
    displs_B   = malloc(size * sizeof(int));
    {
        int offset = 0;
        for (int i = 0; i < size; i++) {
            int c = cols_per_proc + (i < rem_cols ? 1 : 0);
            sendcnts_B[i] = c * N;      // cada columna tiene N elementos
            displs_B[i]   = offset;
            offset       += sendcnts_B[i];
        }
    }

    // ----------------------------------------------------------------
    // 3) Reservar memoria local
    A_local = malloc(local_rows * N * sizeof(double));
    B_local = malloc(N * local_cols * sizeof(double));
    C_local = calloc(local_rows * local_cols, sizeof(double));

    if (rank == 0) {
        // maestro crea matrices completas
        A = malloc(N * N * sizeof(double));
        B = malloc(N * N * sizeof(double));
        C = malloc(N * N * sizeof(double));
        fill_matrix(A, N, N);
        fill_matrix(B, N, N);
    }

    // Scatter filas de A
    MPI_Scatterv(A, sendcnts_A, displs_A, MPI_DOUBLE,
                 A_local, sendcnts_A[rank], MPI_DOUBLE,
                 0, MPI_COMM_WORLD);

    // Empaquetar columnas de B en buffer contiguo solo en maestro
    double *B_pack = NULL;
    if (rank == 0) {
        B_pack = malloc(N * N * sizeof(double));
        // Para cada proceso i, copiamos sus columnas [start_col..end_col) en B_pack
        for (int p = 0, col_off = 0; p < size; p++) {
            int cols_p = cols_per_proc + (p < rem_cols ? 1 : 0);
            int start_col = p * cols_per_proc + (p < rem_cols ? p : rem_cols);
            for (int c = 0; c < cols_p; c++) {
                int gc = start_col + c; // columna global
                // copiamos la columna gc entera (N elementos) a B_pack + displs_B[p]
                for (int r = 0; r < N; r++) {
                    B_pack[ displs_B[p] + c * N + r ] = B[r * N + gc];
                }
            }
        }
    }

    // Scatterv de B_pack: cada proceso recibe solo sus columnas
    MPI_Scatterv(B_pack, sendcnts_B, displs_B, MPI_DOUBLE,
                 B_local, sendcnts_B[rank], MPI_DOUBLE,
                 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    start = MPI_Wtime();

    // ----------------------------------------------------------------
    // 4) Multiplicación local (A_local: local_rows x N) × (B_local: N x local_cols)
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < local_rows; i++) {
        for (int j = 0; j < local_cols; j++) {
            double sum = 0.0;
            for (int k = 0; k < N; k++) {
                sum += A_local[i * N + k] * B_local[k * local_cols + j];
            }
            C_local[i * local_cols + j] = sum;
        }
    }

    // ----------------------------------------------------------------
    // 5) Recolección de bloques de columnas de C
    sendcnts_C = sendcnts_B;
    displs_C   = displs_B;

    MPI_Gatherv(C_local, local_rows * local_cols, MPI_DOUBLE,
                C, sendcnts_C, displs_C, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    if (rank == 0) {
        printf("Tiempo total: %.6f s con %d procesos y MPI+OpenMP\n",
               end - start, size);
    // ----------------------------------------------------------------
    // limpieza
    free(A_local);
    free(B_local);
    free(C_local);
    free(sendcnts_A);
    free(displs_A);
    free(sendcnts_B);
    free(displs_B);
    if (rank == 0) {
        free(A);
        free(B);
        free(C);
        free(B_pack);
    }

    MPI_Finalize();
    return 0;
}
