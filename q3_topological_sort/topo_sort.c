#include "topo_sort.h"

/**
 * Role: Allocate adjacency-list node for a directed edge.
 * Time Complexity: O(1)
 */
static AdjNode *create_adj_node(int dest) {
    AdjNode *node = (AdjNode *)malloc(sizeof(AdjNode));
    if (node == NULL) {
        return NULL;
    }
    node->dest = dest;
    node->next = NULL;
    return node;
}

/**
 * Role: DFS utility for cycle detection with recursion stack tracking.
 * Time Complexity: O(V + E) over full traversal
 */
static int detect_cycle_dfs(Graph *graph,
                            int node,
                            int visited[MAX_SERVICES],
                            int rec_stack[MAX_SERVICES],
                            int parent[MAX_SERVICES]) {
    AdjNode *cur;

    visited[node] = 1;
    rec_stack[node] = 1;

    cur = graph->heads[node];
    while (cur != NULL) {
        int nxt = cur->dest;

        if (!visited[nxt]) {
            parent[nxt] = node;
            if (detect_cycle_dfs(graph, nxt, visited, rec_stack, parent)) {
                return 1;
            }
        } else if (rec_stack[nxt]) {
            int chain[MAX_SERVICES];
            int len = 0;
            int p = node;

            chain[len++] = nxt;
            while (p != nxt && p != -1 && len < MAX_SERVICES) {
                chain[len++] = p;
                p = parent[p];
            }
            chain[len++] = nxt;

            printf("Cycle detected among services: ");
            while (len > 0) {
                int id;
                len--;
                id = chain[len];
                if (graph->names[id] != NULL) {
                    printf("%s", graph->names[id]);
                } else {
                    printf("Node%d", id);
                }
                if (len > 0) {
                    printf(" -> ");
                }
            }
            printf("\n");
            return 1;
        }

        cur = cur->next;
    }

    rec_stack[node] = 0;
    return 0;
}

/**
 * Role: DFS utility for topological sorting; pushes finished node to explicit stack.
 * Time Complexity: O(V + E) over full traversal
 */
static void topo_dfs(Graph *graph,
                     int node,
                     int visited[MAX_SERVICES],
                     int topo_stack[MAX_SERVICES],
                     int *top,
                     int *unvisited_count) {
    AdjNode *cur;

    visited[node] = 1;
    (*unvisited_count)--;

    // Decrease step: problem reduces to n-1 unvisited nodes after marking current node.
    printf("  Visiting %s, remaining unvisited nodes: %d\n",
           graph->names[node] ? graph->names[node] : "<unnamed>",
           *unvisited_count);

    cur = graph->heads[node];
    while (cur != NULL) {
        if (!visited[cur->dest]) {
            topo_dfs(graph, cur->dest, visited, topo_stack, top, unvisited_count);
        }
        cur = cur->next;
    }

    topo_stack[++(*top)] = node;
}

/**
 * Role: Create directed graph with n nodes.
 * Time Complexity: O(V)
 */
Graph *create_graph(int n) {
    Graph *graph;
    int i;

    if (n < 1 || n > MAX_SERVICES) {
        return NULL;
    }

    graph = (Graph *)malloc(sizeof(Graph));
    if (graph == NULL) {
        return NULL;
    }

    graph->n = n;
    for (i = 0; i < n; i++) {
        graph->heads[i] = NULL;
        graph->names[i] = NULL;
    }

    return graph;
}

/**
 * Role: Assign readable service name label to a node.
 * Time Complexity: O(1)
 */
void set_service_name(Graph *graph, int node, const char *name) {
    if (graph == NULL || node < 0 || node >= graph->n) {
        return;
    }
    graph->names[node] = name;
}

/**
 * Role: Add directed dependency edge src -> dest.
 * Time Complexity: O(1)
 */
void add_edge(Graph *graph, int src, int dest) {
    AdjNode *node;

    if (graph == NULL || src < 0 || src >= graph->n || dest < 0 || dest >= graph->n) {
        return;
    }

    node = create_adj_node(dest);
    if (node == NULL) {
        printf("Failed to allocate edge node (%d -> %d).\n", src, dest);
        return;
    }

    node->next = graph->heads[src];
    graph->heads[src] = node;
}

/**
 * Role: Free graph adjacency lists and graph object.
 * Time Complexity: O(V + E)
 */
void free_graph(Graph *graph) {
    int i;

    if (graph == NULL) {
        return;
    }

    for (i = 0; i < graph->n; i++) {
        AdjNode *cur = graph->heads[i];
        while (cur != NULL) {
            AdjNode *next = cur->next;
            free(cur);
            cur = next;
        }
    }

    free(graph);
}

/**
 * Role: Detect whether a directed graph contains any cycle.
 * Time Complexity: O(V + E)
 */
