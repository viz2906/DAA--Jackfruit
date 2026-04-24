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
 * Role: Optional interactive menu for manual max-heap task operations in Q1.
 * Time Complexity: Depends on selected operations; insert/extract are O(log n), peek/print are O(1)/O(n)
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
        printf("  2. Show highest-priority task (peek)\n");
        printf("  3. Remove highest-priority task\n");
        printf("  4. Print heap tree\n");
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
            if (heap->size > 0) {
                printf("Highest-priority task: %s (priority=%d)\n",
                       heap->arr[0].name,
                       heap->arr[0].priority);
            } else {
                printf("Heap is empty.\n");
            }
        } else if (choice == 3) {
            Task out;
            if (extract_max(heap, &out, 1)) {
                printf("Removed highest-priority task: %s (priority=%d)\n", out.name, out.priority);
            } else {
                printf("Heap is empty, nothing to remove.\n");
            }
        } else if (choice == 4) {
            print_heap_tree(heap);
        } else {
            printf("Unknown option. Try again.\n");
        }
    }

    free_heap(heap);
    printf("Exited Q1 manual session.\n");
}

/**
 * Role: Manual interactive menu for Q2 graph editing and Floyd-Warshall execution.
 * Time Complexity: Depends on selected operations; Floyd-Warshall run is O(n^3)
 */
static void run_q2_manual_session(void) {
    int graph[MAX_NODES][MAX_NODES];
    int dist[MAX_NODES][MAX_NODES];
    int pred[MAX_NODES][MAX_NODES];
    int n = 5;
    int choice;
    int has_run = 0;

    initialize_graph(graph, n);

    printf("\n-------------------- Q2 Manual Floyd-Warshall Session --------------------\n");

    while (1) {
        printf("\nCurrent node count: %d\n", n);
        printf("Choose an option:\n");
        printf("  1. Set number of nodes (reset graph)\n");
        printf("  2. Add directed edge (u -> v, weight)\n");
        printf("  3. Remove directed edge (u -> v)\n");
        printf("  4. Print current graph matrix\n");
        printf("  5. Run Floyd-Warshall\n");
        printf("  6. Print distance matrix (after run)\n");
        printf("  7. Reconstruct path i -> j (after run)\n");
        printf("  8. Load sample graph\n");
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
            int new_n;
            printf("Enter node count (1..%d): ", MAX_NODES);
            if (scanf("%d", &new_n) != 1) {
                printf("Invalid input.\n");
                clear_input_buffer();
                continue;
            }
            clear_input_buffer();
            if (new_n < 1 || new_n > MAX_NODES) {
                printf("Out of range.\n");
                continue;
            }
            n = new_n;
            initialize_graph(graph, n);
            has_run = 0;
            printf("Graph reset for %d nodes.\n", n);
        } else if (choice == 2) {
            int u;
            int v;
            int w;
            printf("Enter u v weight: ");
            if (scanf("%d %d %d", &u, &v, &w) != 3) {
                printf("Invalid input.\n");
                clear_input_buffer();
                continue;
            }
            clear_input_buffer();
            if (u < 0 || u >= n || v < 0 || v >= n) {
                printf("Node id out of range 0..%d\n", n - 1);
                continue;
            }
            graph[u][v] = w;
            has_run = 0;
            printf("Edge added: %d -> %d (w=%d)\n", u, v, w);
        } else if (choice == 3) {
            int u;
            int v;
            printf("Enter u v to remove: ");
            if (scanf("%d %d", &u, &v) != 2) {
                printf("Invalid input.\n");
                clear_input_buffer();
                continue;
            }
            clear_input_buffer();
            if (u < 0 || u >= n || v < 0 || v >= n) {
                printf("Node id out of range 0..%d\n", n - 1);
                continue;
            }
            if (u == v) {
                graph[u][v] = 0;
            } else {
                graph[u][v] = INF;
            }
            has_run = 0;
            printf("Edge removed: %d -> %d\n", u, v);
        } else if (choice == 4) {
            print_matrix("Current graph matrix:", graph, n);
        } else if (choice == 5) {
            floyd_warshall(graph, n, dist, pred);
            has_run = 1;
            print_matrix("Distance matrix after Floyd-Warshall:", dist, n);
            if (has_negative_cycle(dist, n)) {
                printf("Negative weight cycle detected (dist[i][i] < 0).\n");
            } else {
                printf("No negative weight cycle detected.\n");
            }
        } else if (choice == 6) {
            if (!has_run) {
                printf("Run Floyd-Warshall first (option 5).\n");
                continue;
            }
            print_matrix("Distance matrix:", dist, n);
        } else if (choice == 7) {
            int i;
            int j;
            if (!has_run) {
                printf("Run Floyd-Warshall first (option 5).\n");
                continue;
            }
            printf("Enter source and destination: ");
            if (scanf("%d %d", &i, &j) != 2) {
                printf("Invalid input.\n");
                clear_input_buffer();
                continue;
            }
            clear_input_buffer();
            if (i < 0 || i >= n || j < 0 || j >= n) {
                printf("Node id out of range 0..%d\n", n - 1);
                continue;
            }
            reconstruct_path(pred, i, j, n);
        } else if (choice == 8) {
            n = 5;
            initialize_graph(graph, n);

            graph[0][1] = 3;
            graph[0][2] = 8;
            graph[0][4] = -4;
            graph[1][3] = 1;
            graph[1][4] = 7;
            graph[2][1] = 4;
            graph[3][0] = 2;
            graph[3][2] = -5;
            graph[4][3] = 6;

            has_run = 0;
            printf("Loaded built-in 5-node sample graph.\n");
            print_matrix("Current graph matrix:", graph, n);
        } else {
            printf("Unknown option. Try again.\n");
        }
    }

    printf("Exited Q2 manual session.\n");
}

