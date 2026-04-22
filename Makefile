CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2
LDFLAGS = -lm
TARGET = daa_assignment

Q1_DIR = q1_priority_scheduler
Q2_DIR = q2_shortest_path
Q3_DIR = q3_topological_sort

OBJS = main.o \
	$(Q1_DIR)/heap_scheduler.o \
	$(Q1_DIR)/benchmark.o \
	$(Q2_DIR)/floyd_warshall.o \
	$(Q3_DIR)/topo_sort.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

main.o: main.c $(Q1_DIR)/heap_scheduler.h $(Q2_DIR)/floyd_warshall.h $(Q3_DIR)/topo_sort.h
	$(CC) $(CFLAGS) -c main.c -o $@

$(Q1_DIR)/heap_scheduler.o: $(Q1_DIR)/heap_scheduler.c $(Q1_DIR)/heap_scheduler.h
	$(CC) $(CFLAGS) -c $(Q1_DIR)/heap_scheduler.c -o $@

$(Q1_DIR)/benchmark.o: $(Q1_DIR)/benchmark.c $(Q1_DIR)/heap_scheduler.h
	$(CC) $(CFLAGS) -c $(Q1_DIR)/benchmark.c -o $@

$(Q2_DIR)/floyd_warshall.o: $(Q2_DIR)/floyd_warshall.c $(Q2_DIR)/floyd_warshall.h
	$(CC) $(CFLAGS) -c $(Q2_DIR)/floyd_warshall.c -o $@

$(Q3_DIR)/topo_sort.o: $(Q3_DIR)/topo_sort.c $(Q3_DIR)/topo_sort.h
	$(CC) $(CFLAGS) -c $(Q3_DIR)/topo_sort.c -o $@

q1: $(Q1_DIR)/heap_scheduler.o $(Q1_DIR)/benchmark.o

q2: $(Q2_DIR)/floyd_warshall.o

q3: $(Q3_DIR)/topo_sort.o

clean:
	-rm -f *.o
	-rm -f $(Q1_DIR)/*.o
	-rm -f $(Q2_DIR)/*.o
	-rm -f $(Q3_DIR)/*.o
	-rm -f $(TARGET).exe
	-rm -f $(TARGET)

.PHONY: all clean q1 q2 q3