int detect_cycle(Graph *graph) {
    int visited[MAX_SERVICES] = {0};
    int rec_stack[MAX_SERVICES] = {0};
    int parent[MAX_SERVICES];
    int i;

    if (graph == NULL) {
        return 0;
    }

    for (i = 0; i < graph->n; i++) {
        parent[i] = -1;
    }

    for (i = 0; i < graph->n; i++) {
        if (!visited[i]) {
            if (detect_cycle_dfs(graph, i, visited, rec_stack, parent)) {
                return 1;
            }
        }
    }

    return 0;
}

/**
 * Role: Compute topological order using DFS and an explicit stack.
 * Time Complexity: O(V + E)
 */
int topological_sort(Graph *graph, int order[MAX_SERVICES]) {
    int visited[MAX_SERVICES] = {0};
    int topo_stack[MAX_SERVICES];
    int top = -1;
    int unvisited_count;
    int i;
    int out_idx = 0;

    if (graph == NULL) {
        return 0;
    }

    if (detect_cycle(graph)) {
        printf("Topological sort aborted: circular dependency detected.\n");
        return 0;
    }

    unvisited_count = graph->n;

    for (i = 0; i < graph->n; i++) {
        if (!visited[i]) {
            topo_dfs(graph, i, visited, topo_stack, &top, &unvisited_count);
        }
    }

    while (top >= 0) {
        order[out_idx++] = topo_stack[top--];
    }

    return 1;
}

/**
 * Role: Print a topological order with service labels.
 * Time Complexity: O(V)
 */
static void print_topo_order(Graph *graph, int order[MAX_SERVICES]) {
    int i;
    printf("Topological Build Order:\n");
    for (i = 0; i < graph->n; i++) {
        int node = order[i];
        printf("  %d. %s (Node %d)\n", i + 1,
               graph->names[node] ? graph->names[node] : "<unnamed>",
               node);
    }
}

/**
 * Role: Execute Q3 demo with both DAG and cycle test cases.
 * Time Complexity: O(V + E)
 */
void run_q3_demo(void) {
    Graph *dag;
    Graph *cyclic;
    int order[MAX_SERVICES];

    printf("\n==================== Q3: Topological Sort for Microservice Build Order ====================\n");

    dag = create_graph(8);
    if (dag == NULL) {
        printf("Failed to allocate DAG graph.\n");
        return;
    }

    set_service_name(dag, 0, "AuthService");
    set_service_name(dag, 1, "DatabaseService");
    set_service_name(dag, 2, "UserService");
    set_service_name(dag, 3, "OrderService");
    set_service_name(dag, 4, "PaymentService");
    set_service_name(dag, 5, "NotificationService");
    set_service_name(dag, 6, "APIGateway");
    set_service_name(dag, 7, "DeploymentService");

    add_edge(dag, 2, 0);
    add_edge(dag, 2, 1);
    add_edge(dag, 3, 2);
    add_edge(dag, 4, 1);
    add_edge(dag, 5, 2);
    add_edge(dag, 6, 2);
    add_edge(dag, 6, 3);
    add_edge(dag, 6, 4);
    add_edge(dag, 7, 6);
    add_edge(dag, 7, 5);

    printf("\nDAG case (8 microservices):\n");
    if (topological_sort(dag, order)) {
        print_topo_order(dag, order);
    }

    free_graph(dag);

    cyclic = create_graph(3);
    if (cyclic == NULL) {
        printf("Failed to allocate cycle-test graph.\n");
        return;
    }

    set_service_name(cyclic, 0, "ServiceA");
    set_service_name(cyclic, 1, "ServiceB");
    set_service_name(cyclic, 2, "ServiceC");

    add_edge(cyclic, 0, 1);
    add_edge(cyclic, 1, 2);
    add_edge(cyclic, 2, 0);

    printf("\nCycle test case (0->1->2->0):\n");
    if (!topological_sort(cyclic, order)) {
        printf("Cycle test behaved correctly: build order rejected.\n");
    }

    free_graph(cyclic);
}

/*
Complexity Analysis (Q3 - Decrease-and-Conquer via DFS Topological Sort)
Best Case Time:
- O(V + E), every vertex and edge is visited at most once.

Worst Case Time:
- O(V + E), dense or sparse graph still traversed linearly in representation size.

Average Case Time:
- O(V + E).

Space Complexity:
- O(V + E) for adjacency list, O(V) auxiliary arrays and stack.

Why Decrease-and-Conquer is optimal here:
- DFS reduces problem size by one visited node per recursive expansion and pushes
  resolved nodes onto a stack. Brute-force permutation checking is factorial, while
  DFS provides linear-time ordering for DAGs and naturally supports cycle detection.
*/
