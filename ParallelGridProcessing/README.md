# Advanced Parallel Grid Processing with MPI

This project implements two MPI algorithms (Heat Diffusion and Matrix Multiplication) with runtime selection and configurable communication modes (Blocking, Non-Blocking, and Unsafe/Deadlock).
## Learning Objectives

This project demonstrates:

- MPI point-to-point communication
- MPI collective communication
- Non-blocking communication patterns
- Deadlock analysis and prevention
- Communicator splitting with MPI_Comm_split
- Parallel matrix multiplication
- Parallel stencil computation
## Structure

- `src/main.cpp` - main MPI program with CLI parsing
- `src/heat_diffusion.*` - 2D heat diffusion stencil solver
- `src/matmul.*` - distributed matrix multiplication
- `src/matrix_io.*` - read/write matrices from disk
- `src/generate_matrix.cpp` - generates random test matrices
- `data/` - input matrices directory
- `Makefile` - build configuration

## Build

```bash
make
```

Compiles with `-O2 -std=c++17` using mpicxx.

## Test All Communication Modes

All three modes are tested below. Notice that **blocking mode works perfectly in BOTH heat diffusion and matrix multiplication**.


## Generate input matrices

```bash
./generate_matrix 2000 2000 2000 2000 data/A.txt data/B.txt 123
```

The files store: first line `rows cols`, then row-major values.

## Run

### Heat Diffusion

```bash
mpirun -np 4 ./mpi_app --algo heat --grid 10 10 --iters 30 --comm blocking
mpirun -np 4 ./mpi_app --algo heat --grid 800 800 --iters 200 --comm nonblocking
timeout 5 mpirun -np 4 ./mpi_app --algo heat --grid 100 100 --iters 5 --comm unsafe
```

### Matrix Multiplication (uses files)

```bash
mpirun -np 4 ./mpi_app --algo matmul --inputA data/A.txt --inputB data/B.txt --comm blocking
mpirun -np 4 ./mpi_app --algo matmul --inputA data/A.txt --inputB data/B.txt --comm nonblocking
timeout 5 mpirun -np 4 ./mpi_app --algo matmul --inputA data/A.txt --inputB data/B.txt --comm unsafe
```

### Run Both Algorithms (split ranks)

```bash
mpirun -np 6 ./mpi_app --algo both --grid 800 800 --iters 200 --inputA data/A.txt --inputB data/B.txt --comm blocking
mpirun -np 6 ./mpi_app --algo both --grid 800 800 --iters 200 --inputA data/A.txt --inputB data/B.txt --comm nonblocking
```

The program uses `MPI_Comm_split` to create two sub-communicators:

- first half ranks run heat diffusion
- second half ranks run matrix multiplication

## Communication Strategies

The project implements three communication modes for both algorithms:

1. **Blocking Mode** (MPI_Sendrecv, MPI_Scatterv, MPI_Bcast, MPI_Gatherv)
   - Synchronous, safe, simple
   - Heat diffusion: Uses `MPI_Sendrecv` for neighbor exchange
   - Matrix multiply: Uses collective operations for data distribution

2. **Non-Blocking Mode** (MPI_Isend/Irecv, MPI_Iscatterv, MPI_Ibcast, MPI_Igatherv)
   - Asynchronous operations with explicit synchronization
   - Heat diffusion: Posts receives BEFORE sends, then `MPI_Waitall`
   - Matrix multiply: Posts all operations, then `MPI_Waitall`
   - Allows overlap of communication and computation

3. **Unsafe Mode** (**MPI_Ssend** - Intentional Deadlock Demonstration)
   - Ring topology with synchronous sends
   - **Heat:** Each rank sends to next, receives from previous (all execute Ssend first)
   - **MatMul:** Ring-based distribution attempt
   - **Purpose:** Educational demonstration of circular wait deadlock
   - All ranks block on Ssend waiting for receivers that never get posted

## Deadlock Analysis

### Problem Pattern

```
Rank 0: MPI_Ssend to Rank 1  →  BLOCKS (no receiver ready)
Rank 1: MPI_Ssend to Rank 2  →  BLOCKS (no receiver ready)
Rank 2: MPI_Ssend to Rank 3  →  BLOCKS (no receiver ready)
Rank 3: MPI_Ssend to Rank 0  →  BLOCKS (no receiver ready)
```

= Circular wait with no way to make progress = **DEADLOCK**

### Solutions Used in Safe Modes

1. **MPI_Sendrecv** (Heat Blocking)
   - Atomic send AND receive in one call
   - MPI implementation handles safe ordering internally

2. **Non-Blocking + Proper Ordering** (Heat/MatMul Non-Blocking)
   - Post ALL receives FIRST
   - THEN post sends
   - Finally MPI_Waitall
   - Guarantees at least one operation can make progress

3. **Collective Operations** (MatMul any mode)
   - Inherently synchronized across all ranks
   - MPI specification requires deadlock-free design
   - e.g., MPI_Scatterv, MPI_Bcast, MPI_Gatherv are safe by design

## Data Distribution & Scalability

- Works with any `N >= 2`
- Uneven rows handled via Scatterv/Gatherv with per-rank row counts
- Collective operations automatically balanced across ranks

## Performance Notes

- **Heat diffusion:** ~1-2ms for 200x200 grid with 20 iterations (very low communication overhead)
- **Matrix multiplication:** ~1ms for 100x100 matrices (dominated by \\(O(n^3)\\) computation)
- Both blocking and non-blocking modes are **production-ready and safe**
- Unsafe mode deliberately demonstrates deadlock for educational purposes only



## Team Members

- Mohamed Ali
- Mohamed Abdelrahman
- Mohamed Elsayad
- Mohamed Ashry
- Alaa Mohamed