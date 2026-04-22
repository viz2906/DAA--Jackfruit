#include "floyd_warshall.h"

/**
 * Role: Initialize graph matrix with INF and 0 on diagonal.
 * Time Complexity: O(n^2)
 */
void initialize_graph(int graph[MAX_NODES][MAX_NODES], int n) {
    int i;
    int j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (i == j) {
                graph[i][j] = 0;
            } else {
                graph[i][j] = INF;
            }
        }
    }
}

/**
 * Role: Print a matrix with INF formatting for readability.
 * Time Complexity: O(n^2)
 */
void print_matrix(const char *title, int matrix[MAX_NODES][MAX_NODES], int n) {
    int i;
    int j;

    printf("\n%s\n", title);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (matrix[i][j] >= INF / 2) {
                printf("%7s", "INF");
            } else {
                printf("%7d", matrix[i][j]);
            }
        }
        printf("\n");
    }
}

/**
 * Role: Compute all-pairs shortest paths and predecessor matrix using Floyd-Warshall DP.
 * Time Complexity: O(n^3)
 */
void floyd_warshall(int graph[MAX_NODES][MAX_NODES], int n,
                    int dist[MAX_NODES][MAX_NODES],
                    int pred[MAX_NODES][MAX_NODES]) {
    int i;
    int j;
    int k;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            dist[i][j] = graph[i][j];
            if (i != j && graph[i][j] < INF) {
                pred[i][j] = i;
            } else {
                pred[i][j] = -1;
            }
        }
    }

    for (k = 0; k < n; k++) {
        // DP stage: allow node k as an intermediate for all (i, j) pairs.
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                if (dist[i][k] < INF && dist[k][j] < INF) {
                    int through_k = dist[i][k] + dist[k][j];
                    if (through_k < dist[i][j]) {
                        dist[i][j] = through_k;
                        pred[i][j] = pred[k][j];
                    }
                }
            }
        }
    }
}

/**
 * Role: Reconstruct and print shortest path from i to j using predecessor matrix.
 * Time Complexity: O(n) in path length
 */
void reconstruct_path(int pred[MAX_NODES][MAX_NODES], int i, int j, int n) {
    int path[MAX_NODES];
    int len = 0;
    int cur = j;

    (void)n;

    if (i == j) {
        printf("Path %d -> %d: %d\n", i, j, i);
        return;
    }

    if (pred[i][j] == -1) {
        printf("Path %d -> %d: No path exists\n", i, j);
        return;
    }

    while (cur != i && cur != -1 && len < MAX_NODES) {
        path[len++] = cur;
        cur = pred[i][cur];
    }

    if (cur == -1 || len >= MAX_NODES) {
        printf("Path %d -> %d: Reconstruction failed\n", i, j);
        return;
    }

    path[len++] = i;

    printf("Path %d -> %d: ", i, j);
    while (len > 0) {
        len--;
        printf("%d", path[len]);
        if (len > 0) {
            printf(" -> ");
        }
    }
    printf("\n");
}

/**
 * Role: Check whether the distance matrix indicates a negative weight cycle.
 * Time Complexity: O(n)
 */
int has_negative_cycle(int dist[MAX_NODES][MAX_NODES], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (dist[i][i] < 0) {
            return 1;
        }
    }
    return 0;
}

/**
 * Role: Execute Q2 demo with sample graph, matrix prints, cycle detection, and path queries.
 * Time Complexity: O(n^3)
 */
void run_q2_demo(void) {
    int n = 5;
    int graph[MAX_NODES][MAX_NODES];
    int dist[MAX_NODES][MAX_NODES];
    int pred[MAX_NODES][MAX_NODES];

    printf("\n==================== Q2: Floyd-Warshall All-Pairs Shortest Path ====================\n");

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

    print_matrix("Distance matrix BEFORE Floyd-Warshall:", graph, n);

    floyd_warshall(graph, n, dist, pred);

    print_matrix("Distance matrix AFTER Floyd-Warshall:", dist, n);

    if (has_negative_cycle(dist, n)) {
        printf("Negative weight cycle detected (dist[i][i] < 0).\n");
    } else {
        printf("No negative weight cycle detected.\n");
    }

    printf("\nSample shortest-path reconstructions:\n");
    reconstruct_path(pred, 0, 3, n);
    reconstruct_path(pred, 3, 2, n);
}

/*
Complexity Analysis (Q2 - Dynamic Programming via Floyd-Warshall)
Best Case Time:
- O(n^3), since all triplets (i, j, k) are always evaluated.

Worst Case Time:
- O(n^3), independent of edge pattern.

Average Case Time:
- O(n^3), fixed by algorithmic structure.

Space Complexity:
- O(n^2) for distance and predecessor matrices.

Why Dynamic Programming is optimal here:
- Brute-force all-pairs shortest path by enumerating all simple paths is exponential.
- Floyd-Warshall reuses overlapping subproblems: shortest path from i to j using
  only nodes {0..k} as intermediates, producing a polynomial-time solution.
*/
