CC = gcc        # compiler
CFLAGS = -Wall  # warning flags

# source files
CFILES = main.c ant_graph.c
TESTFILES = test_ant_graph.c ant_graph.c

all: ant ant-test  # default builds both

ant: $(CFILES)
	$(CC) $(CFLAGS) -o ant $(CFILES)

ant-test: $(TESTFILES)
	$(CC) $(CFLAGS) -o ant-test $(TESTFILES)

clean:
	-rm -f ant ant-test *.out *.exe