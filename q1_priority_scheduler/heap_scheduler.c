#include "heap_scheduler.h"

/**
 * Role: Swap two Task elements in-place.
 * Time Complexity: O(1)
 */
static void swap_task(Task *a, Task *b) {
    Task tmp = *a;
    *a = *b;
    *b = tmp;
}

/**
 * Role: Return parent index in a binary heap array.
 * Time Complexity: O(1)
 */
static int parent_index(int i) {
    return (i - 1) / 2;
}

/**
 * Role: Return left child index in a binary heap array.
 * Time Complexity: O(1)
 */
static int left_index(int i) {
    return (2 * i) + 1;
}

/**
 * Role: Return right child index in a binary heap array.
 * Time Complexity: O(1)
 */
static int right_index(int i) {
    return (2 * i) + 2;
}

/**
 * Role: Resize heap storage when capacity is exhausted.
 * Time Complexity: O(n) due to full-array copy by realloc in worst case
 */
static int ensure_capacity(MaxHeap *heap) {
    Task *new_arr;
    int new_capacity;

    if (heap->size < heap->capacity) {
        return 1;
    }

    new_capacity = heap->capacity * 2;
    if (new_capacity < 1) {
        new_capacity = DEFAULT_HEAP_CAPACITY;
    }

    new_arr = (Task *)realloc(heap->arr, (size_t)new_capacity * sizeof(Task));
    if (new_arr == NULL) {
        return 0;
    }

    heap->arr = new_arr;
    heap->capacity = new_capacity;
    return 1;
}

/**
 * Role: Restore max-heap order from a node to root using heapify-up.
 * Time Complexity: O(log n)
 */
static void heapify_up(MaxHeap *heap, int idx) {
    while (idx > 0 && heap->arr[parent_index(idx)].priority < heap->arr[idx].priority) {
        swap_task(&heap->arr[idx], &heap->arr[parent_index(idx)]);
        idx = parent_index(idx);
    }
}

/**
 * Role: Restore max-heap order from root downward after extraction.
 * Time Complexity: O(log n)
 */
static void heapify_down(MaxHeap *heap, int idx, int show_steps) {
    int largest;
    int left;
    int right;
    int step = 1;

    while (1) {
        largest = idx;
        left = left_index(idx);
        right = right_index(idx);

        if (left < heap->size && heap->arr[left].priority > heap->arr[largest].priority) {
            largest = left;
        }
        if (right < heap->size && heap->arr[right].priority > heap->arr[largest].priority) {
            largest = right;
        }

        if (largest == idx) {
            break;
        }

        if (show_steps) {
            printf("  Heapify-down step %d: swap %s(%d) with %s(%d)\n",
                   step,
                   heap->arr[idx].name,
                   heap->arr[idx].priority,
                   heap->arr[largest].name,
                   heap->arr[largest].priority);
        }

        swap_task(&heap->arr[idx], &heap->arr[largest]);
        idx = largest;

        if (show_steps) {
            print_heap_tree(heap);
            step++;
        }
    }
}

/**
 * Role: Heapify-down variant used for bottom-up heap construction.
 * Time Complexity: O(log n)
 */
static void heapify_down_build(MaxHeap *heap, int idx) {
    int largest;
    int left;
    int right;

    while (1) {
        largest = idx;
        left = left_index(idx);
        right = right_index(idx);

        if (left < heap->size && heap->arr[left].priority > heap->arr[largest].priority) {
            largest = left;
        }
        if (right < heap->size && heap->arr[right].priority > heap->arr[largest].priority) {
            largest = right;
        }

        if (largest == idx) {
            break;
        }

        swap_task(&heap->arr[idx], &heap->arr[largest]);
        idx = largest;
    }
}

/**
 * Role: Allocate and initialize a max-heap.
 * Time Complexity: O(1)
 */
MaxHeap *create_heap(int capacity) {
    MaxHeap *heap;
    int initial_capacity = capacity;

    if (initial_capacity < 1) {
        initial_capacity = DEFAULT_HEAP_CAPACITY;
    }

    heap = (MaxHeap *)malloc(sizeof(MaxHeap));
    if (heap == NULL) {
        return NULL;
    }

    heap->arr = (Task *)malloc((size_t)initial_capacity * sizeof(Task));
    if (heap->arr == NULL) {
        free(heap);
        return NULL;
    }

    heap->size = 0;
    heap->capacity = initial_capacity;
    return heap;
}

/**
 * Role: Free all memory associated with a heap.
 * Time Complexity: O(1)
 */
void free_heap(MaxHeap *heap) {
    if (heap == NULL) {
        return;
    }
    free(heap->arr);
    free(heap);
}

/**
 * Role: Insert a task with priority into max-heap using heapify-up.
 * Time Complexity: O(log n)
 */
int insert_task(MaxHeap *heap, const char *task_name, int priority) {
    Task *slot;

    if (heap == NULL || task_name == NULL) {
        return 0;
    }

    if (!ensure_capacity(heap)) {
        return 0;
    }

    slot = &heap->arr[heap->size];
    strncpy(slot->name, task_name, MAX_TASK_NAME_LEN - 1);
    slot->name[MAX_TASK_NAME_LEN - 1] = '\0';
    slot->priority = priority;

    heapify_up(heap, heap->size);
    heap->size++;
    return 1;
}

