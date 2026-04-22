#ifndef TOPO_SORT_H
#define TOPO_SORT_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_SERVICES 20

typedef struct AdjNode {
    int dest;
    struct AdjNode *next;
} AdjNode;

typedef struct {
    int n;
    AdjNode *heads[MAX_SERVICES];
    const char *names[MAX_SERVICES];
} Graph;

Graph *create_graph(int n);
void set_service_name(Graph *graph, int node, const char *name);
void add_edge(Graph *graph, int src, int dest);
void free_graph(Graph *graph);
int detect_cycle(Graph *graph);
int topological_sort(Graph *graph, int order[MAX_SERVICES]);
void run_q3_demo(void);

#endif
