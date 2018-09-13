.PHONY:clean
test_sem:test_sem.o sem.o
	gcc test_sem.o sem.o -o test_sem

test_sem.o:test_sem.c
	gcc -c test_sem.c -o test_sem.o

sem.o:sem.c
	gcc -c sem.c -o sem.o
	
clean:
	rm -rf *.o
