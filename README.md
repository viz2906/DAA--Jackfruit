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
- Optional interactive manual session for runtime task insertion/extraction

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

### Algorithm Design (Key Ideas + Pseudocode)
Key idea:
- Maintain tasks in a max-heap so the highest-priority task is always at index `0`.
- Use bottom-up heap construction (Floyd method) for fast batch initialization.

Pseudocode:

```text
INSERT_TASK(heap, task, priority)
  if heap is full: resize array to 2x
  heap[size] <- (task, priority)
  i <- size
  while i > 0 and heap[parent(i)].priority < heap[i].priority
    swap(heap[i], heap[parent(i)])
    i <- parent(i)
  size <- size + 1

EXTRACT_MAX(heap)
  if size == 0: return EMPTY
  answer <- heap[0]
  heap[0] <- heap[size-1]
  size <- size - 1
  HEAPIFY_DOWN(heap, 0)
  return answer

HEAPIFY_DOWN(heap, i)
  loop
    largest <- i
    l <- left(i), r <- right(i)
    if l < size and heap[l] > heap[largest]: largest <- l
    if r < size and heap[r] > heap[largest]: largest <- r
    if largest == i: break
    swap(heap[i], heap[largest])
    i <- largest
```

### Manual Task Insertion Mode (Q1)

When the program starts, it asks:

```text
Run manual Q1 task insertion session first? (y/n):
```

Enter `y` to open an interactive heap menu:

```text
1. Insert task
2. Show highest-priority task (peek)
3. Remove highest-priority task
4. Print heap tree
0. Exit manual session
```

Notes:
- Task names are entered as a single token (`no spaces`).
- Priority is an integer; larger value means higher scheduling priority.
- Option `2` shows the current max without removing it.
- Option `3` removes the max and shows step-by-step heapify-down after extraction.
- After exiting (`0`), the normal Q1/Q2/Q3 demos continue automatically.

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

### Algorithm Design (Key Ideas + Pseudocode)
Key idea:
- Dynamic programming over allowed intermediate vertices.
- `dist[i][j]` stores current best shortest path from `i` to `j`.
- At phase `k`, paths are allowed to use vertices `{0..k}` as intermediates.

Pseudocode:

```text
FLOYD_WARSHALL(graph, n)
  dist <- copy(graph)
  pred <- predecessor initialization

  for k in 0..n-1
    for i in 0..n-1
      for j in 0..n-1
        if dist[i][k] != INF and dist[k][j] != INF
          through_k <- dist[i][k] + dist[k][j]
          if through_k < dist[i][j]
            dist[i][j] <- through_k
            pred[i][j] <- pred[k][j]

  return dist, pred
```

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

### Algorithm Design (Key Ideas + Pseudocode)
Key idea:
- Use DFS with a recursion stack for cycle detection.
- For DAGs, push each node onto a stack after exploring descendants.
- Pop stack to get topological order.

Pseudocode:

```text
TOPOLOGICAL_SORT(graph)
    if DETECT_CYCLE(graph): return FAILURE
    visited <- all false
    stack <- empty

    for each vertex v
        if not visited[v]
            DFS_TOPO(v)

    return reverse(finish_order_stack)

DFS_TOPO(v)
    visited[v] <- true
    for each neighbor u of v
        if not visited[u]
            DFS_TOPO(u)
    push v onto stack

DETECT_CYCLE(graph)
    visited <- false, in_recursion <- false
    for each vertex v
        if not visited[v] and DFS_CYCLE(v): return true
    return false
```

## Complexity Analysis (With Justification)

### Q1: Max-Heap Scheduler
- Build from unsorted priorities (bottom-up): `O(n)` because heapify is applied only to internal nodes and aggregated work is linear.
- Insert: `O(log n)` due to heapify-up along tree height.
- Extract max: `O(log n)` due to heapify-down along tree height.
- Space: `O(n)` for heap array.

