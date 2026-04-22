# DAA Assignment Project (C11)

This project contains three fully implemented Design and Analysis of Algorithms (DAA) solutions in pure C, aligned with core paradigms:

- Transform-and-Conquer: Max-Heap based real-time priority scheduler
- Dynamic Programming: Floyd-Warshall all-pairs shortest path
- Decrease-and-Conquer: DFS-based topological sort with cycle detection

## Repository Structure

```text
daa_assignment/
├── q1_priority_scheduler/
│   ├── heap_scheduler.c
│   ├── heap_scheduler.h
│   └── benchmark.c
├── q2_shortest_path/
│   ├── floyd_warshall.c
│   └── floyd_warshall.h
├── q3_topological_sort/
│   ├── topo_sort.c
│   └── topo_sort.h
├── main.c
├── Makefile
└── benchmark_results.csv
```

## Question 1: Real-Time Priority Task Scheduler (Max-Heap)

### Features
- Custom max-heap implementation from scratch
- `insert_task(heap, task, priority)` with heapify-up
- `extract_max(heap)` with heapify-down
- Step-by-step heapify-down visualization after extraction
- Demo with random tasks and top-k extraction
- Benchmark against sorted-array approach

### Benchmark Setup
- Input sizes: `n = 1000, 5000, 10000, 50000`
- Extraction count: `k = 100`
- Repetitions per size: `5` (average reported)
- Output CSV columns:
  - `n`
  - `heap_build_ms`
  - `heap_extract_ms`
  - `array_build_ms`
  - `array_extract_ms`

### Complexity
- Heap build (bottom-up): `O(n)`
- Insert: `O(log n)`
- Extract max: `O(log n)`

## Question 2: Floyd-Warshall (All-Pairs Shortest Path)

### Features
- Weighted directed graph handling with `INF = 99999`
- `floyd_warshall(graph, n, dist, pred)` for shortest path matrix and predecessor matrix
- `reconstruct_path(pred, i, j)` for actual node sequence output
- Negative cycle detection using `dist[i][i] < 0`
- Demo graph with 5 nodes and a negative edge

### Required Demo Outputs
- Distance matrix before algorithm
- Distance matrix after algorithm
- Path reconstructions for:
  - `(0, 3)`
  - `(3, 2)`

### Complexity
- Time: `O(n^3)`
- Space: `O(n^2)`

## Question 3: Topological Sort for Microservice Build Order

### Features
- DAG represented with adjacency list
- DFS-based topological sort using explicit stack
- Prints remaining unvisited node count at each recursive step
- Cycle detection with recursion stack
- If cycle exists, prints offending service chain and aborts topological ordering

### Test Cases Included
- Realistic 8-service dependency graph:
  - `0=AuthService`
  - `1=DatabaseService`
  - `2=UserService`
  - `3=OrderService`
  - `4=PaymentService`
  - `5=NotificationService`
  - `6=APIGateway`
  - `7=DeploymentService`
- Deliberate cycle graph: `0->1, 1->2, 2->0`

### Complexity
- Time: `O(V + E)`
- Space: `O(V + E)`

## Build and Run

### Using GCC directly (works on Windows PowerShell)

From project root:

```powershell
gcc -Wall -Wextra -std=c11 -O2 -c main.c -o main.o
gcc -Wall -Wextra -std=c11 -O2 -c q1_priority_scheduler/heap_scheduler.c -o q1_priority_scheduler/heap_scheduler.o
gcc -Wall -Wextra -std=c11 -O2 -c q1_priority_scheduler/benchmark.c -o q1_priority_scheduler/benchmark.o
gcc -Wall -Wextra -std=c11 -O2 -c q2_shortest_path/floyd_warshall.c -o q2_shortest_path/floyd_warshall.o
gcc -Wall -Wextra -std=c11 -O2 -c q3_topological_sort/topo_sort.c -o q3_topological_sort/topo_sort.o
gcc -Wall -Wextra -std=c11 -O2 -o daa_assignment main.o q1_priority_scheduler/heap_scheduler.o q1_priority_scheduler/benchmark.o q2_shortest_path/floyd_warshall.o q3_topological_sort/topo_sort.o -lm
./daa_assignment.exe
```

### Using Makefile

```bash
make all
./daa_assignment
```

Available targets:
- `all`
- `q1`
- `q2`
- `q3`
- `clean`

## Output

- Console output includes:
  - Labeled demo sections for all three questions
  - Heapify-down intermediate tree states
  - Floyd-Warshall before/after matrices and path reconstruction
  - Topological ordering and cycle detection diagnostics
- Benchmark CSV is written to:
  - `benchmark_results.csv`

## Notes

- All dynamic allocations are paired with `free()` calls.
- Each function includes a role + time complexity comment block.
- Each implementation file ends with a complexity summary (best/worst/average, space, and paradigm justification).
