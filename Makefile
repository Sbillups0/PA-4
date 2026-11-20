CC = gcc
CFLAGS = -Wall -g

memsimulator: main.c memory.c pagetable.c tlb.c replacement.c memory.h pagetable.h tlb.h replacement.h
	$(CC) $(CFLAGS) -o memsimulator main.c memory.c pagetable.c tlb.c replacement.c

clean:
	rm -rf memsimulator memsimulator.dSYM *.o out.txt