/**
 * Role: Print all outgoing dependencies for each node in a graph.
 * Time Complexity: O(V + E)
 */
static void print_q3_edges(Graph *graph) {
    int i;

    if (graph == NULL) {
        printf("Graph is not initialized.\n");
        return;
    }

    printf("Current dependency edges:\n");
    for (i = 0; i < graph->n; i++) {
        AdjNode *cur = graph->heads[i];
        printf("  Node %d (%s):", i, graph->names[i] ? graph->names[i] : "<unnamed>");
        if (cur == NULL) {
            printf(" [none]");
        }
        while (cur != NULL) {
            printf(" -> %d(%s)", cur->dest,
                   graph->names[cur->dest] ? graph->names[cur->dest] : "<unnamed>");
            cur = cur->next;
        }
        printf("\n");
    }
}

/**
 * Role: Manual interactive menu for Q3 graph editing, cycle detection, and topological sort.
 * Time Complexity: Depends on selected operations; detection/sort are O(V + E)
 */
static void run_q3_manual_session(void) {
    Graph *graph = NULL;
    int order[MAX_SERVICES];
    char name_store[MAX_SERVICES][MAX_TASK_NAME_LEN] = {{0}};
    int choice;

    printf("\n-------------------- Q3 Manual Topological Sort Session --------------------\n");

    while (1) {
        printf("\nChoose an option:\n");
        printf("  1. Create/reset graph\n");
        printf("  2. Set service name\n");
        printf("  3. Add dependency edge (src -> dest)\n");
        printf("  4. Detect cycle\n");
        printf("  5. Run topological sort\n");
        printf("  6. Load sample DAG\n");
        printf("  7. Load sample cycle graph\n");
        printf("  8. Print adjacency list\n");
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
            int n;
            printf("Enter node count (1..%d): ", MAX_SERVICES);
            if (scanf("%d", &n) != 1) {
                printf("Invalid input.\n");
                clear_input_buffer();
                continue;
            }
            clear_input_buffer();

            if (n < 1 || n > MAX_SERVICES) {
                printf("Out of range.\n");
                continue;
            }

            free_graph(graph);
            graph = create_graph(n);
            if (graph == NULL) {
                printf("Failed to create graph.\n");
                continue;
            }
            memset(name_store, 0, sizeof(name_store));
            printf("Graph initialized with %d nodes.\n", n);
        } else if (choice == 2) {
            int node;
            char name[MAX_TASK_NAME_LEN];

            if (graph == NULL) {
                printf("Create graph first (option 1).\n");
                continue;
            }

            printf("Enter node id and service name (no spaces): ");
            if (scanf("%d %63s", &node, name) != 2) {
                printf("Invalid input.\n");
                clear_input_buffer();
                continue;
            }
            clear_input_buffer();

            if (node < 0 || node >= graph->n) {
                printf("Node id out of range 0..%d\n", graph->n - 1);
                continue;
            }

            strncpy(name_store[node], name, MAX_TASK_NAME_LEN - 1);
            name_store[node][MAX_TASK_NAME_LEN - 1] = '\0';
            set_service_name(graph, node, name_store[node]);
            printf("Set node %d name to %s\n", node, name);
        } else if (choice == 3) {
            int src;
            int dest;

            if (graph == NULL) {
                printf("Create graph first (option 1).\n");
                continue;
            }

            printf("Enter src dest: ");
            if (scanf("%d %d", &src, &dest) != 2) {
                printf("Invalid input.\n");
                clear_input_buffer();
                continue;
            }
            clear_input_buffer();

            if (src < 0 || src >= graph->n || dest < 0 || dest >= graph->n) {
                printf("Node id out of range 0..%d\n", graph->n - 1);
                continue;
            }

            add_edge(graph, src, dest);
            printf("Added dependency: %d -> %d\n", src, dest);
        } else if (choice == 4) {
            if (graph == NULL) {
                printf("Create graph first (option 1).\n");
                continue;
            }
            if (detect_cycle(graph)) {
                printf("Cycle present in current graph.\n");
            } else {
                printf("No cycle detected.\n");
            }
        } else if (choice == 5) {
            if (graph == NULL) {
                printf("Create graph first (option 1).\n");
                continue;
            }
            if (topological_sort(graph, order)) {
                int i;
                printf("Topological Build Order:\n");
                for (i = 0; i < graph->n; i++) {
                    int node = order[i];
                    printf("  %d. %s (Node %d)\n", i + 1,
                           graph->names[node] ? graph->names[node] : "<unnamed>",
                           node);
                }
            }
        } else if (choice == 6) {
            free_graph(graph);
            graph = create_graph(8);
            if (graph == NULL) {
                printf("Failed to create graph.\n");
                continue;
            }

            memset(name_store, 0, sizeof(name_store));
            strncpy(name_store[0], "AuthService", MAX_TASK_NAME_LEN - 1);
            strncpy(name_store[1], "DatabaseService", MAX_TASK_NAME_LEN - 1);
            strncpy(name_store[2], "UserService", MAX_TASK_NAME_LEN - 1);
            strncpy(name_store[3], "OrderService", MAX_TASK_NAME_LEN - 1);
            strncpy(name_store[4], "PaymentService", MAX_TASK_NAME_LEN - 1);
            strncpy(name_store[5], "NotificationService", MAX_TASK_NAME_LEN - 1);
            strncpy(name_store[6], "APIGateway", MAX_TASK_NAME_LEN - 1);
            strncpy(name_store[7], "DeploymentService", MAX_TASK_NAME_LEN - 1);

            set_service_name(graph, 0, name_store[0]);
            set_service_name(graph, 1, name_store[1]);
            set_service_name(graph, 2, name_store[2]);
            set_service_name(graph, 3, name_store[3]);
            set_service_name(graph, 4, name_store[4]);
            set_service_name(graph, 5, name_store[5]);
            set_service_name(graph, 6, name_store[6]);
            set_service_name(graph, 7, name_store[7]);

            add_edge(graph, 2, 0);
            add_edge(graph, 2, 1);
            add_edge(graph, 3, 2);
            add_edge(graph, 4, 1);
            add_edge(graph, 5, 2);
            add_edge(graph, 6, 2);
            add_edge(graph, 6, 3);
            add_edge(graph, 6, 4);
            add_edge(graph, 7, 6);
            add_edge(graph, 7, 5);

            printf("Loaded built-in DAG sample (8 nodes).\n");
        } else if (choice == 7) {
            free_graph(graph);
            graph = create_graph(3);
            if (graph == NULL) {
                printf("Failed to create graph.\n");
                continue;
            }

            memset(name_store, 0, sizeof(name_store));
            strncpy(name_store[0], "ServiceA", MAX_TASK_NAME_LEN - 1);
            strncpy(name_store[1], "ServiceB", MAX_TASK_NAME_LEN - 1);
            strncpy(name_store[2], "ServiceC", MAX_TASK_NAME_LEN - 1);

            set_service_name(graph, 0, name_store[0]);
            set_service_name(graph, 1, name_store[1]);
            set_service_name(graph, 2, name_store[2]);

            add_edge(graph, 0, 1);
            add_edge(graph, 1, 2);
            add_edge(graph, 2, 0);

            printf("Loaded built-in cycle sample (0->1->2->0).\n");
        } else if (choice == 8) {
            print_q3_edges(graph);
        } else {
            printf("Unknown option. Try again.\n");
        }
    }

    free_graph(graph);
    printf("Exited Q3 manual session.\n");
}

