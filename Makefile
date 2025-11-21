CC = gcc        # compiler
CFLAGS = -Wall  # warning flags

# source files
CFILES = main.c ant_graph.c aco.c

# test files
GRAPH_TESTFILES = test_ant_graph.c ant_graph.c
ACO_TESTFILES   = test_aco.c ant_graph.c aco.c

all: ant graph-test aco-test  # build everything

ant: $(CFILES)
	$(CC) $(CFLAGS) -o ant $(CFILES) -lm

graph-test: $(GRAPH_TESTFILES)
	$(CC) $(CFLAGS) -o graph-test $(GRAPH_TESTFILES) -lm

aco-test: $(ACO_TESTFILES)
	$(CC) $(CFLAGS) -o aco-test $(ACO_TESTFILES) -lm

clean:
	-rm -f ant graph-test aco-test *.out *.exe
