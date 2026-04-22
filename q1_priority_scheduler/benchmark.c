#include "heap_scheduler.h"

/**
 * Role: Compare integers in descending order for qsort.
 * Time Complexity: O(1)
 */
static int compare_desc(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    if (ia < ib) {
        return 1;
    }
    if (ia > ib) {
        return -1;
    }
    return 0;
}

/**
 * Role: Build sorted-array priority structure from raw priorities.
 * Time Complexity: O(n log n) due to qsort
 */
static void build_sorted_array(int *arr, const int *priorities, int n) {
    int i;
    for (i = 0; i < n; i++) {
        arr[i] = priorities[i];
    }
    qsort(arr, (size_t)n, sizeof(int), compare_desc);
}

/**
 * Role: Extract k max elements from sorted array by shifting remaining elements.
 * Time Complexity: O(k * n)
 */
static int extract_k_sorted_array(int *arr, int n, int k, int *sum_sink) {
    int i;
    int extracted = 0;
    int current_n = n;

    for (i = 0; i < k && current_n > 0; i++) {
        int j;
        int value = arr[0];
        *sum_sink += value;

        for (j = 1; j < current_n; j++) {
            arr[j - 1] = arr[j];
        }

        current_n--;
        extracted++;
    }

    return extracted;
}

/**
 * Role: Convert CPU ticks from clock() into milliseconds.
 * Time Complexity: O(1)
 */
static double ticks_to_ms(clock_t ticks) {
    return (1000.0 * (double)ticks) / (double)CLOCKS_PER_SEC;
}

/**
 * Role: Generate deterministic random priorities for repeatable benchmark runs.
 * Time Complexity: O(n)
 */
static void generate_priorities(int *priorities, int n, unsigned int seed) {
    int i;
    srand(seed);
    for (i = 0; i < n; i++) {
        priorities[i] = (rand() % 100000) + 1;
    }
}

/**
 * Role: Benchmark heap scheduler against sorted-array strategy and export CSV.
 * Time Complexity: O(R * (n + k log n + n log n + k*n)) per n, where R=5
 */
void run_q1_benchmark(const char *csv_path) {
    const int sizes[] = {1000, 5000, 10000, 50000};
    const int repeats = 5;
    const int n_sizes = (int)(sizeof(sizes) / sizeof(sizes[0]));
    FILE *fp;
    int s;

    printf("\nRunning Q1 benchmark (heap vs sorted-array)...\n");

    fp = fopen(csv_path, "w");
    if (fp == NULL) {
        printf("Failed to open benchmark CSV file: %s\n", csv_path);
        return;
    }

    fprintf(fp, "n,heap_build_ms,heap_extract_ms,array_build_ms,array_extract_ms\n");

    for (s = 0; s < n_sizes; s++) {
        int n = sizes[s];
        int r;
        double heap_build_total = 0.0;
        double heap_extract_total = 0.0;
        double arr_build_total = 0.0;
        double arr_extract_total = 0.0;

        for (r = 0; r < repeats; r++) {
            int *priorities = (int *)malloc((size_t)n * sizeof(int));
            int *arr = (int *)malloc((size_t)n * sizeof(int));
            MaxHeap *heap = create_heap(n + 8);
            clock_t t0;
            clock_t t1;
            int sink_sum = 0;

            if (priorities == NULL || arr == NULL || heap == NULL) {
                printf("Memory allocation failed during benchmark (n=%d).\n", n);
                free(priorities);
                free(arr);
                free_heap(heap);
                fclose(fp);
                return;
            }

            generate_priorities(priorities, n, (unsigned int)(n * 100 + r + 7));

            t0 = clock();
            if (!build_heap_from_priorities(heap, priorities, n)) {
                printf("Heap build failed during benchmark (n=%d).\n", n);
                free(priorities);
                free(arr);
                free_heap(heap);
                fclose(fp);
                return;
            }
            t1 = clock();
            heap_build_total += ticks_to_ms(t1 - t0);

            t0 = clock();
            for (int i = 0; i < BENCHMARK_K_EXTRACT; i++) {
                Task out;
                if (!extract_max(heap, &out, 0)) {
                    break;
                }
                sink_sum += out.priority;
            }
            t1 = clock();
            heap_extract_total += ticks_to_ms(t1 - t0);

            t0 = clock();
            build_sorted_array(arr, priorities, n);
            t1 = clock();
            arr_build_total += ticks_to_ms(t1 - t0);

            t0 = clock();
            extract_k_sorted_array(arr, n, BENCHMARK_K_EXTRACT, &sink_sum);
            t1 = clock();
            arr_extract_total += ticks_to_ms(t1 - t0);

            if (sink_sum == -1) {
                printf("Impossible sink value: %d\n", sink_sum);
            }

            free(priorities);
            free(arr);
            free_heap(heap);
        }

        fprintf(fp, "%d,%.4f,%.4f,%.4f,%.4f\n",
                n,
                heap_build_total / repeats,
                heap_extract_total / repeats,
                arr_build_total / repeats,
                arr_extract_total / repeats);

        printf("  n=%d completed.\n", n);
    }

    fclose(fp);
    printf("Benchmark results saved to %s\n", csv_path);
}

/*
Complexity Analysis (Q1 Benchmark)
Best Case Time:
- Heap build: O(n) using Floyd's bottom-up heapify.
- Heap extract k: O(k) if each extract requires no swaps (rare best case).
- Sorted-array build: O(n log n) with comparison sort.
- Sorted-array extract k with shifting: O(k*n).

Worst Case Time:
- Heap build: O(n).
- Heap extract k: O(k log n).
- Sorted-array build: O(n log n).
- Sorted-array extract k: O(k*n).

Average Case Time:
- Heap build: O(n).
- Heap extract k: O(k log n).
- Sorted-array build: O(n log n).
- Sorted-array extract k: O(k*n).

Space Complexity:
- O(n) for both heap and array representations.

Why Transform-and-Conquer is optimal here:
- Max-heap keeps high-priority retrieval efficient under continuous updates,
  while sorted-array suffers expensive shifts during repeated extractions.
*/
