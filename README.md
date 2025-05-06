# Implementación de Algoritmos Paralelos de Matrices mediante MPI

![MPI Logo](https://upload.wikimedia.org/wikipedia/commons/thumb/2/25/Message_Passing_Interface_logo.svg/1200px-Message_Passing_Interface_logo.svg.png)

Un algoritmo eficiente para multiplicar matrices grandes utilizando computación paralela con el estándar MPI.

## 📌 Características Clave

- **Algoritmo paralelo** distribuido en múltiples procesos/nodos
- **Soporte para matrices grandes** (limitado solo por la memoria disponible)
- **Generación aleatoria** de matrices para pruebas
- **Métricas de rendimiento**: Tiempo de ejecución y speedup
- **Modo serial** incluido para comparación de resultados

## 📦 Estructura del Proyecto

matmul_mpi/
├── bin/ # Ejecutables compilados
├── data/ # Matrices de prueba (opcional)
├── docs/ # Documentación técnica
├── include/ # Cabeceras (.h)
│ ├── matrix.h # Operaciones con matrices
│ └── utils.h # Funciones auxiliares
├── src/ # Código fuente
│ ├── matrix.c # Lógica de matrices
│ ├── utils.c # Generación de datos
│ └── matmul_mpi.c # Núcleo MPI
├── tests/ # Casos de prueba
├── Makefile # Sistema de compilación
├── LICENSE # Licencia GPLv3
└── README.md # Este archivo



## 🛠 Requisitos Mínimos

- **OpenMPI** 4.0+ o **MPICH** 3.3+
- **gcc** 9.0+ o **clang** 10.0+
- **Linux** (Ubuntu/Debian recomendado) o **macOS**
- 4GB RAM (para matrices >2000x2000)

## 🔥 Instalación Rápida

```bash
# Clonar repositorio
git clone https://github.com/tu_usuario/matmul_mpi.git
cd matmul_mpi

# Compilar (usando Makefile)
make all

# Verificar instalación
./bin/matmul_mpi --version
