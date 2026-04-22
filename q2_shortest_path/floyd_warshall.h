#ifndef FLOYD_WARSHALL_H
#define FLOYD_WARSHALL_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 10
#define INF 99999

void initialize_graph(int graph[MAX_NODES][MAX_NODES], int n);
void floyd_warshall(int graph[MAX_NODES][MAX_NODES], int n,
                    int dist[MAX_NODES][MAX_NODES],
                    int pred[MAX_NODES][MAX_NODES]);
void reconstruct_path(int pred[MAX_NODES][MAX_NODES], int i, int j, int n);
void print_matrix(const char *title, int matrix[MAX_NODES][MAX_NODES], int n);
int has_negative_cycle(int dist[MAX_NODES][MAX_NODES], int n);
void run_q2_demo(void);

#endif
