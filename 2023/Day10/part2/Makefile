main: build run clean
	@echo main done
build: main.c
	gcc -Wall -Wextra ./main.c -lm -g -o main

run:
	./main $(FILE)

clean:
	rm main

test:
	gcc -Wall -Wextra ./test.c -lm -o test
	./test
	rm test

dbug: dbug_build dbug_run clean

dbug_build: main.c
	gcc -Wall -Wextra ./main.c -lm -o main -g

dbug_run:
	gdb ./main 