### Q2: Floyd-Warshall
- Time: `O(n^3)` from three nested loops `(k, i, j)`.
- Space: `O(n^2)` for distance and predecessor matrices.
- This complexity is deterministic and independent of edge density.

### Q3: DFS Topological Sort + Cycle Detection
- Time: `O(V + E)` since each vertex and edge is processed a constant number of times.
- Space: `O(V + E)` for adjacency list plus `O(V)` auxiliary arrays/stack.

## Implementation Details

### Programming Approach
- Language: C11 with modular decomposition by question (`q1`, `q2`, `q3`).
- Main orchestrator: `main.c` runs interactive Q1 mode, then all demos and Q1 benchmarking.
- Defensive checks: allocation failures and invalid inputs are handled with early returns/messages.

### Data Structures Used
- Q1:
  - `Task { name, priority }`
  - `MaxHeap { Task *arr, size, capacity }` dynamic array-based binary heap
- Q2:
  - Fixed-size adjacency matrix for graph (`MAX_NODES x MAX_NODES`)
  - `dist` matrix for shortest-path values
  - `pred` matrix for path reconstruction
- Q3:
  - Adjacency list using linked nodes `AdjNode { dest, next }`
  - `Graph { heads[], names[], n }`

### Design Choices
- Heap in Q1 is array-based for cache-friendly operations and direct index arithmetic.
- Floyd-Warshall in Q2 uses matrix representation for direct transition checks and straightforward DP transitions.
- Q3 uses adjacency list to keep traversal linear in graph representation size and naturally support DFS recursion.
- Cycle detection is integrated before ordering to prevent invalid build schedules.

## Experimental Evaluation

### Input Datasets and Test Cases
- Q1 benchmark datasets:
  - Random integer priorities in range `[1, 100000]`
  - Sizes: `1000`, `5000`, `10000`, `50000`
  - Repetitions: `5` per size (average reported)
  - Compared methods:
    - Max-heap build + extract `k=100`
    - Sorted-array build (`qsort`) + extract by repeated shift
- Q2 functional test dataset:
  - Directed weighted graph of `5` nodes with one negative edge and no negative cycle.
  - Validates full distance matrix and path reconstructions `(0,3)` and `(3,2)`.
- Q3 functional test datasets:
  - Realistic DAG of `8` microservices.
  - Deliberate cyclic graph `0->1->2->0` for rejection behavior.

### Benchmark Results (Q1)

From `benchmark_results.csv`:

| n | heap_build_ms | heap_extract_ms | array_build_ms | array_extract_ms |
|---:|---:|---:|---:|---:|
| 1000 | 3.0000 | 0.0000 | 0.2000 | 0.0000 |
| 5000 | 14.0000 | 0.2000 | 0.6000 | 0.0000 |
| 10000 | 28.4000 | 0.0000 | 1.8000 | 1.0000 |
| 50000 | 105.6000 | 0.0000 | 8.4000 | 1.8000 |

### Observations and Performance Trends
- Build phase:
  - Sorted-array build is faster in this implementation due to optimized C library sort constants at tested sizes.
  - Heap build still shows near-linear growth as expected from bottom-up construction.
- Extraction phase (`k=100`):
  - Heap extraction remains very small and scales gently with `n`, consistent with `O(k log n)`.
  - Sorted-array extraction grows with `n` because each extraction shifts remaining elements (`O(k*n)`).
- Zero values in some extraction timings are due to `clock()` granularity for short intervals, not truly zero cost.

### Overall Evaluation
- For workloads with frequent online insertions and repeated high-priority removals, heap scheduling remains the more scalable strategy despite build-time constant-factor differences.
- Q2 and Q3 functional tests validate correctness in normal and edge conditions (path reconstruction and cycle rejection).

## Code Structure and Documentation Quality
- Code is split by problem domain with separate headers and implementation files.
- Functions include role and time-complexity comments.
- Memory allocated dynamically is explicitly released via `free()`.
- Output is labeled clearly to aid traceability during demonstration and grading.

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