/**
 * Role: Extract highest-priority task from heap and optionally show heapify-down steps.
 * Time Complexity: O(log n)
 */
int extract_max(MaxHeap *heap, Task *out_task, int show_steps) {
    if (heap == NULL || out_task == NULL || heap->size == 0) {
        return 0;
    }

    *out_task = heap->arr[0];
    heap->arr[0] = heap->arr[heap->size - 1];
    heap->size--;

    if (show_steps) {
        printf("  After moving last node to root:\n");
        print_heap_tree(heap);
    }

    heapify_down(heap, 0, show_steps);
    return 1;
}

/**
 * Role: Print heap as level-order tree to visualize structure.
 * Time Complexity: O(n)
 */
void print_heap_tree(const MaxHeap *heap) {
    int i;
    int level_nodes = 1;
    int printed = 0;

    if (heap == NULL) {
        return;
    }

    if (heap->size == 0) {
        printf("  [empty heap]\n");
        return;
    }

    printf("  Heap tree (size=%d):\n", heap->size);
    for (i = 0; i < heap->size; i++) {
        printf("  %s(%d) ", heap->arr[i].name, heap->arr[i].priority);
        printed++;
        if (printed == level_nodes) {
            printf("\n");
            level_nodes *= 2;
            printed = 0;
        }
    }
    if (printed != 0) {
        printf("\n");
    }
}

/**
 * Role: Fill heap with n random tasks for simulation of scheduler input.
 * Time Complexity: O(n log n)
 */
void build_random_heap(MaxHeap *heap, int n) {
    int i;
    int *priorities;

    if (heap == NULL || n <= 0) {
        return;
    }

    priorities = (int *)malloc((size_t)n * sizeof(int));
    if (priorities == NULL) {
        return;
    }

    for (i = 0; i < n; i++) {
        priorities[i] = (rand() % 100000) + 1;
    }

    build_heap_from_priorities(heap, priorities, n);
    free(priorities);
}

/**
 * Role: Build max-heap from raw priorities using bottom-up Floyd heap construction.
 * Time Complexity: O(n)
 */
int build_heap_from_priorities(MaxHeap *heap, const int *priorities, int n) {
    int i;

    if (heap == NULL || priorities == NULL || n < 0) {
        return 0;
    }

    while (heap->capacity < n) {
        if (!ensure_capacity(heap)) {
            return 0;
        }
    }

    for (i = 0; i < n; i++) {
        snprintf(heap->arr[i].name, MAX_TASK_NAME_LEN, "Task_%d", i + 1);
        heap->arr[i].priority = priorities[i];
    }
    heap->size = n;

    // Transform step: convert unsorted array into heap-ordered structure in linear time.
    for (i = (n / 2) - 1; i >= 0; i--) {
        heapify_down_build(heap, i);
    }

    return 1;
}

/**
 * Role: Run Q1 demonstration with heap visualization and top-k extraction.
 * Time Complexity: O(n log n + k log n)
 */
void run_q1_demo(void) {
    MaxHeap *heap;
    Task extracted;
    int i;
    int k = 5;

    printf("\n==================== Q1: Real-Time Priority Task Scheduler (Max-Heap) ====================\n");
    srand((unsigned int)time(NULL));

    heap = create_heap(DEMO_TASK_COUNT * 2);
    if (heap == NULL) {
        printf("Heap allocation failed.\n");
        return;
    }

    printf("Building heap from %d random tasks...\n", DEMO_TASK_COUNT);
    build_random_heap(heap, DEMO_TASK_COUNT);
    print_heap_tree(heap);

    printf("\nDemonstrating extract_max with step-by-step heapify-down:\n");
    if (extract_max(heap, &extracted, 1)) {
        printf("Extracted highest-priority task: %s (priority=%d)\n", extracted.name, extracted.priority);
    }

    printf("\nExtracting next %d highest-priority tasks:\n", k);
    for (i = 0; i < k; i++) {
        if (!extract_max(heap, &extracted, 0)) {
            break;
        }
        printf("  #%d -> %s (priority=%d)\n", i + 1, extracted.name, extracted.priority);
    }

    free_heap(heap);
}

/*
Complexity Analysis (Q1 - Transform-and-Conquer using Max-Heap)
Best Case Time:
- insert_task: O(1) when inserted task is not larger than parent.
- extract_max: O(1) when root replacement already satisfies heap order.
- Build from n inserts in this implementation: O(n log n).

Worst Case Time:
- insert_task: O(log n) with full climb to root.
- extract_max: O(log n) with full descent to leaf.
- Build from n inserts: O(n log n).

Average Case Time:
- insert_task: O(log n)
- extract_max: O(log n)
- Build from n inserts: O(n log n)

Space Complexity:
- O(n) for heap array storage.

Why Transform-and-Conquer is optimal here:
- Heap transforms an unsorted set into a partially ordered tree-array representation,
  enabling repeated highest-priority selection in O(log n), significantly better than
  re-sorting the full list after each insertion or extraction in real-time scheduling.
*/
