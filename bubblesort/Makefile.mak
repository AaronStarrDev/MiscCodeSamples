HEADERS = bubble_sort.h unit_tests.h

default: test

test:	bubble_sort.o unit_tests.o
	gcc -o test bubble_sort.o unit_tests.o
	
bubble_sort.o: bubble_sort.c $(HEADERS)
	gcc -c bubble_sort.c

unit_tests.o: unit_tests.c $(HEADERS)
	gcc -c unit_tests.c

clean:
	-rm -f bubble_sort.o
	-rm -f unit_tests.o
	-rm -f test
	