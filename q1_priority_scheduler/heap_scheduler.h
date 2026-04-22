#ifndef HEAP_SCHEDULER_H
#define HEAP_SCHEDULER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TASK_NAME_LEN 64
#define DEFAULT_HEAP_CAPACITY 1024
#define BENCHMARK_K_EXTRACT 100
#define DEMO_TASK_COUNT 15

typedef struct {
    char name[MAX_TASK_NAME_LEN];
    int priority;
} Task;

typedef struct {
    Task *arr;
    int size;
    int capacity;
} MaxHeap;

MaxHeap *create_heap(int capacity);
void free_heap(MaxHeap *heap);
int insert_task(MaxHeap *heap, const char *task_name, int priority);
int extract_max(MaxHeap *heap, Task *out_task, int show_steps);
void print_heap_tree(const MaxHeap *heap);
void build_random_heap(MaxHeap *heap, int n);
int build_heap_from_priorities(MaxHeap *heap, const int *priorities, int n);
void run_q1_demo(void);
void run_q1_benchmark(const char *csv_path);

#endif
