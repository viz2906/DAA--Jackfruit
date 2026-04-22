#include <stdio.h>

#include "q1_priority_scheduler/heap_scheduler.h"
#include "q2_shortest_path/floyd_warshall.h"
#include "q3_topological_sort/topo_sort.h"

/**
 * Role: Entry point that runs all three DAA assignment demonstrations.
 * Time Complexity: Dominated by benchmark and Floyd-Warshall sections
 */
int main(void) {
    printf("====================================================================\n");
    printf("        DAA Assignment Project (Transform, DP, Decrease)\n");
    printf("====================================================================\n");

    run_q1_demo();
    run_q1_benchmark("benchmark_results.csv");

    run_q2_demo();

    run_q3_demo();

    printf("\n====================================================================\n");
    printf("All demos completed. Check benchmark_results.csv for Q1 timings.\n");
    printf("====================================================================\n");

    return 0;
}
