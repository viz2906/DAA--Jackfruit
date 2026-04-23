#include <stdio.h>
#include <string.h>

#include "q1_priority_scheduler/heap_scheduler.h"
#include "q2_shortest_path/floyd_warshall.h"
#include "q3_topological_sort/topo_sort.h"

/**
 * Role: Clear remaining characters from stdin buffer after scanf operations.
 * Time Complexity: O(m), where m is remaining buffered input length
 */
static void clear_input_buffer(void) {
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

/**
 * Role: Optional interactive menu for manual max-heap task insertion/extraction in Q1.
 * Time Complexity: Depends on selected operations; insert/extract are O(log n)
 */
static void run_q1_manual_session(void) {
    MaxHeap *heap;
    int choice;

    heap = create_heap(32);
    if (heap == NULL) {
        printf("Manual session skipped: heap allocation failed.\n");
        return;
    }

    printf("\n-------------------- Q1 Manual Task Scheduler Session --------------------\n");
    printf("Use this menu to insert your own tasks before running auto demos.\n");

    while (1) {
        printf("\nChoose an option:\n");
        printf("  1. Insert task\n");
        printf("  2. Extract highest-priority task\n");
        printf("  3. Print heap tree\n");
        printf("  0. Exit manual session\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (choice == 0) {
            break;
        }

        if (choice == 1) {
            char task_name[MAX_TASK_NAME_LEN];
            int priority;

            printf("Enter task name (no spaces): ");
            if (scanf("%63s", task_name) != 1) {
                printf("Invalid task name.\n");
                clear_input_buffer();
                continue;
            }
            clear_input_buffer();

            printf("Enter priority (integer): ");
            if (scanf("%d", &priority) != 1) {
                printf("Invalid priority.\n");
                clear_input_buffer();
                continue;
            }
            clear_input_buffer();

            if (insert_task(heap, task_name, priority)) {
                printf("Inserted: %s (priority=%d)\n", task_name, priority);
            } else {
                printf("Insert failed.\n");
            }
        } else if (choice == 2) {
            Task out;
            if (extract_max(heap, &out, 1)) {
                printf("Extracted: %s (priority=%d)\n", out.name, out.priority);
            } else {
                printf("Heap is empty, nothing to extract.\n");
            }
        } else if (choice == 3) {
            print_heap_tree(heap);
        } else {
            printf("Unknown option. Try again.\n");
        }
    }

    free_heap(heap);
    printf("Exited Q1 manual session.\n");
}

/**
 * Role: Entry point that runs all three DAA assignment demonstrations.
 * Time Complexity: Dominated by benchmark and Floyd-Warshall sections
 */
int main(void) {
    char use_manual_session[8];

    printf("====================================================================\n");
    printf("        DAA Assignment Project (Transform, DP, Decrease)\n");
    printf("====================================================================\n");

    printf("\nRun manual Q1 task insertion session first? (y/n): ");
    if (scanf("%7s", use_manual_session) == 1) {
        clear_input_buffer();
        if (use_manual_session[0] == 'y' || use_manual_session[0] == 'Y') {
            run_q1_manual_session();
        }
    } else {
        clear_input_buffer();
    }

    run_q1_demo();
    run_q1_benchmark("benchmark_results.csv");

    run_q2_demo();

    run_q3_demo();

    printf("\n====================================================================\n");
    printf("All demos completed. Check benchmark_results.csv for Q1 timings.\n");
    printf("====================================================================\n");

    return 0;
}