/**
 * Role: Entry point that runs all three DAA assignment demonstrations.
 * Time Complexity: Dominated by benchmark and Floyd-Warshall sections
 */
int main(void) {
    int choice;

    printf("====================================================================\n");
    printf("        DAA Assignment Project (Transform, DP, Decrease)\n");
    printf("====================================================================\n");

    while (1) {
        printf("\nMain Menu:\n");
        printf("  1. Q1 manual task scheduler menu\n");
        printf("  2. Q1 auto demo\n");
        printf("  3. Q1 benchmark\n");
        printf("  4. Q2 manual Floyd-Warshall menu\n");
        printf("  5. Q2 auto demo\n");
        printf("  6. Q3 manual topological sort menu\n");
        printf("  7. Q3 auto demo\n");
        printf("  8. Run all demos + benchmark\n");
        printf("  0. Exit\n");
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
            run_q1_manual_session();
        } else if (choice == 2) {
            run_q1_demo();
        } else if (choice == 3) {
            run_q1_benchmark("benchmark_results.csv");
        } else if (choice == 4) {
            run_q2_manual_session();
        } else if (choice == 5) {
            run_q2_demo();
        } else if (choice == 6) {
            run_q3_manual_session();
        } else if (choice == 7) {
            run_q3_demo();
        } else if (choice == 8) {
            run_q1_demo();
            run_q1_benchmark("benchmark_results.csv");
            run_q2_demo();
            run_q3_demo();
        } else {
            printf("Unknown option. Try again.\n");
        }
    }

    printf("\n====================================================================\n");
    printf("Session ended. Check benchmark_results.csv for Q1 timings if benchmark was run.\n");
    printf("====================================================================\n");

    return 0;
}
