#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

// Generador de números aleatorios entre 0 y 9
void fill_matrix(int *mat, int N) {
    for (int i = 0; i < N * N; i++) {
        mat[i] = rand() % 10;
    }
}

// Extrae columna j de una matriz NxN
void get_col(int *mat, int N, int j, int *col) {
    for (int i = 0; i < N; i++) {
        col[i] = mat[i * N + j];
    }
}

// Producto escalar de dos vectores de tamaño N
int dot_product(int *row, int *col, int N) {
    int sum = 0;
    for (int i = 0; i < N; i++) {
        sum += row[i] * col[i];
    }
    return sum;
}

int main(int argc, char *argv[]) {
    int rank, size, N;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 2) {
        if (rank == MASTER)
            printf("Uso: %s <tamaño N de la matriz>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    N = atoi(argv[1]);

    int *A = NULL, *B = NULL, *C = NULL;

    if (rank == 0) {
        A = (int *)malloc(N * N * sizeof(int));
        B = (int *)malloc(N * N * sizeof(int));
        C = (int *)malloc(N * N * sizeof(int));

        srand(42); // Semilla fija para reproducibilidad
        fill_matrix(A, N);
        fill_matrix(B, N);

        int total_tasks = N * N;
        int next_task = 0;

        start_time = MPI_Wtime();

        for (int i = 1; i < size && next_task < total_tasks; i++, next_task++) {
            int row_idx = next_task / N;
            int col_idx = next_task % N;
            int *row = &A[row_idx * N];
            int col[N];
            get_col(B, N, col_idx, col);

            MPI_Send(row, N, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(col, N, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&row_idx, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
            MPI_Send(&col_idx, 1, MPI_INT, i, 3, MPI_COMM_WORLD);
        }

        while (next_task < total_tasks) {
            MPI_Status status;
            int result, row_idx, col_idx;
            MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE, 4, MPI_COMM_WORLD, &status);
            MPI_Recv(&row_idx, 1, MPI_INT, status.MPI_SOURCE, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&col_idx, 1, MPI_INT, status.MPI_SOURCE, 6, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            C[row_idx * N + col_idx] = result;

            int row_i = next_task / N;
            int col_j = next_task % N;
            int *row = &A[row_i * N];
            int col[N];
            get_col(B, N, col_j, col);

            MPI_Send(row, N, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
            MPI_Send(col, N, MPI_INT, status.MPI_SOURCE, 1, MPI_COMM_WORLD);
            MPI_Send(&row_i, 1, MPI_INT, status.MPI_SOURCE, 2, MPI_COMM_WORLD);
            MPI_Send(&col_j, 1, MPI_INT, status.MPI_SOURCE, 3, MPI_COMM_WORLD);

            next_task++;
        }

        for (int i = 1; i < size && i <= total_tasks; i++) {
            MPI_Status status;
            int result, row_idx, col_idx;
            MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE, 4, MPI_COMM_WORLD, &status);
            MPI_Recv(&row_idx, 1, MPI_INT, status.MPI_SOURCE, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&col_idx, 1, MPI_INT, status.MPI_SOURCE, 6, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            C[row_idx * N + col_idx] = result;
        }

        end_time = MPI_Wtime();

        for (int i = 1; i < size; i++) {
            MPI_Send(NULL, 0, MPI_INT, i, 99, MPI_COMM_WORLD);
        }

        printf("Multiplicación terminada.\n");
        printf("Tiempo total con %d proceso(s): %f segundos\n", size, end_time - start_time);

        free(A);
        free(B);
        free(C);
    } else {
        // Código de los procesos esclavos
        while (1) {
            MPI_Status status;
            int row[N], col[N];
            int row_idx, col_idx;

            MPI_Recv(row, N, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (status.MPI_TAG == 99)
                break; // señal de fin de trabajo

            MPI_Recv(col, N, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&row_idx, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&col_idx, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            int result = dot_product(row, col, N);

            MPI_Send(&result, 1, MPI_INT, 0, 4, MPI_COMM_WORLD);
            MPI_Send(&row_idx, 1, MPI_INT, 0, 5, MPI_COMM_WORLD);
            MPI_Send(&col_idx, 1, MPI_INT, MASTER, 6, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return 0;
}
